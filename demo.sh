#!/bin/bash
# ARROWLINE
# powerline-like shell prompt generator
#
# file: demo.sh
# v0.6.4 / 2015.07.28
#
# (c) 2015 Bernd Busse
# The MIT License (MIT)
#

EXEC=${1}

if [[ ! -x ${EXEC} ]]; then
    echo "ERROR: cannot find ${EXEC}"
    exit 1
fi

echo -e "$( ${EXEC} | sed -re 's/\\\[//g' -e 's/\\\]//g' -e 's/%\{//g' -e 's/%}//g' )"

