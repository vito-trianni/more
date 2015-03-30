#!/bin/bash
#
# This script helps you to associate controllers with a real robot.
# It configures the build system so that you can build controllers
# for the specified real robot platform.
#
# Author: Arne Brutschy <arne.brutschy@ulb.ac.be>


###############################################################################
# Displays a small explanation for the user.
#
function info() {
  echo
  echo "This script helps you to associate controllers with a real robot."
  echo "It configures the build system so that you can build controllers"
  echo "for the specified real robot platform."
  echo
  echo "You don't have to associate a controller; if you choose to leave"
  echo "the controller unassociated, you can change it's association at a"
  echo "later point using this script."
  echo 
  echo "Please note that a controller can only be associated to a single"
  echo "platform."
  echo 
}

###############################################################################
# Displays usage information.
#
function usage() {
  echo "Usage: `basename $0` <controller_name> [robot_type]"
  info
}

###############################################################################
# Outputs the list of controllers for a specific robot type.
# PARAM1: name of controller
# PARAM2: requested robot type
# PARAM3: robot type we're creating the list for
# PARAM4: variable name in Makefile.am of this robot
#
function generate_controllers_list() {
  local CONTROLLER_NAME=$1
  local REQ_ROBOT_TYPE=$2
  local THIS_ROBOT_TYPE=$3
  local THIS_ROBOT_VAR=$4

  # read this robot's controllers
  local MY_CONTROLLERS=`grep "${THIS_ROBOT_VAR} =" Makefile.am | \
                        sed "s/${THIS_ROBOT_VAR} =\(.*\)/\1/"`
  
  # remove the controller from the list
  local MY_CONTROLLERS=`echo ${MY_CONTROLLERS} | sed "s/ ${CONTROLLER_NAME}//g" |
                                                 sed "s/^${CONTROLLER_NAME}//g"`

  # check if this robot is of the requested type
  if [ "${REQ_ROBOT_TYPE}" == "${THIS_ROBOT_TYPE}" ]; then
    # yes, add the controller to the list
    local MY_CONTROLLERS=`echo "${MY_CONTROLLERS} ${CONTROLLER_NAME}"`
  fi

  # output list of controllers
  echo "${MY_CONTROLLERS}" | sed 's/^\ *\(.*\)\ *$/\1/'
}

###############################################################################
# This funtion generates the AC_CONFIG_FILES section of
# the configure.ac for all unassociated controllers.
#
function output_configure_section_crosscompiling() {  
  echo

  # output a configure option block for every robot, including conditional and rules
  for I in `seq 0 $((${NUM_ROBOTS}-1))`; do
    if [ ${ROBOT_SIMONLY[$I]} = 0 ]; then
      echo "# ${ROBOT_TYPES[$I]}"
      echo "AC_MSG_CHECKING(whether to build for the ${ROBOT_TYPES[$I]})"
      echo "${ROBOT_TYPES[$I]}cross=no"
      echo "AC_ARG_ENABLE(${ROBOT_TYPES[$I]},"
      echo "  AS_HELP_STRING([--enable-${ROBOT_TYPES[$I]}],[enable the compilation for the ${ROBOT_TYPES[$I]} (default=no)]),"
      echo "    ${ROBOT_TYPES[$I]}cross=yes)"
      echo "AC_MSG_RESULT(\$${ROBOT_TYPES[$I]}cross)"
      echo -e "AM_CONDITIONAL(${ROBOT_VARS[$I]}CROSS, test \"\$${ROBOT_TYPES[$I]}cross\" = yes)"
      echo
    fi
  done

  # output general crosscompiling conditional
  echo '# general crosscompiling flags'
  echo 'AM_CONDITIONAL(NOTCROSS, test "$cross_compiling" = no)'
  echo 'if test x"$cross_compiling" = x"yes"; then'
  echo '  echo -n "checking for presence of controller specific environment variables... "'
  echo -e "  if test x\"\$CONTROLLER_MAIN\" == x\"\" || \\"
  echo -e "     test x\"\$CONTROLLER_LDFLAGS\" == x\"\" || \\"
  echo '     test x"$CONTROLLER_DEPENDENCIES" == x""; then'
  echo '    echo "no"'
  echo '    echo "Error: You have to define the following environment variables for real robot building:"'
  echo '    echo'
  echo '    echo "  CONTROLLER_MAIN          main function for the real robot"'
  echo '    echo "  CONTROLLER_LDFLAGS       robot specific linking flags"'
  echo '    echo "  CONTROLLER_DEPENDENCIES  depencencies for controllers of this robot type"'
  echo '    echo'
  echo '    echo "These environment variables should have been set by your setup_environment.sh script."'
  echo '    exit 1'
  echo '  fi'
  echo '  AC_SUBST(CONTROLLER_MAIN)'
  echo '  AC_SUBST(CONTROLLER_LDFLAGS)'
  echo '  AC_SUBST(CONTROLLER_DEPENDENCIES)'
  echo '  CPPFLAGS="$CPPFLAGS -DCROSSCOMPILING"'
  echo 'fi'
  echo
}

