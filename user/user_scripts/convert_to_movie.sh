#!/bin/bash
#
# Create video from ahss xml file and directory name
#

function display_usage ( ) {
  echo "$0 <frame directory> <filename> <format> <width> <height> <fps> <codec> <outputfilename> <extra>"
  echo
  echo "This script creates a video starting from a frame directory"
  echo

  exit -1
}

# Check command line
if [ $# -lt 8 ]; then
    display_usage
fi

# Action :) :
export TMPVID=$DISPLAY
export DISPLAY=:0
mencoder mf://$1/$2*.$3 -mf w=$4:h=$5:fps=$6 -ovc $7 -o $8 $9
export DISPLAY=$TMPVID
