#!/bin/sh
find . \( -name "*.cpp" -or -name "*.h" -or -name "configure.ac" -or -name "Makefile.am" \) -exec grep -n --color -H $* {} \;