###############################################################################
# This funtion generates the AC_CONFIG_FILES section of
# the configure.ac for all unassociated controllers.
#
function output_configure_section_controllers() {
  # first, ouput a AC_CONFIG_FILES sections for all unassociated/simulationonly controllers
  echo
  if [ "${CONTROLLERS_UNASS}" != "" ]; then
    echo "# Makefile for simulation only."
    echo "# Unassociated controllers (only used for simulation) go here."
    echo "if test x\"\$cross_compiling\" = x\"no\"; then"
    echo "  AC_CONFIG_FILES([\\"
    for CONTROLLER in ${CONTROLLERS_UNASS}; do
      echo "    controllers/${CONTROLLER}/Makefile \\"
    done
    echo "  ])"
    echo "fi"
    echo
  else
    echo "# No controllers are currently unassociated."
    echo "# Omitting AC_CONFIG_FILES section for simulation-only controllers."
    echo
  fi

  # now, output for every robot a AC_CONFIG_FILES section
  for I in `seq 0 $((${NUM_ROBOTS}-1))`; do
    if [ ${ROBOT_SIMONLY[$I]} = 0 ]; then
      if [ "${ROBOT_CONTROLLERS[$I]}" != "" ]; then
        echo "# Makefile for ${ROBOT_TYPES[$I]} included in simulation and cross compiling."
        echo "# Controllers which are associated with the ${ROBOT_TYPES[$I]} go here."
        echo "# These controllers are compiled for simulation as well."
        echo "if test x\"\$cross_compiling\" = x\"no\" || test x\"\$${ROBOT_TYPES[$I]}cross\" = x\"yes\"; then"
        echo "  AC_CONFIG_FILES([\\"
        for CONTROLLER in ${ROBOT_CONTROLLERS[$I]}; do
          echo "    controllers/${CONTROLLER}/Makefile \\"
        done
        echo "  ])"
        echo "fi"
        echo
      else
        echo "# No controllers are currently associated with the ${ROBOT_TYPES[$I]}."
        echo "# Omitting AC_CONFIG_FILES section for ${ROBOT_TYPES[$I]}."
        echo
      fi
    fi
  done
}

