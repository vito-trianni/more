#!/bin/bash
 
echo "Attempting to resurrect robot ..."
echo
echo "(did you press the translator reset"
echo " button before executing this script?)"
echo

/etc/init.d/dspics-power.sh restart && \
/etc/init.d/dbus-1 restart && \
/etc/init.d/asebamedulla.sh restart && \
sleep 2 && \
/etc/init.d/ARGoS.sh restart

exit $?
