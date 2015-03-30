#!/bin/bash
#
# Author: Carlo Pinciroli
#
# This script converts all the png files in a directory to the wanted format.
# It internally uses ImageMagick's convert utility.

# Displays the help and exits.
# PARAM $1: The exit code.
function display_usage (  ) {
    echo "$0 <directory> <format>"
    echo
    echo "This script converts all the png files in a directory to the wanted format."
    echo "The wanted format is identified by the graphics image file extension you want"
    echo "to create. For example:"
    echo
    echo "$ $0 frames jpg"
    echo
    echo "converts all the png files in directory ./frames to jpg format."

    exit $1
}



### Execution starts here ###

# Check tools presence
CONVERT=`which convert &> /dev/null`
if [ ! $? -eq 0 ]; then
    echo "[FATAL] Required command convert not found. Please install ImageImagick package."
    exit -1
fi

# Check command line
if [ $# != 2 ]; then
    display_usage -1
fi

# Check directory
if [ ! -d "$1" ]; then
    echo "[FATAL] Directory '$1' not found."
    exit -1;
fi

if [ ! -w "$1" ]; then
    echo "[FATAL] Directory '$1' is not writable."
    exit -1;
fi

#ls "$1"/*.png &> /dev/null
#if [ ! $? -eq 0 ]; then
#    echo "[FATAL] Unable to read png files in directory '$1'."
#    exit -1;
#fi

# Execute conversion
for f in "$1"/*.png; do
    f2="$1/"`basename "$f" png`$2;
    echo "$f -> $f2";
    convert -quality 100 "$f" "$f2";
done