###############################################################################
# Write the controllers/Makefile.am from scratch.
#
function output_controllers_makefile_am() {
  echo "# This is the list of all controllers known to the build system."
  echo "# Each existing controller should be listed in the SIMULATION variable."
  echo "#"
  echo "# If you want to associate a controller with a specific robot,"
  echo "# use the associate_controller.sh script in your controller directory."
  echo "#"
  echo "# WARNING: THIS FILE GETS CREATED AUTOMATICALLY BY THE CONTROLLER"
  echo "#          ASSOCIATION SCRIPT. LOCAL CHANGES WILL BE OVERWRITTEN!"
  echo "#"

  # list all controllers in the SIMULATION variable
  echo -n "SIMULATION = "
  for CONTROLLER in ${CONTROLLERS_ALL}; do
    echo -n "${CONTROLLER} "
  done
  echo

  # for all robot types, output the appropriate variable and the associated controllers
  for I in `seq 0 $((${NUM_ROBOTS}-1))`; do
    if [ ${ROBOT_SIMONLY[$I]} = 0 ]; then
      echo -n "${ROBOT_VARS[$I]} = "
      for CONTROLLER in ${ROBOT_CONTROLLERS[$I]}; do
        echo -n "${CONTROLLER} "
      done
      echo
    fi
  done
  echo

  # header for SUBDIRS section and SUBDIRS for simulation
  echo "#"
  echo "# DON'T change anything behind this line"
  echo "#"
  echo 'if NOTCROSS'
  echo 'SUBDIRS = ${SIMULATION}'
  echo 'endif'

  # for all robot types, create an appropriate cross compilation section
  for I in `seq 0 $((${NUM_ROBOTS}-1))`; do
    if [ ${ROBOT_SIMONLY[$I]} = 0 ]; then
      echo "if ${ROBOT_VARS[$I]}CROSS"
      echo -e "SUBDIRS = \${${ROBOT_VARS[$I]}}"
      echo "endif"
    fi
  done

  #
}

###############################################################################
# Creates an index dirctory for each robot type and adds links for
# each controller associated with this type.
#
function create_robot_type_index() {
  for I in `seq 0 $((${NUM_ROBOTS}-1))`; do
    # remove previous link directories
    if [ -d "all_${ROBOT_TYPES[$I]}_controllers" ]; then
      svn --force delete "all_${ROBOT_TYPES[$I]}_controllers" &> /dev/null
    fi

    # create index dirs for all used robot types
    if [ "${ROBOT_CONTROLLERS[$I]}" != "" ]; then
      # create the index dir if it does not exist already
      mkdir -p "all_${ROBOT_TYPES[$I]}_controllers" &> /dev/null
      # then (re)add the index dir to the svn
      svn add "all_${ROBOT_TYPES[$I]}_controllers" &> /dev/null

      # link each controller into the directory and (re)add this link to the svn
      for CONTROLLER in ${ROBOT_CONTROLLERS[$I]}; do
        cd "all_${ROBOT_TYPES[$I]}_controllers"
        ln -s ../${CONTROLLER} &> /dev/null
        svn add "${CONTROLLER}" &> /dev/null
        cd ..
      done      
    fi
  done
}

###############################################################################
# Main starts here.
###############################################################################

# check if we got a basedir
if [ "${AHSSINSTALLDIR}" == "" ]; then
  echo "Error: You need to set the AHSSINSTALLDIR environment variable!" 1>&2
  exit 1
fi
if [ ! -e ${AHSSINSTALLDIR}/user/user_scripts/ahss_script_functions ]; then
  echo "Error: Your AHSSINSTALLDIR environment variable is not set properly!" 1>&2
  exit 1
fi

# source the common script functions
source ${AHSSINSTALLDIR}/user/user_scripts/ahss_script_functions

# do some basic checks
default_checks
default_user_checks

# check if we're in the right directory
check_controller_directory

# check if we got a controller name
if [ "$1" == "" ]; then
  usage
  exit 1
fi

# strip slashes from the controllers name
CONTROLLER_NAME=`echo $1 | sed 's/\///g'`

# read known controllers and their association
CONTROLLERS_ALL=`grep "SIMULATION = " Makefile.am | sed 's/SIMULATION = \(.*\)/\1/'`

