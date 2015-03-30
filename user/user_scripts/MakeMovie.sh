#!/bin/bash
#
# Create video from ahss xml file and directory name
#

function display_usage ( ) {
  echo "$0 <xml file> <frame directory> <width> <height> <fps> <outputfilename>"
  echo
  echo "This script creates a video starting from an ahss xml file and frame directory"
  echo

  exit -1
}

# Check command line
if [ $# != 6 ]; then
    display_usage
fi

# Action :) :
export TMPVID=$DISPLAY
export DISPLAY=:0
ahss -c $1 &> temp.txt
convert_all_png_to.sh $2 jpg
rm $2/*.png
mencoder mf://$2/*.jpg -mf w=$3:h=$4:fps=$5 -o $6 -ovc lavc -lavcopts vcodec=msmpeg4v2:vbitrate=3000
rm $2/*.jpg
rm -f temp.txt
export DISPLAY=$TMPVID
