#!/bin/bash
#
# Author: Carlo Pinciroli <cpinciro@ulb.ac.be>
#         Arne Brutschy <arne.brutschy@ulb.ac.be>
#
# This script creates a new controller using the sample_footbot_controller
# from the template user as template.
#
AWK=
SED=


###############################################################################
# Displays a welcome message.
#
function welcome_message (  ) {
  echo
  echo "+++++   [ CREATE A NEW CONTROLLER ]   +++++"
  echo
  echo "This script helps you creating a new controller. Please select"
  echo "the robot type you want to create a new controller for."
  echo
}

###############################################################################
# Checks that the needed programs are present.
#
function check_requirements (  ) {
  if ( which awk &> /dev/null ); then
    AWK=`which awk`
  else
    echo "[FATAL] Unable to execute the script: awk has not been found."
    exit -1
  fi
  if ( which sed &> /dev/null ); then
    SED=`which sed`
  else
    echo "[FATAL] Unable to execute the script: sed has not been found."
    exit -1
  fi
}

###############################################################################
# Checks if a subdirectory with the passed name already exists.
# PARAM : $1   The subdirectory name to check
# RETURN: 0    If the subdirectory exists, 1 otherwise.
#
function directory_exists (  ) {
  if [ -e "$1" ]; then
    echo
    echo "A directory called $1 already exists, choose a different name."
    return 0;
  else
    return 1;
  fi
}

###############################################################################
# Creates a subdirectory with the passed name.
# PARAM $1: The directory name.
#
function create_directory (  ) {
  echo "Creating directory $1..."
  mkdir "$1"
  if [ ! $? -eq 0 ]; then
    echo "Unexpected error while creating directory $1" 1>&2
    echo "Check if you have the right permissions."
    exit 1
  fi
  echo "Success."
}

###############################################################################
# Asks the user to type the wanted controller tag name until a name of a
# non-existing subdirectory is given. Then the name is returned.
# RETURN: The controller tag name.
#
function get_controller_tag_name (  ) {
  local tagname
  local controller_name="new_controller"

  read -ep "Insert controller XML tag name [$controller_name]: " tagname
  if [ x"$tagname" = x ]; then
    tagname="$controller_name"
  fi

  while ( test "x$tagname" = x || directory_exists "$tagname" ); do
    read -ep "Insert controller XML tag name: " tagname
  done
  echo $tagname
}

###############################################################################
# Asks the user to type the wanted controller class name until a name of a
# non-existing subdirectory is given. Then the name is returned.
# RETURN: The controller class name.
#
function get_controller_class_name (  ) {
  local classname
  local controller_name="CNewController"

  read -ep "Insert controller class name [$controller_name]: " classname
  if [ x"$classname" = x ]; then
    classname="$controller_name"
  fi

  echo $classname
}

###############################################################################
# Copies all the files in .template to the given subdirectory.
# PARAM $1: The controller tag name.
# PARAM $2: The controller class name.
# PARAM $3: The desired robot type.
#
function populate_structure (  ) {
  local robottype=$3

  echo "Populating controller directory..."  
  if [ ! -e ../../.template/controllers/${robottype}_sample_controller ]; then
    echo "Error: The controller template directory ../../.template/controllers/${robottype}_sample_controller" 1>&2
    echo "does not exist. This should not happen. I'm sorry, but someone messed up." 1>&2
    exit 1
  fi

  echo "Using .template/controllers/${robottype}_sample_controller as template..."
  cp -rf ../../.template/controllers/${robottype}_sample_controller/* "$1"
  if [ ! $? -eq 0 ]; then
    echo "Unexpected error while creating files in directory $1" 1>&2
    echo "Check if you have the right permissions." 1>&2
    exit 1
   else
    local upcase
 
    # Source the conversion rules and delete the rule file afterwards.
    source $1/convert_rules
    rm -f $1/convert_rules
    # Remove the .svn files that came from the template dir
    find "$1" -name .svn | xargs rm -rf
    # Rename copied files
    echo "Renaming files..."
    cd "$1"
    mv ${robottype}_sample_controller.h "$1.h"
    mv ${robottype}_sample_controller.cpp "$1.cpp"
    # Retouch the files with the chosen names
    echo "Fixing the code..."
    upcase="`echo $2 | tr '[:lower:]' '[:upper:]'`"
    $SED -i "s/C${CAMEL}SampleController/$2/g" "$1.h" "$1.cpp"
    $SED -i "s/C${UPPER}SAMPLECONTROLLER/$upcase/g" "$1.h"
    $SED -i "s/${LOWER}_sample_controller/$1/g" "$1.h" "$1.cpp" Makefile.am
    cd ..
    # Fix the compilation environment
    echo "Fixing the compilation environment..."
    $AWK "/^SIMULATION =/ {print \$0,\" $1\"} ! /^SIMULATION =/ {print \$0}" Makefile.am | $SED "s/  / /g" > Makefile.am.new
    mv Makefile.am.new Makefile.am
  fi
  echo "Success."
}

###############################################################################
# Associate controller.
#
function associate_controller ( ) {
  if [ ! $? -eq 0 ]; then
    echo "Error: Unexpected error while associating controller." 1>&2
    exit 1
  else
    ${AHSSINSTALLDIR}/user/user_scripts/associate_controller.sh $1 $2
  fi
}

###############################################################################
# Displays the real robot info.
#
function real_robot_info (  ) {
  echo
  echo "In order to compile your new controller on a real robot,"
  echo "use the script 'build_real_robot_framework.sh' in your"
  echo '${AHSSINSTALLDIR} directory.'
  echo
  echo "The script fetches the toolchain, builds the required"
  echo "libraries and the actual controller."
  echo
}


###############################################################################
# Main program
###############################################################################
function main (  ) {
  local tagname
  local classname

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

  welcome_message
  check_requirements
  get_robot_type 0
  robottype=${REQ_ROBOT_TYPE}
  tagname=`get_controller_tag_name`
  classname=`get_controller_class_name`
  create_directory "$tagname"
  populate_structure "$tagname" "$classname" "$robottype"
  associate_controller "$tagname" "$robottype"
  real_robot_info
}

# Execution starts here
main
