#!/bin/bash
#
# Sets the compulsory svn properties on the newly added files and svn:ignores on the unversioned files
#

USER_LIST="$AHSSINSTALLDIR/user/user_list.txt"

###############################################################################
# Displays usage help.
#
function display_help (  ) {
  echo "Usage: "`basename $0`
  echo
  echo "This script sets the compulsory svn properties 'argos:access-model' and"
  echo "'argos:maintainer' on all newly added files and directories found in the"
  echo "\$AHSSINSTALLDIR working copy."
  echo
  echo "At the same time, it sets the property 'svn:ignore' on all new directiories."
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
# Get user related information
#
function get_user_info (  ) {
  ARGOS_USER="$(basename $AHSSUSERDIR)"
  ARGOS_USER_INFO="$(cat $USER_LIST | grep $ARGOS_USER)"

  # save $IFS 
  ORIGIFS=$IFS
  IFS=","

  #only the first three field from the user list file
  field=( $ARGOS_USER_INFO )
  ARGOS_USER_INFO_FILTERED="${field[0]},${field[1]},${field[2]}"
  
  # set $IFS back
  IFS=$ORIGIFS
  
  read -ep "Is '${field[0]}' your correct name and '${field[2]}' your valid email id? (y/n) " yn

  if [ x"$yn" != "xy" ]; then
    display_error
    exit 1;
  fi
}


###############################################################################
# Set the properties for the added and unversioned files
#
function set_argos_svn_properties (  ) {

  # create a tmp file
  tmp_file=`mktemp`

  svn status $AHSSINSTALLDIR > $tmp_file

  # loop through all entries in the file
  while read line; do 

    # get the status (? or A) of the file and the filename
    stat="$(echo "$line" | awk '{print $1}')"
    filename="$(echo "$line" | awk '{print $NF}')"

    # added file: set missing properties
    if [[ $stat == "A" ]]; then 
      # set both compulsory properties for access model
      #echo " * Setting properties 'argos:access-model' and 'argos:maintainer' on '$filename'."
      svn propset argos:access-model 'argos:maintainer' "$filename"
      svn propset argos:maintainer "$ARGOS_USER_INFO_FILTERED" "$filename"

      # check if it's a directory and add the svn:ignore tag if it has
      # not been added by the user yet
      IGNORE=`svn propget svn:ignore "$filename"`
      if [ -d "$filename" ] && [ "$IGNORE" == "" ]; then
        #echo " * Setting property 'svn:ignore' on '$filename'."
        svn propset svn:ignore -F $AHSSINSTALLDIR/extras/svnignores_default "$filename"
      fi
    fi

  done < $tmp_file

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
  
if [ $# != 0 ]; then
  display_help
fi
  
check_userdir
get_user_info
set_argos_svn_properties  

exit 0
