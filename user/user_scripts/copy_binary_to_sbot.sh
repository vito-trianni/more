#!/bin/sh


if [ $# -eq 0 ]
then
   echo "Usage: copy_binary_to_sbot.sh <binary_name> <sbot_num> [<sbot_num>*]"
   exit 1
fi

BINARY=$1
shift

if [ ! -e ${BINARY} ]
then
   echo "Error: can't find binary ${BINARY}"
   echo "Usage: copy_binary_to_sbot.sh <binary_name> <sbot_num> [<sbot_num>*]"
   exit 1
fi

for SBOT in $*
do
   ./split_copy_binary_to_sbot.sh ${BINARY} ${SBOT} &
done
