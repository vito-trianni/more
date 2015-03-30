#!/bin/bash
#
# Authors: Carlo Pinciroli <cpinciro@ulb.ac.be>
#          Arne Brutschy <arne.brutschy@ulb.ac.be>
#
# This script creates a working environment in a subdirectory with the wanted
# name and populates it with the standard user files taken from .template.
# Then, the files are configured and everything gets compiled.


###############################################################################
# Displays a welcome message.
#
function welcome_message (  ) {
    echo
    echo "+++++   [ CREATE A NEW USER ]   +++++"
    echo
    echo "This script helps you creating your working directory to code "
    echo "controllers and run experiments with this framework."
    echo
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
# Asks the user to type the wanted directory name until a name of a
# non-existing subdirectory is given. Then the name is returned.
# RETURN: The directory name.
#
function get_directory_name (  ) {
    local dirname
    local username=`whoami`

    read -ep "Insert your directory name [$username]: " dirname
    if [ x"$dirname" = x ]; then
	      dirname="$username"
    fi

    while directory_exists "$dirname"
    do
        read -ep "Insert your directory name: " dirname
    done
    echo $dirname
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
	      echo "Check if you have the right permissions." 1>&2
	      exit 1
    fi
    echo "Success."
}

###############################################################################
# Copies all the files in .template to the given subdirectory.
# PARAM $1: The directory name.
#
function populate_structure (  ) {
    echo "Populating directory structure..."
    cp -a .template/* "$1"
    if [ ! $? -eq 0 ]; then
	      echo "Unexpected error while creating files in directory $1" 1>&2
	      echo "Check if you have the right permissions." 1>&2
	      exit 1
    else
        sed -i "s|THEUSER|$1|g" "$1/CMakeLists.txt" "$1"/README "$1"/xml/*.xml
    fi
    echo "Success."
}

###############################################################################
# Adds the newly created user directory to git
# PARAM $1: The directory name.
#
function git_add (  ) {
    echo
    read -ep "We recommend that you add your user directory to the IRIDIA git repository. Do this now (y/n) [y]: " yn
    if [ ! x"$yn" = "xn" ]; then
        echo "Adding directory: $1 to IRIDIA git repository."
        echo "You may need to provide the password you use to access the IRIDIA git repository."
        git add $1
        git commit $1 -m "New user directory"
        git pull
        echo
    fi
}

###############################################################################
# Prints some information to the user about the next steps to take.
#
function inform_user (  ) {
    echo
    echo "You now have your own user directory ($1)."
    echo "Here, you develop controllers, and compile and run the simulator executable."
    echo
    echo "Your user directory $1 contains several"
    echo "sample controllers that will be compiled for you by this script."
    echo
    echo "To run the simulator execute following commands (from your user directory):"
    echo
    echo "  $ export ARGOSINSTALLDIR=${ARGOSINSTALLDIR}"
    echo "  $ cd $1"
    echo "  $ ./argos -c xml/diffusion_1.xml"
    echo
    echo "NB. It is a good idea to set the ARGOSINSTALLDIR environment var in your"
    echo "    .bashrc file."
    echo
    read -p "Press enter to start building all packages: "
}


###############################################################################
# Main program
###############################################################################
function main (  ) {
        local dirname

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

        welcome_message
        dirname=`get_directory_name`
        create_directory "$dirname"
        create_user_link "$dirname"
        populate_structure "$dirname"
        git_add  "$dirname"
        inform_user "$dirname"
    }

# Execution starts here

    main

