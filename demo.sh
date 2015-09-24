#!/bin/bash
# ARROWLINE
# powerline-like shell prompt generator
#
# file: demo.sh
# v0.7 / 2015.08.07
#
# (c) 2015 Bernd Busse
# The MIT License (MIT)
#

EXEC="arrowline"
CMDS=('s/\\\[//g' 's/\\\]//g' 's/%\{//g' 's/%}//g')

BASE_DIR="$( echo "$( cd "$( dirname ${BASH_SOURCE[0]} )" && pwd )" )"

echo -e "$( ${BASE_DIR}/${EXEC} ${@} | sed -re ${CMDS[0]} -e ${CMDS[1]} -e ${CMDS[2]} -e ${CMDS[3]} )"

