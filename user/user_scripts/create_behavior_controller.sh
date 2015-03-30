#!/bin/bash
#
# Author: Carlo Pinciroli <cpinciro@ulb.ac.be>
#         Arne Brutschy <arne.brutschy@ulb.ac.be>
# 		  Eliseo Ferrante <eferrant@ulb.ac.be>
#         Nithin Mathews <nmathews@ulb.ac.be@ulb.ac.be>
#
# This script creates a new controller using the sample_bt_controller
# from user nithin as template.
#
AWK=
SED=


###############################################################################
# Displays a welcome message.
#
function welcome_message (  ) {
  echo
  echo "+++++   [ CREATE A NEW BEHAVIOR CONTROLLER ]   +++++"
  echo
  echo "This script helps you creating a new BEHAVIOR controller. Please select"
  echo "the robot type you want to create the new BEHAVIOR controller for."
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
# PARAM $1: The robot type
# RETURN: The controller tag name.
#
function get_controller_tag_name (  ) {
  local tagname
  local controller_name="bt_$1_new_controller"

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
# Capitalizes initial character
# PARAM $1: string to be capitalzed
#
capitalize_ichar ()          #  Capitalizes initial character
{                            #+ of argument string(s) passed.

  string0="$@"               # Accepts multiple arguments.

  firstchar=${string0:0:1}   # First character.
  string1=${string0:1}       # Rest of string(s).

  FirstChar=`echo "$firstchar" | tr a-z A-Z` # Capitalize first character.

  echo "$FirstChar$string1"  # Output to stdout.

}


###############################################################################
# Convert the xml tag name of the controller to a possible controller class name
# PARAM $1: xml tag name
#
convert_tag_to_classname ()
{                           
	OLD_IFS="$IFS"
	IFS="_"
	class_parts=( $1 )
	IFS="$OLD_IFS"
	
	for part in ${class_parts[*]}
	do
	    class=${class}`capitalize_ichar $part`
	done
	
	echo "$class"

}



###############################################################################
# Asks the user to type the wanted controller class name until a name of a
# non-existing subdirectory is given. Then the name is returned.
# PARAM $1: xml tag name
# RETURN: The controller class name.
#
function get_controller_class_name (  ) {
  local classname
  local robot
  if [ $REQ_ROBOT_TYPE == footbot ]; then
    robot="FootBot"
  fi
  if [ $REQ_ROBOT_TYPE == handbot ]; then
    robot="HandBot"
  fi
  if [ $REQ_ROBOT_TYPE == eyebot ]; then
    robot="EyeBot"
  fi
  local controller_name="C"$robot"NewController"

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
# PARAM $4: The root behavior tag name
# PARAM $5: The root behavior class name
#
function populate_structure (  ) {
  local robottype=$3
  
  if [[ $EXPERIENCED_USER == "1" ]] 
  then	
  	path_to_template="$AHSSINSTALLDIR/user/.template/controllers/sample_bt_advanced_controller"
  else	
  	path_to_template="$AHSSINSTALLDIR/user/.template/controllers/sample_bt_controller"
  fi

  echo "Populating controller directory..."  
  if [ ! -e $path_to_template ]; then
    echo "Error: The controller template directory $path_to_template" 1>&2
    echo "does not exist. This should not happen. I'm sorry, but someone messed up." 1>&2
    exit 1
  fi

  echo "Using $path_to_template as template..."
  cp -rf $path_to_template/* "$1"
  if [ ! $? -eq 0 ]; then
    echo "Unexpected error while creating files in directory $1" 1>&2
    echo "Check if you have the right permissions." 1>&2
    exit 1
   else
    local upcase

    
    # Remove the .svn files that came from the template dir
    find "$1" -name .svn | xargs rm -rf
    
    # Rename copied files
    echo "Renaming behavior controller and root behavior files..."
    cd "$1"
    mv sample_bt_controller.h "$1.h"
    mv sample_bt_controller.cpp "$1.cpp"
    mv sample_behavior.h "$4.h"
    mv sample_behavior.cpp "$4.cpp"
    
    # Retouch the controller files with the chosen names for the controller
    echo "Fixing the code..."
    upcase="`echo $2 | tr '[:lower:]' '[:upper:]'`"
    $SED -i "s/CBTFootBootSampleController/$2/g" "$1.h" "$1.cpp"
    $SED -i "s/CBTFOOTBOOTSAMPLECONTROLLER/$upcase/g" "$1.h"
    $SED -i "s/sample_behavior/$4/g" "$1.h"
    $SED -i "s/CBTFootbotSampleBehavior/$5/g" "$1.cpp"
    $SED -i "s/sample_bt_controller/$1/g" "$1.h" "$1.cpp" Makefile.am
    $SED -i "s/demo_bt_behavior/$4/g" "$4.cpp"
    
  if [ ! $? -eq 0 ]; then
    echo "Unexpected error while creating files (.h and .cpp) $1" 1>&2
    echo "Check if you have the right permissions." 1>&2
    exit 1
   else
    local upcase
 
    # Retouch the behavior files with the chosen names
    upcase="`echo $5 | tr '[:lower:]' '[:upper:]'`"
    $SED -i "s/CBTFootbotSampleBehavior/$5/g" "$4.h" "$4.cpp"
    $SED -i "s/CBTFOOTBOTSAMPLEBEHAVIOR/$upcase/g" "$4.h"
    $SED -i "s/sample_behavior/$4/g" "$4.h" "$4.cpp"
    $SED -i "s/sample_behavior/$4/g" "$4.h" "$4.cpp" Makefile.am

    if [ $3 == eyebot ]; then
      $SED -i "s/FootBot/EyeBot/g" "$4.h" "$4.cpp" "$1.h" "$1.cpp"
      $SED -i "s/Footbot/Eyebot/g" "$4.h" "$4.cpp" "$1.h" "$1.cpp"
      $SED -i "s/footbot/eyebot/g" "$4.h" "$4.cpp" "$1.h" "$1.cpp"
      $SED -i "s/FOOTBOT/EYEBOT/g" "$4.h" "$4.cpp" "$1.h" "$1.cpp"
    fi
    if [ $3 == handbot ]; then
      $SED -i "s/FootBot/HandBot/g" "$4.h" "$4.cpp" "$1.h" "$1.cpp"
      $SED -i "s/Footbot/Handbot/g" "$4.h" "$4.cpp" "$1.h" "$1.cpp"
      $SED -i "s/footbot/handbot/g" "$4.h" "$4.cpp" "$1.h" "$1.cpp"
      $SED -i "s/FOOTBOT/HANDBOT/g" "$4.h" "$4.cpp" "$1.h" "$1.cpp"
    fi

    cd ..
    # Fix the compilation environment
    echo "Fixing the compilation environment..."
    $AWK "/^SIMULATION =/ {print \$0,\" $1\"} ! /^SIMULATION =/ {print \$0}" Makefile.am | $SED "s/  / /g" > Makefile.am.new
    mv Makefile.am.new Makefile.am
  fi
  
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
# Ask user about his experience with the BT
#
check_user_experience ()
{                           
  read -ep "Do you want ARGoS to generate a behavior with example states and transitions? (y/n) [n]: " yn

  if [ x"$yn" == "xy" ]; then
	EXPERIENCED_USER="0"
  else
    EXPERIENCED_USER="1"
  fi	
}

###############################################################################
# Ask user about his experience with the BT
# PARAM $1: The controller tag name.
# PARAM $2: The root behavior tag name.
check_controller_behavior_filename ()
{                           

  if [ "$1" == "$2" ]; then
	  echo
	  echo "[ERROR] You cannot choose the same filename for a controller ($1) and"
	  echo "its root behavior ($2)! Please run the script again."
	  echo
	  exit 1
  fi	
}


###############################################################################
# Asks the user to type the wanted root behavior filename. This name is returned.
# RETURN: The behavior files name.
#
function get_behavior_source_name (  ) {
  local sourcename
  local behavior_name="new_root_behavior"

  read -ep "Insert you root behavior file name [$behavior_name]: " sourcename
  if [ x"$sourcename" = x ]; then
    sourcename="$behavior_name"
  fi

  echo $sourcename
}


###############################################################################
# Asks the user to type the wanted behavior class name. This name is returned.
# PARAM $1: The behavior file name.
# RETURN: The behavior class name.
#
function get_behavior_class_name (  ) {
  local classname
  #local current_robot=`capitalize_ichar $REQ_ROBOT_TYPE`
  local current_robot
  
  if [ $REQ_ROBOT_TYPE == footbot ]; then
    current_robot="FootBot"
  fi
  if [ $REQ_ROBOT_TYPE == handbot ]; then
    current_robot="HandBot"
  fi
  if [ $REQ_ROBOT_TYPE == eyebot ]; then
    current_robot="EyeBot"
  fi
  
  # Get rid of punctuations and capitalize the initial letters
  local tag=`convert_tag_to_classname $1`
  local behavior="$tag"
  
  local behavior_name="CBT${current_robot}${behavior}"

  read -ep "Insert your root behavior class name [$behavior_name]: " classname
  if [ x"$classname" = x ]; then
    classname="$behavior_name"
  fi

  echo $classname
}

###############################################################################
# Checks if the robot is supported by the behavioral toolkit
# PARAM $1: The desired robot type.
#

function check_supported_robot() {
  # check if it is an sbot
  if [ $1 == sbot ]; then
    echo "Sorry. The SBot is not supported by the Behavioral Toolkit (yet?)." 1>&2
    exit 1
  fi
  # check if it is an rbot
  if [ $1 == rbot ]; then
    echo "Sorry. The RBot is not supported by the Behavioral Toolkit (yet?)." 1>&2
    exit 1
  fi
  # check if it is an epuck
  if [ $1 == epuck ]; then
    echo "Sorry. The EPuck is not supported by the Behavioral Toolkit (yet?)." 1>&2
    exit 1
  fi
}

###############################################################################
# Displays the real robot info.
# PARAM $1: The controller tag name.
#
function create_experiment (  ) {
  read -ep "Do you want to create also an experiment XML associated with this BEHAVIOR controller [$1]? (y/n) [n]: " YN
  if [ x"$YN" == "xy" ]; then
  	
  	
  local experiment_xml="example_bt_experiment.xml"
  read -ep "Input the experiment XML name: [$experiment_xml]: " experiment_xml_input  	
  	
  if [ x"$experiment_xml_input" = x ]; then
    experiment_xml_input="$experiment_xml"
  fi  	
  	
  	
	while [ "${AHSSUSERNAME}" == "" ]; do
	read -ep "Please enter your username [${USERNAME}]: " AHSSUSERNAME
	
		# check if the user wants to use his unix username
		if [ "${AHSSUSERNAME}" == "" ]; then
		  AHSSUSERNAME=${USERNAME}
		fi
		
		# see if the requested user directory actually exists
		if [ ! -d ${AHSSINSTALLDIR}/user/${AHSSUSERNAME} ]; then
		  echo "This user does not exist!"
		      AHSSUSERNAME=
	    else
	      break
	    fi
	done  	
  	
  	
  local experiment_folder="${AHSSINSTALLDIR}/user/${AHSSUSERNAME}"
  read -ep "To which folder shall the experiment XML be added: [$experiment_folder]: " experiment_folder_input
  
  if [ x"$experiment_folder_input" = x ]; then
    experiment_folder_input="$experiment_folder"
  fi    

  	if  [ ! -d $experiment_folder_input ] ; then
  	  echo "[ERROR] $experiment_folder_input does not exist!"
  	  exit 1
  	  else
  	  	cp $AHSSINSTALLDIR/user/.template/example_behavioral_toolkit.xml $experiment_folder_input/$experiment_xml_input
  	  	$SED -i "s/bt_example_footbot_controller/$1/g" $experiment_folder_input/$experiment_xml_input
  	  	$SED -i "s/nithin/$AHSSUSERNAME/g" $experiment_folder_input/$experiment_xml_input
  	  	echo "File $experiment_folder_input/$experiment_xml_input was properly created. Use it to test your new behavioral toolkit controller."
  	fi
  fi
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
  check_supported_robot "$robottype"
  controller_filename=`get_controller_tag_name $robottype`
  controller_classname=`get_controller_class_name $controller_filename`
  rootbehavior_filename=`get_behavior_source_name`
  rootbehavior_classname=`get_behavior_class_name $rootbehavior_filename`
  check_controller_behavior_filename "$controller_filename" "$rootbehavior_filename"
  create_directory "$controller_filename"
  create_experiment "$controller_filename"
  check_user_experience
  populate_structure "$controller_filename" "$controller_classname" "$robottype" "$rootbehavior_filename" "$rootbehavior_classname"
  associate_controller "$controller_filename" "$robottype"
  real_robot_info
}

# Execution starts here
main