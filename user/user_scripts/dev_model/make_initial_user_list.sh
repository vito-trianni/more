#!/bin/sh

svn list -R ${AHSSINSTALLDIR} | while read file
do
    echo -n "$file "
    svn log $file -q | tail -n 2 | head -n 1 | awk '{ print $3 }'
done