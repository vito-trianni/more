#!/bin/sh

TMPFILE=`mktemp -t fix_user_list.XXXXXX`

grep '^user' | while read record; do

    FILE=`echo $record | cut -d ' ' -f 1`
    HOMEDIR=`echo $record | cut -d '/' -f 2`
    
    # format: 'user in repository' 'home in user dir'
    cat ${AHSSINSTALLDIR}/user/user_list.txt | grep $HOMEDIR | cut -d ' ' -f 1 > $TMPFILE

    echo "$FILE `cat $TMPFILE`"

done

rm -f $TMPFILE