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

# source the user's global build config
if [ -f ${ARGOSUSERDIR}/build.conf ]; then
    source ${ARGOSUSERDIR}/build.conf
fi

CMAKE_BUILD_TYPE="Debug"
while [ $# -gt 0 ]; do
    OPTION="$1"
    shift

    # clean the build dir if requested
    if [ "${OPTION}" == "clean" ]; then
        rm -rf ${ARGOSINSTALLDIR}/build/common/simulator/*
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

# create build directory
mkdir -p ${ARGOSINSTALLDIR}/build/common/simulator; cd ${ARGOSINSTALLDIR}/build/common/simulator || { echo "Error: directory ${ARGOSINSTALLDIR}/build/common/simulator does not exists, and the attemp to create it failed: permissions trouble?" 1>&2; exit 1; }
# configure package for crosscompilation
echo "cmake ../../../common -DCMAKE_INSTALL_PREFIX=/usr -G 'Eclipse CDT4 - Unix Makefiles' -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} || { echo 'cmake failed' 1>&2; exit 1; }" | sh

# compile
make ${GLOBAL_MAKE_ARGS} || { echo "make failed" 1>&2; exit 1; }

# display final message about recompiling
display_make_message

exit 0
