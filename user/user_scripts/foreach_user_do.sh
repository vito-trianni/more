#!/bin/bash

if [ "$1" == "" ]; then
  echo "Error: you have to specify a command to execute!" 1>&2
  exit 1
fi

USERS=`ls | grep -v -e "my_user" -e "user_scripts" -e "create_user.sh"` 
USERS="$USERS .template"

for USER in $USERS; do
  echo "Doing user $USER..."
  cd $USER
  $*
  cd ..
done

