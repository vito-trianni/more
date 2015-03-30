#!/bin/bash


if [ "$1" == "" ]; then
    echo "Usage: set_user.sh <userdir>"
    exit 1
fi

DIR=`echo $1 | sed 's/\/$//'`
if [ ! -d $DIR ]; then
    echo "Error: User not found!"
    exit 1
fi

echo "Changing user to $DIR..."
rm -f my_user
ln -s $DIR my_user

exit 0
