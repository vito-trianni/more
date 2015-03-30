#!/bin/sh

set +x

# INPUT FILE FORMAT
# path user1 user2 ... userN

MAINTAINER_FILE=`mktemp -t maintainers.XXXXXX`

# loop for each record of the file/users list
while read RECORD; do

    # get file name
    FILE=`echo $RECORD | cut -d ' ' -f 1`

    # prepare the list pf maintainers
    for USER in `echo $RECORD | cut -d ' ' -f 2-`; do
        grep -h $USER $AHSSINSTALLDIR/user/user_list.txt | cut -d ',' -f 1-3 >> $MAINTAINER_FILE        
    done

    # set access model
    echo "svn propset \"argos:access-model\" \"argos:maintainer\" $FILE"
    svn propset "argos:access-model" "argos:maintainer" $FILE

    # set the list of maintainers
    echo "svn propset \"argos:maintainer\" -F $MAINTAINER_FILE $FILE"
    cat $MAINTAINER_FILE
    svn propset "argos:maintainer" -F $MAINTAINER_FILE $FILE

    rm -f $MAINTAINER_FILE

    echo

done
