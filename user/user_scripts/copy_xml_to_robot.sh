#!/bin/bash


if [ $# -ne 2 ]
then
   echo "[ERROR] Usage: copy_xml_to_robot.sh <path_to_xml> <robot>"
   exit 1
fi


XML=$1
ROBOT=$2


# some other needed variables
XmlTagRandomSeed="random_seed"
xml_tmp1="/tmp/xml_tmp1.xml"


#Create random seed
random_number=$((RANDOM))
while [ $random_number -eq 0 ]; do
        random_number=$((RANDOM))
done


# Check whether the xml exists
if [ -f $XML ]
	then
		echo "[INFO] Found XML" ;
	else
		echo "[ERROR] Could not find $XML"
		exit 1
fi


echo "[INFO] Changing random seed to $random_number"
		
# Extracting the old random seed value
OldRandomSeed=`grep "<$XmlTagRandomSeed>.*<.$XmlTagRandomSeed>" $XML | sed -e "s/^.*<$XmlTagRandomSeed/<$XmlTagRandomSeed/" | cut -f2 -d">"| cut -f1 -d"<"`
# Replacing the random seed value 
sed -e "s/<$XmlTagRandomSeed>$OldRandomSeed<\/$XmlTagRandomSeed>/<$XmlTagRandomSeed>$random_number<\/$XmlTagRandomSeed>/g" $XML > $xml_tmp1





scp -q $xml_tmp1 $ROBOT:/tmp
if test $? -eq 0; then
echo "[INFO] Copied XML to $ROBOT"
else
echo "[ERROR] Something went wrong while copying! Sorry.."
fi 


# Cleaning up
rm $xml_tmp1