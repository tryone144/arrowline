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

/* segment identifiers (set below) */
#define SEGMENT_STATUS 1
#define SEGMENT_HOST 2
#define SEGMENT_CWD_PREFIX 8
#define SEGMENT_CWD 4
#define SEGMENT_VCS 32

/* buffer length */
#define BUF_FORMAT_LEN 48
#define BUF_PROMPT_LEN 128


/**
 * SEGMENT COLOR CODES
 **/

/* hostname / username */
#define COLOR_FG_HOST_USER 16   // black
#define COLOR_BG_HOST_USER 47   // green

#define COLOR_FG_HOST_ROOT 16   // black
#define COLOR_BG_HOST_ROOT 196  // red

#define COLOR_FG_HOST_SSH 16    // black
#define COLOR_BG_HOST_SSH 226   // yellow

/* returncode status */
#define COLOR_FG_STATUS 196     // red
#define COLOR_BG_STATUS 0       // black

/* current working directory */
#define COLOR_FG_CWD_PREFIX 250 // light-grey
#define COLOR_BG_CWD_PREFIX 238 // dark-light-grey

#define COLOR_FG_CWD 250        // light-grey
#define COLOR_BG_CWD 240        // grey
#define COLOR_SEP_CWD 245       // light-grey

/* version control system */
#define COLOR_FG_VCS_CLEAN 236  // dark-grey
#define COLOR_BG_VCS_CLEAN 82   // dull-green

#define COLOR_FG_VCS_DIRTY 236  // dark-grey
#define COLOR_BG_VCS_DIRTY 208  // orange


/**
 * APPEARANCE CONFIGURATION
 **/

/* number of directories to show in hirarchy */
#define CWD_LEN 3

/* output styles */
#define USE_POWERLINE_SYMBOLS
#define PROMPT_OUTPUT_FORMAT OUTPUT_BASH
#define STATUS_OUTPUT_FORMAT OUTPUT_LEMONBAR

/* segments to use */
static const unsigned int USE_SEGMENTS[] = {
    SEGMENT_STATUS,
    SEGMENT_HOST,
    SEGMENT_CWD_PREFIX,
    SEGMENT_CWD,
    SEGMENT_VCS
};
static const int NUM_SEGMENTS = sizeof(USE_SEGMENTS) / sizeof(int);

#endif // _CONFIG_H
