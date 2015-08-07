/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: config.h
 * v0.6.4 / 2015.07.28
 *
 * (c) 2015 Bernd Busse
 * The MIT License (MIT)
 **/

#ifndef _CONFIG_H
#define _CONFIG_H

/**
 * CONSTANTS: DO NOT CHANGE!
 **/

/* output formats (set below) */
#define OUTPUT_PLAIN 1
#define OUTPUT_BASH 2
#define OUTPUT_ZSH 4

#define OUTPUT_LEMONBAR 8
#define OUTPUT_DZEN2 16

/* buffer length */
#define BUF_FORMAT_LEN 48
#define BUF_PROMPT_LEN 128

/* number of parent dirs to show */
#define CWD_LEN 3

#endif // _CONFIG_H
