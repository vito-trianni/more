#!/bin/bash
#
# Sets the compulsory svn properties on the newly added files and svn:ignores on the unversioned files
#

USER_LIST="$AHSSINSTALLDIR/user/user_list.txt"

###############################################################################
# Displays usage help.
#
function display_help (  ) {
  echo "Usage: "`basename $0`" [svn username]"
  echo
  echo "This displays the maintainer responsibilities for a user for the whole"
  echo "ARGoS source tree. If you do not specify a svn username, the script"
  echo "will try to determine your username by using the 'user/my_user' link."
  echo
  echo "Please note that this script ignores the 'user' package, as usually"
  echo "all files in your subfolder should be your responsibility."
  exit 0
}


###############################################################################
# Displays the real robot info.
#
function display_error (  ) {
	echo 1>&2
  echo "[Error]: Your user directory is not properly configured!" 1>&2
  echo 1>&2
  echo "You should have a link named 'my_user' in ${AHSSINSTALLDIR}/user," 1>&2
  echo "which points to your own user directory. Without that, this script cannot figure" 1>&2
  echo "your identity!" 1>&2
  echo 1>&2
}


###############################################################################
# Gets the responsibilities for all versioned files
#
function get_responsibilities (  ) {

  MAINTAINER=0
  RESTRICTED=0

  # create a tmp file
  tmp_file=`mktemp`

  # get the argos user
  if [ "$1" == "" ]; then
    echo "No user specified on command line, using user/my_user link to determine your user."
    ARGOS_USER="$(basename $AHSSUSERDIR)"
  	SVN_USER=`grep ${ARGOS_USER} ${AHSSINSTALLDIR}/user/user_list.txt | cut -d , -f 2`
	  if [ "${SVN_USER}" == "" ]; then
  	  echo 1>&2
	    echo "[Error]: Could not extract your username from ${AHSSINSTALLDIR}/user/user_list.txt, check your user/my_user link." 1>&2
  	  echo 1>&2
	   	exit 1
  	fi
  else
		SVN_USER="$1"
    ARGOS_USER=`grep ${SVN_USER} ${AHSSINSTALLDIR}/user/user_list.txt | cut -d , -f 4`
    if [ "${ARGOS_USER}" == "" ]; then
      echo 1>&2
      echo "[Error]: Could not extract the ARGoS homedir from ${AHSSINSTALLDIR}/user/user_list.txt, are you sure this user exists?" 1>&2
      echo 1>&2
      exit 1
    fi
  fi

  echo "Checking for user '${SVN_USER}' (svn login) with ARGoS homedir '${ARGOS_USER}'."
  echo
  echo "M = you are maintainer"
  echo "R = you have restricted access"
  echo
  echo "Scanning for files ..."
  echo


  # get the list of all files
  cd $AHSSINSTALLDIR
  svn -R list . > $tmp_file

  # loop through all entries in the file
  while read filename; do 

    # first check if the file is really there and accessible
    svn info "$filename" &> /dev/null
    if [ $? -ne 0 ] || [ ! -e "$filename" ]; then
      continue
    fi

    # check if the filename is in the user package (if yes, ignore it)
    PATTERN="^user\/"
    if [[ "$filename" =~ ${PATTERN} ]]; then
      continue 
    fi

    # read the tags and grep for the users name 
    svn propget argos:maintainer "$filename" | grep "$SVN_USER" &> /dev/null
    MRET=$?
    svn propget argos:restricted "$filename" | grep "$SVN_USER" &> /dev/null
    RRET=$?
    if [ $MRET -eq 0 ] && [ $RRET -eq 0 ]; then
      echo "  MR  $filename"
      MAINTAINER=$(($MAINTAINER + 1))
      RESTRICTED=$(($RESTRICTED + 1))
    elif [ $MRET -eq 0 ]; then
      echo "  M   $filename"
      MAINTAINER=$(($MAINTAINER + 1))
    elif [ $RRET -eq 0 ]; then
      echo "   R  $filename"
      RESTRICTED=$(($RESTRICTED + 1))
    fi

  done < $tmp_file
 
  # display stats
  echo
  echo "Done."
  echo
  echo "You are maintainer for $MAINTAINER files/directories and"
  echo "you have restricted access to $RESTRICTED files/directories."

  # remove the tmp file
  rm -f $tmp_file
}


###############################################################################
# Main program
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
  
if [ $# -lt 0 ] || [ $# -ge 2 ] || [ "$1" == "--help" ] || [ "$1" == "-h" ]; then
  display_help
fi
  
check_userdir
get_responsibilities $1

exit 0
