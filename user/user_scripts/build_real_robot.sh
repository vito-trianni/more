#!/bin/bash

# check if we got a basedir
if [ "${ARGOSINSTALLDIR}" == "" ]; then
  echo "Error: You need to set the ARGOSINSTALLDIR environment variable!" 1>&2
  exit 1
fi
if [ ! -e ${ARGOSINSTALLDIR}/user/user_scripts/argos_script_functions ]; then
  echo "Error: Your ARGOSINSTALLDIR environment variable is not set properly!" 1>&2
  exit 1
fi

# source the common script functions
source ${ARGOSINSTALLDIR}/user/user_scripts/argos_script_functions

# do some basic checks
default_checks
default_user_checks

# check if the user was smart enough to tell us which robot he actually wants to build
check_robot_type $1 1
if [[ "$1" == "" || $? == 0 ]]; then
  echo "Usage: `basename $0` <robot_type>"
  display_robot_types 1
  exit 1
fi
ROBOT_TYPE=$1
robot_var ${ROBOT_TYPE}
ROBOT_VAR=${REQ_ROBOT_VAR}
shift

# source the user's global build config
#source ${ARGOSUSERDIR}/build.conf

CMAKE_BUILD_TYPE="Debug"
while [ $# -gt 0 ]; do
    OPTION="$1"
    shift

    # clean the build dir if requested
    if [ "${OPTION}" == "clean" ]; then
        rm -rf ${ARGOSINSTALLDIR}/build/user/${ARGOSUSERNAME}/${ROBOT_TYPE}/*
    fi
    # build type
    if [ "${OPTION}" == "debug" ]; then
        CMAKE_BUILD_TYPE="Debug"
    fi
    if [ "${OPTION}" == "release" ]; then
        CMAKE_BUILD_TYPE="Release"
    fi
    if [ "${OPTION}" == "relwithdebinfo" ]; then
        CMAKE_BUILD_TYPE="RelWithDebInfo"
    fi
done

# setup build environment by calling the appropriate script
source ${ARGOSINSTALLDIR}/real_robot/${ROBOT_TYPE}/setup_environment.sh
export REAL_ROBOT_GLOBAL_CMAKE_ARGS="-DARGOS_COMPILE_REAL_${ROBOT_VAR}=1 -Wno-dev"

# create build directory
mkdir -p ${ARGOSINSTALLDIR}/build/user/${ARGOSUSERNAME}/${ROBOT_TYPE}; cd ${ARGOSINSTALLDIR}/build/user/${ARGOSUSERNAME}/${ROBOT_TYPE} || { echo "Error: directory ${ARGOSINSTALLDIR}/build/${ARGOSUSERDIR}/${ROBOT_TYPE} does not exists, and the attemp to create it failed: permissions trouble?" 1>&2; exit 1; }
# configure package for crosscompilation
cmake -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} ${REAL_ROBOT_GLOBAL_CMAKE_ARGS} ${ARGOSUSERDIR} || { echo "cmake failed" 1>&2; exit 1; }

# compile
make ${GLOBAL_MAKE_ARGS} || { echo "make failed" 1>&2; exit 1; }

# display final message about recompiling
display_make_message

exit 0
