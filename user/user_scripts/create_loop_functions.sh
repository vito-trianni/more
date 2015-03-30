#!/bin/bash
#
# Author: Arne Brutschy <arne.brutschy@ulb.ac.be>
#
# This script creates a new loop functions using the test_loop_functions 
# from the template user as template.
#
AWK=
SED=


###############################################################################
# Displays a welcome message.
#
function welcome_message (  ) {
  echo
  echo "+++++   [ CREATE NEW LOOP FUNCTIONS ]   +++++"
  echo
  echo "This script helps you creating new loop functions."
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
# Asks the user to type the wanted loop functions tag name until a name of a
# non-existing subdirectory is given. Then the name is returned.
# RETURN: The loop functions tag name.
#
function get_loop_functions_tag_name (  ) {
  local tagname
  local loop_functions_name="new_loop_functions"

  read -ep "Insert loop_functions XML tag name [$loop_functions_name]: " tagname
  if [ x"$tagname" = x ]; then
    tagname="$loop_functions_name"
  fi

  while ( test "x$tagname" = x || directory_exists "$tagname" ); do
    read -ep "Insert loop functions XML tag name: " tagname
  done
  echo $tagname
}

###############################################################################
# Asks the user to type the wanted loop functions class name until a name of a
# non-existing subdirectory is given. Then the name is returned.
# RETURN: The loop functions class name.
#
function get_loop_functions_class_name (  ) {
  local classname
  local loop_functions_name="CNewLoopFunction"

  read -ep "Insert loop_functions class name [$loop_functions_name]: " classname
  if [ x"$classname" = x ]; then
    classname="$loop_functions_name"
  fi

  echo $classname
}

###############################################################################
# Copies all the files in .template to the given subdirectory.
# PARAM $1: The loop functions tag name.
# PARAM $2: The loop functions class name.
#
function populate_structure (  ) {
  echo "Populating loop functions directory..."  
  if [ ! -e ../../.template/loop_functions/test_loop_functions ]; then
    echo "Error: The loop functions template directory ../../.template/loop_functions/test_loop_functions" 1>&2
    echo "does not exist. This should not happen. I'm sorry, but someone messed up." 1>&2
    exit 1
  fi

  echo "Using .template/loop_functions/test_loop_functions as template..."
  cp -rf ../../.template/loop_functions/test_loop_functions/* "$1"
  if [ ! $? -eq 0 ]; then
    echo "Unexpected error while creating files in directory $1" 1>&2
    echo "Check if you have the right permissions." 1>&2
    exit 1
   else
    local upcase
 
    # Remove the .svn files that came from the template dir
    find "$1" -name .svn | xargs rm -rf
    # Rename copied files
    echo "Renaming files..."
    cd "$1"
    mv test_loop_functions.h "$1.h"
    mv test_loop_functions.cpp "$1.cpp"
    # Retouch the files with the chosen names
    echo "Fixing the code..."
    upcase="`echo $2 | tr '[:lower:]' '[:upper:]'`"
    $SED -i "s/CTestLoopFunctions/$2/g" "$1.h" "$1.cpp"
    $SED -i "s/CTESTLOOPFUNCTIONS/$upcase/g" "$1.h"
    $SED -i "s/test_loop_functions/$1/g" "$1.h" "$1.cpp" Makefile.am
    cd ..
    # Fix the compilation environment
    echo "Fixing the compilation environment..."
    $AWK "/^SUBDIRS/ {print \$0,\" $1\"} ! /^SUBDIRS/ {print \$0}" Makefile.am > Makefile.am.new
    mv Makefile.am.new Makefile.am
    cd ..
    sed -i "/AC_CONFIG_FILES/{
N;N;s/\(AC_CONFIG_FILES(\[\\\\\n *loop_functions\/Makefile \\\\\\n\)/\1  loop_functions\/$1\/Makefile \\\\\\
/}" configure.ac
  fi
  echo "Success."
}

###############################################################################
# Displays final info.
#
function final_info (  ) {
  echo
  echo "Everything went fine."
  echo "You can now start editing your new loop functions in"
  echo "${AHSSUSERDIR}/loop_functions/$1"
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
  check_loop_functions_directory

  welcome_message
  check_requirements
  tagname=`get_loop_functions_tag_name`
  classname=`get_loop_functions_class_name`
  create_directory "$tagname"
  populate_structure "$tagname" "$classname"
  final_info "$tagname"
}

# Execution starts here
main
