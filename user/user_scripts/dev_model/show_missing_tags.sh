#!/bin/bash
#
# Sets the compulsory svn properties on the newly added files and svn:ignores on the unversioned files
#

USER_LIST="$AHSSINSTALLDIR/user/user_list.txt"


###############################################################################
# Gets the responsibilities for all versioned files
#
function show_missing_tags (  ) {

  # create a tmp file
  tmp_file=`mktemp`

  # get the list of all files
  cd $AHSSINSTALLDIR
  svn -R list . > $tmp_file

  # loop through all entries in the file
  while read filename; do 

    USER=""

    # first check if the file is really there and accessible
    svn info "$filename" &> /dev/null
    if [ $? -ne 0 ] || [ ! -e "$filename" ]; then
#      echo "$filename	seems to be inaccessible!"
      continue
    fi

    # check if the filename is in the user package (if yes, ignore it)
    PATTERN="^user\/"
    if [[ "$filename" =~ ${PATTERN} ]]; then
#      USER=`echo $filename | sed "s/^user\/\([a-zA-Z0-9]*\)\/.*$/\1/"`
      continue
    fi

    # read the tags and grep for the users name 
    svn propget argos:maintainer "$filename" | grep "$SVN_USER" &> /dev/null
    MRET=$?
#    if [ $MRET -ne 0 ]; then
#      echo "$filename	missing 'argos:maintainer' tag!"
#    fi
    svn propget argos:restricted "$filename" | grep "$SVN_USER" &> /dev/null
    RRET=$?
#    if [ $RRET -ne 0 ]; then
#      echo "$filename	missing 'argos:restricted' tag!"
#    fi
    svn propget argos:access-model "$filename" | grep "$SVN_USER" &> /dev/null
    ARET=$?
#    if [ $ARET -ne 0 ]; then
#      echo "$filename	missing 'argos:access-model' tag!"
#    fi
    if [ $ARET -ne 0 ] && [ $RRET -ne 0 ] && [ $MRET -ne 0 ]; then
      echo "$filename $USER"
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
  
show_missing_tags

exit 0
