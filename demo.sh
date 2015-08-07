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

if [[ -n ${1} ]]; then
    case ${1} in
        "prompt")
            EXEC="arrowline"
            CMDS=('s/\\\[//g' 's/\\\]//g' 's/%\{//g' 's/%}//g')
            ;;
        "status")
            EXEC="arrowbar"
            CMDS=('s/%\{[RlcrA]\}//g' 's/%\{[BFTS].+\}//g' 's/%\{A\d?:.*:\}//g' 's/%\{[+\-\!][uo]\}//g')
            ;;
        *) ;;
    esac
fi

BASE_DIR="$( echo "$( cd "$( dirname ${BASH_SOURCE[0]} )" && pwd )" )"

echo -e "$( ${BASE_DIR}/${EXEC} | sed -re ${CMDS[0]} -e ${CMDS[1]} -e ${CMDS[2]} -e ${CMDS[3]} )"

