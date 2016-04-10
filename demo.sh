#!/bin/bash
# ARROWLINE
# powerline-like shell prompt generator
#
# file: demo.sh
# v0.7.5 / 2016.04.10
#
# (c) 2016 Bernd Busse
# The MIT License (MIT)
#

EXEC="arrowline"
CMDS=('s/\\\[//g' 's/\\\]//g' 's/%\{//g' 's/%}//g')

BASE_DIR="$( echo "$( cd "$( dirname ${BASH_SOURCE[0]} )" && pwd )" )"

echo -e "$( ${BASE_DIR}/${EXEC} ${@} | sed -re ${CMDS[0]} -e ${CMDS[1]} -e ${CMDS[2]} -e ${CMDS[3]} )"

