#! /bin/bash



if [ $# -lt 1 ]; then
	echo 'Syntax: ./render_video.sh <path_to_frame>'
	exit 1
fi

export POVINI="ini/high_quality.ini"

pwd
echo "povray $1 +d -F +P -visual DirectColor"

povray $1 +d -F +P -visual DirectColor
