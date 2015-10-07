/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: config.h
 * v0.7 / 2015.10.07
 *
 * (c) 2015 Bernd Busse
 * The MIT License (MIT)
 **/

#ifndef _CONFIG_H
#define _CONFIG_H

#include "segments.h"

/**
 * CONSTANTS: DO NOT CHANGE!
 **/

/* output formats (set below) */
#define OUTPUT_PLAIN 1
#define OUTPUT_BASH 2
#define OUTPUT_ZSH 4

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

/* output styles */
#define USE_POWERLINE_SYMBOLS       // use powerline symbols, requires powerline font
#define OUTPUT_FORMAT OUTPUT_BASH   // change escape character formatting to match your shell

/* number of parent dirs to show */
#define CWD_LEN 3

/* date and time formatting */
#define TIME_FMT "%H:%M:%S"
#define DATE_FMT "%a, %d. %b"

/* segments to use (NULL-terminated!) */
static const segment_generator SEGMENTS_LEFT[] = {
    al_segment_status,
    al_segment_host,
    al_segment_cwd_prefix,
    al_segment_cwd,
    al_segment_vcs,
    NULL
};

static const segment_generator SEGMENTS_RIGHT[] = {
    al_segment_datetime,
    NULL
};

/* segment count (DO NOT CHANGE!) */
static const int NUM_SEGMENTS_LEFT = sizeof(SEGMENTS_LEFT) / sizeof(segment_generator) - 1;
static const int NUM_SEGMENTS_RIGHT = sizeof(SEGMENTS_RIGHT) / sizeof(segment_generator) - 1;

#endif // _CONFIG_H