# check if the requested controller is known
PATTERN="(^${CONTROLLER_NAME}| ${CONTROLLER_NAME} |${CONTROLLER_NAME}$)"
if [[ ! ${CONTROLLERS_ALL} =~ ${PATTERN} ]]; then
  echo "Error: the controller '${CONTROLLER_NAME}' is not known!" 1>&2
  echo "List of known controllers:"
  for CONTROLLER in $CONTROLLERS_ALL; do
    echo "    $CONTROLLER"
  done
  exit 1
fi

# check if we got a robot type or if we're in interactive mode
if [ "$2" == "" ]; then
  info
  get_robot_type 0
else
  REQ_ROBOT_TYPE=$2
fi

# check robot type to make sure
check_robot_type ${REQ_ROBOT_TYPE} 0
if [ $? == 0 ]; then
  echo "Error: you specified an invalid robot type." 1>&2
  echo "List of known robot types:"
  for I in `seq 0 $((${NUM_ROBOTS}-1))`; do
    echo "    ${ROBOT_TYPES[$I]}"
  done
  exit 1
fi

echo "Associating controller '${CONTROLLER_NAME}' with the ${REQ_ROBOT_TYPE}."

# get the list of associated controllers of each robot type
for I in `seq 0 $((${NUM_ROBOTS}-1))`; do
  ROBOT_CONTROLLERS[$I]=`generate_controllers_list ${CONTROLLER_NAME} ${REQ_ROBOT_TYPE} ${ROBOT_TYPES[$I]} ${ROBOT_VARS[$I]}`  
done

# create the list of unassociated controllers
for CONTROLLER in ${CONTROLLERS_ALL}; do
  UNASS=1

  # filter controller that are associated
  for I in `seq 0 $((${NUM_ROBOTS}-1))`; do

    if [[ ${ROBOT_SIMONLY[$I]} == 0 && ${ROBOT_CONTROLLERS[$I]} =~ ${CONTROLLER} ]]; then
      UNASS=0; 
    fi
  done

  # append controller if not used
  if [ ${UNASS} == 1 ]; then
    CONTROLLERS_UNASS="${CONTROLLERS_UNASS} ${CONTROLLER}"
  fi
done

# output the current configuration and detected controllers when we're in debugging mode
if [ $DEBUG ]; then
  echo "===list of all controllers"
  for CONTROLLER in ${CONTROLLERS_ALL}; do
    echo "  ${CONTROLLER}"
  done

  echo "===list of robots with associated controllers"
  for I in `seq 0 $((${NUM_ROBOTS}-1))`; do
    if [ ${ROBOT_SIMONLY[$I]} = 1 ]; then
      echo "${ROBOT_TYPES[$I]} is simulation only!"
    else
      echo "${ROBOT_TYPES[$I]}:"
      for CONTROLLER in ${ROBOT_CONTROLLERS[$I]}; do  
        echo "  ${CONTROLLER}"
      done
    fi
  done
  echo "===list of all unassociated controllers"
  for CONTROLLER in ${CONTROLLERS_UNASS}; do
    echo "  ${CONTROLLER}"
  done

  echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
  output_controllers_makefile_am
  echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
  output_configure_section_crosscompiling
  echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
  output_configure_section_controllers
  echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
fi

# create robot type subdirs with links to all controllers associated with it
echo "  creating index directories..."
create_robot_type_index

# writing controllers/Makefile.am
echo "  writing Makefile.am"
output_controllers_makefile_am > Makefile.am

# writing configure.ac
echo "  writing ../configure.ac"
output_configure_section_crosscompiling > configure.ac.crosscompiling.tmp
output_configure_section_controllers > configure.ac.controllers.tmp
replace_between_infile "CROSSCOMPILING" configure.ac.crosscompiling.tmp ../configure.ac
replace_between_infile "CONTROLLERS" configure.ac.controllers.tmp ../configure.ac
rm -f configure.ac.crosscompiling.tmp configure.ac.controllers.tmp

exit 0
