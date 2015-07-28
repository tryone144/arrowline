/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: config.h
 * v0.6.3 / 2015.07.16
 *
 * (c) 2015 Bernd Busse
 * The MIT License (MIT)
 **/

#ifndef _CONFIG_H
#define _CONFIG_H

/* CONSTANTS DONT CHANGE! */
#define OUTPUT_PLAIN 1
#define OUTPUT_BASH 2
#define OUTPUT_ZSH 4

#define SEGMENT_STATUS 1
#define SEGMENT_HOST 2
#define SEGMENT_CWD_PREFIX 8
#define SEGMENT_CWD 4
#define SEGMENT_VCS 32

#define BUF_FORMAT_LEN 48
#define BUF_PROMPT_LEN 128

/**
 * CONFIGURATION HEADER
 **/

/* number of directories to show in hirarchy */
#define CWD_LEN 3

/* color codes for the segments */
#define COLOR_FG_HOST 16        // black
#define COLOR_BG_HOST_USER 47   // green
#define COLOR_BG_HOST_ROOT 196  // red
#define COLOR_BG_HOST_SSH 226   // yellow

#define COLOR_FG_STATUS 196     // red
#define COLOR_BG_STATUS 0       // black

#define COLOR_FG_CWD_PREFIX 250 // light-grey
#define COLOR_BG_CWD_PREFIX 238 // dark-light-grey

#define COLOR_FG_CWD 250        // light-grey
#define COLOR_BG_CWD 240        // grey
#define COLOR_SEP_CWD 245       // light-grey

#define COLOR_FG_VCS 236        // dark-grey
#define COLOR_BG_VCS_CLEAN 82   // dull-green
#define COLOR_BG_VCS_DIRTY 208  // orange

/* output styles */
#define USE_POWERLINE_SYMBOLS
#define OUTPUT_FORMAT OUTPUT_BASH

/* segments to use */
static const int NUM_SEGMENTS = 5;
static const unsigned int USE_SEGMENTS[] = {
    SEGMENT_STATUS,
    SEGMENT_HOST,
    SEGMENT_CWD_PREFIX,
    SEGMENT_CWD,
    SEGMENT_VCS
};

#endif // _CONFIG_H
