#!/bin/bash

# check if we got a basedir
export ARGOSINSTALLDIR=`pwd`
if [ ! -e ${ARGOSINSTALLDIR}/user/user_scripts/argos_script_functions ]; then
    echo "Error: cannot find argos script functions. Check the ARGOSINSTALLDIR environment variable!" 1>&2
    exit 1
fi


# Compile the Shark library
( cd "${ARGOSINSTALLDIR}/Shark"; cmake .; make )



# source the common sc=`pwd`ript functions
source ${ARGOSINSTALLDIR}/user/user_scripts/argos_script_functions

# do some basic checks
default_checks

# check if the user is known
check_create_user

# check userdir again (probably somebody messed up)
check_userdir
if [ $? == 0 ]; then
    echo "Error: Your username should be known by now." 1>&2
    echo "Something went wrong with the 'create_user.sh' script." 1>&2
    exit 1
fi
echo

BUILDTYPE="debug"
while [ $# -gt 0 ]; do
    OPTION="$1"
    shift

    # clean the build dirs if requested
    if [ "${OPTION}" == "clean" ]; then
        CLEAN="clean"
    fi

    # silent build if requested
    if [ "${OPTION}" == "quiet" ]; then
        QUIET="quiet"
    fi

    # build type
    if [ "${OPTION}" == "debug" ]; then
        BUILDTYPE="debug"
    fi
    if [ "${OPTION}" == "release" ]; then
        BUILDTYPE="release"
    fi
    if [ "${OPTION}" == "relwithdebinfo" ]; then
        BUILDTYPE="relwithdebinfo"
    fi
done

# create the base build directory
mkdir -p ${ARGOSINSTALLDIR}/build

# build the common package
echo "+++++   [ BUILDING THE COMMON PACKAGE FOR THE SIMULATOR ]   +++++"
if [ ${QUIET} ]; then
    { cd "${ARGOSINSTALLDIR}/common" && ./build_simulator.sh ${CLEAN} ${BUILDTYPE} > /dev/null; } || { cd "${ARGOSINSTALLDIR}"; echo "An error occurred during compilation." 1>&2; exit 1; }
else
    { cd "${ARGOSINSTALLDIR}/common" && ./build_simulator.sh ${CLEAN} ${BUILDTYPE}; } || { cd "${ARGOSINSTALLDIR}"; echo "An error occurred during compilation." 1>&2; exit 1; }
fi
echo
echo

# build the real_robot package
echo "+++++   [ BUILDING THE SIMULATOR PACKAGE FOR THE SIMULATOR ]   +++++"
if [ ${QUIET} ]; then
    { cd "${ARGOSINSTALLDIR}/simulator" && ./build_simulator.sh ${CLEAN} ${BUILDTYPE} > /dev/null; } || { cd "${ARGOSINSTALLDIR}"; echo "An error occurred during compilation." 1>&2; exit 1; }
else
    { cd "${ARGOSINSTALLDIR}/simulator" && ./build_simulator.sh ${CLEAN} ${BUILDTYPE}; } || { cd "${ARGOSINSTALLDIR}"; echo "An error occurred during compilation." 1>&2; exit 1; }
fi
echo
echo

# build the user package
echo "+++++   [ BUILDING THE USER PACKAGE FOR THE SIMULATOR ]   +++++"
if [ ${QUIET} ]; then
    { cd "${ARGOSUSERDIR}" && ./build_simulator.sh ${CLEAN} ${BUILDTYPE} > /dev/null; } || { cd "${ARGOSINSTALLDIR}"; echo "An error occurred during compilation." 1>&2; exit 1; }
else
    { cd "${ARGOSUSERDIR}" && ./build_simulator.sh ${CLEAN} ${BUILDTYPE}; } || { cd "${ARGOSINSTALLDIR}"; echo "An error occurred during compilation." 1>&2; exit 1; }
fi
echo
echo

echo "Done. Normelly it werks."
echo
echo "You can find your compiled controllers in the subdirectories of:"
echo
echo "  build/user/${ARGOSUSERNAME}/controllers/"
echo
echo

exit 0
