#!/bin/bash
#
# Author: Eliseo Ferrante <eferrant@ulb.ac.be>
#         Nithin Mathews <nmathews@ulb.ac.be@ulb.ac.be>
#
# This script creates a new behavior using the demo_bt_behavior
# from the template user as template.
#
SED=


###############################################################################
# Displays a welcome message.
#
function welcome_message (  ) {
  echo
  echo "+++++   [ CREATE A NEW BEHAVIOR ]   +++++"
  echo
  echo "This script helps you creating a new behavior. Please select"
  echo "the robot type you want to create a new behavior for."
  echo
}

###############################################################################
# Checks that the needed programs are present.
#
function check_requirements (  ) {
  if ( which sed &> /dev/null ); then
    SED=`which sed`
  else
    echo "[FATAL] Unable to execute the script: sed has not been found."
    exit -1
  fi
}

###############################################################################
# Checks if we're in the users controller inside one controller with a
# non empty directory name.
#
function check_behavior_directory() {
  # check if we're in the right directory
  PATTERN="^.*controllers\/.+$"
  if [[ ! `pwd` =~ ${PATTERN} ]]; then
    echo "Error: You have to call this script from a BEHAVIOR controller directory." 1>&2
    exit 1
  fi
}

###############################################################################
# Checks if we're in the behavior controller directory.
#
function check_behavior_controller() {
  # check if we're in the right directory
  lines_count=$(grep -R "CCI_BehaviorController" . | wc -l)
  if [ $lines_count -le 0 ]; then
    echo "Error: this is not a directory where a CCI_BehaviorController resides." 1>&2
    exit 1
  fi
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
  # in case the robot is supported, check you are inside the right robot directory
  if [ $1 == footbot ]; then
    lines_count=$(grep -R "CCI_FootBotState" . | wc -l)
    if [ $lines_count -le 0 ]; then
      echo "Error: this is not a directory where a Behavioral Toolkit Foot-bot controller resides." 1>&2
      exit 1
    fi
  fi
  if [ $1 == eyebot ]; then
    lines_count=$(grep -R "CCI_EyeBotState" . | wc -l)
    if [ $lines_count -le 0 ]; then
      echo "Error: this is not a directory where a Behavioral Toolkit Eye-bot controller resides." 1>&2
      exit 1
    fi
  fi
  if [ $1 == handbot ]; then
    lines_count=$(grep -R "CCI_HandBotState" . | wc -l)
    if [ $lines_count -le 0 ]; then
      echo "Error: this is not a directory where a Behavioral Toolkit Hand-bot controller resides." 1>&2
      exit 1
    fi
  fi
}

###############################################################################
# Asks the user to type the wanted behavior soure files name until a name of a
# non-existing subdirectory is given. Then the name is returned.
# RETURN: The behavior source files name.
#
function get_behavior_source_name (  ) {
  local sourcename
  local behavior_name="new_behavior"

  read -ep "Insert source file name [$behavior_name]: " sourcename
  if [ x"$sourcename" = x ]; then
    sourcename="$behavior_name"
  fi

  echo $sourcename
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
# Asks the user to type the wanted behavior class name until a name of a
# non-existing subdirectory is given. Then the name is returned.
# RETURN: The behavior class name.
#
function get_behavior_class_name (  ) {
  local classname
  local current_robot=`capitalize_ichar $REQ_ROBOT_TYPE`
  
  # Get rid of punctuations and capitalize the initial letters
  local tag=`convert_tag_to_classname $1`
  local behavior="$tag"
    
  
  local behavior_name="CBT${current_robot}${behavior}"

  read -ep "Insert behavior class name [$behavior_name]: " classname
  if [ x"$classname" = x ]; then
    classname="$behavior_name"
  fi
  

  echo $classname
}

###############################################################################
# Copies all the files in .template to the given subdirectory.
# PARAM $1: The behavior source files name.
# PARAM $2: The behavior class name.
# PARAM $3: The desired robot type.
#
function populate_structure (  ) {
  local robottype=$3

  if [[ $EXPERIENCED_USER == "1" ]] 
  then	
  	path_to_template="$AHSSINSTALLDIR/user/.template/controllers/sample_bt_advanced_controller"
  else	
  	path_to_template="$AHSSINSTALLDIR/user/.template/controllers/sample_bt_controller"
  fi


  echo "Populating behavior directory..."
  if [ ! -e $path_to_template ]; then
    echo "Error: The behavior template $path_to_template" 1>&2
    echo "does not exist. This should not happen. I'm sorry, but someone messed up." 1>&2
    exit 1
  fi

  echo "Using $path_to_template as template..."
  cp $path_to_template/sample_behavior.h "$1"".h"
  cp $path_to_template/sample_behavior.cpp "$1"".cpp"
  if [ ! $? -eq 0 ]; then
    echo "Unexpected error while creating files (.h and .cpp) $1" 1>&2
    echo "Check if you have the right permissions." 1>&2
    exit 1
   else
    local upcase
 
    # Retouch the files with the chosen names
    echo "Fixing the code..."
    upcase="`echo $2 | tr '[:lower:]' '[:upper:]'`"
    $SED -i "s/CBTFootbotSampleBehavior/$2/g" "$1.h" "$1.cpp"
    $SED -i "s/CBTFOOTBOTSAMPLEBEHAVIOR/$upcase/g" "$1.h"
    $SED -i "s/sample_behavior/$1/g" "$1.h" "$1.cpp"
    $SED -i "s/demo_bt_behavior/$1/g" "$1.cpp"
    
    if [ $3 == eyebot ]; then
      $SED -i "s/FootBot/EyeBot/g" "$1.h" "$1.cpp"
      $SED -i "s/Footbot/Eyebot/g" "$1.h" "$1.cpp"
      $SED -i "s/footbot/eyebot/g" "$1.h" "$1.cpp"
      $SED -i "s/FOOTBOT/EYEBOT/g" "$1.h" "$1.cpp"
    fi
    if [ $3 == handbot ]; then
      $SED -i "s/FootBot/HandBot/g" "$1.h" "$1.cpp"
      $SED -i "s/Footbot/Handbot/g" "$1.h" "$1.cpp"
      $SED -i "s/footbot/handbot/g" "$1.h" "$1.cpp"
      $SED -i "s/FOOTBOT/HANDBOT/g" "$1.h" "$1.cpp"
    fi
    # Fix the compilation environment
    echo "Fixing the compilation environment..."
    $SED -i "s/_SOURCES = /_SOURCES = $1.h $1.cpp /g" Makefile.am

  fi
  echo "Success."
}




###############################################################################
# Main program
###############################################################################
function main (  ) {
  local sourcename
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
  check_behavior_directory
  check_behavior_controller

  welcome_message
  check_requirements
  get_robot_type 0
  robottype=${REQ_ROBOT_TYPE}
  check_supported_robot "$robottype"
  sourcename=`get_behavior_source_name`
  classname=`get_behavior_class_name $sourcename`
  check_user_experience
  populate_structure "$sourcename" "$classname" "$robottype"

}

# Execution starts here
main
