#!/bin/sh
#
# This script is used to copy binaries to a sbot without fucking up the cam.
#
# Authors: Anders Lhyne Christensen (???)
#          Arne Brutschy <arne.brutschy@ulb.ac.be>
#

# time to sleep between each block
SLEEP_TIME_SECS=0.25
# block size to split binary into
SPLIT_SIZE="-b40k"

###############################################################################
# Displays a progress counter.
# Counts the lines going through stdin.
# Needs the total number of lines as argument.
# Ripped from ${AHSSINSTALLDIR}/user/user_scripts/ahss_script_functions
# PARAM1: total number of lines
#
function progress_bar() {
  NUM=$1

  # check that we actually have something to count
  if [ ${NUM} == 0 ]; then
    while read INPUT; do
      NOP=
    done
    echo -n "0%....25%....50%....75%....100%"
    return
  fi

  # init vars  and start counter
  STEPS_CURR=0
  STEPS_5PERC=`expr ${NUM} \* 5`
  echo -n "0%"

  # read every line
  while read INPUT; do
    # increase the steps we're at now by 100
    STEPS_CURR=`expr ${STEPS_CURR} + 100`

    # calculate how many dots we have to go to
    DOTS_CURR=`expr ${STEPS_CURR} / ${STEPS_5PERC}`

    # output the required number of dots
    for DOTS in `seq ${DOTS_DONE} ${DOTS_CURR}`; do
      case ${DOTS} in
        20) echo -n "100%";;
        15) echo -n  "75%";;
        10) echo -n  "50%";;
         5) echo -n  "25%";;
         *) if [ ${DOTS} -le 20 ]; then
              echo -n "."
            fi
      esac
    done
    DOTS_DONE=`expr ${DOTS_CURR} + 1`
  done
}

###############################################################################
# Transfers chunks to the specified sbot.
#
function transfer_chunks() {
  for FILE in ${SBOT}_splitbinary*; do
    echo "copying segment ${FILE} to ${SBOT}"
    scp $FILE root@${SBOT}:/tmp/ > /dev/null
    sleep ${SLEEP_TIME_SECS}
    echo
  done
}


###############################################################################
# Main starts here.
###############################################################################

# check if we got the right number of parameters
if [ "$#" != "2" ]; then
   echo "Usage: `basename $0` <binary_name> <sbot_dns_name>"
   exit
fi
BINPATH=`dirname ${1}`
BINARY=`basename ${1}`
SBOT=${2}
echo "Copying file ${BINARY} to ${SBOT}..."

# check if we can read the binary
if [ ! -e ${BINPATH}/${BINARY} ]; then
   echo "Error: can't find binary ${BINPATH}/${BINARY}"
   echo "Usage: copy_binary_to_sbot.sh <binary_name> <sbot_num> [<sbot_num>*]"
   exit
fi

# split the file and read the number of files
echo " * splitting binary"
split ${SPLIT_SIZE} ${BINPATH}/${BINARY} ${SBOT}_splitbinary
NUM=`ls -l ${SBOT}_splitbinary* | wc -l`

# transfer each chunk to the specified sbot
echo -n " * transferring chunks "
transfer_chunks | progress_bar ${NUM}
echo

# rejoin the chunks on the sbot
echo " * rejoining chunks"
ssh root@${SBOT} "cat /tmp/${SBOT}_splitbinary* > /tmp/${BINARY}"
ssh root@${SBOT} "chmod u+x /tmp/${BINARY}"

# clean up chunks
echo " * cleaning up"
ssh root@${SBOT} "rm -f /tmp/${SBOT}_splitbinary*"
rm -f ${SBOT}_splitbinary*

# check
SIZE=`ls -l ${BINPATH}/${BINARY} | awk '{print $5}'`
REMOTE_SIZE=`ssh root@${SBOT} ls -l /tmp/${BINARY} | awk '{print $5}'`
if [ ${SIZE} -eq ${REMOTE_SIZE} ]; then
  echo "Success."
else
  echo "Failure. Local: ${SIZE} Remote: ${REMOTE_SIZE}"
  exit 1
fi

exit 0
