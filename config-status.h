/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: config.h
 * v0.7 / 2015.08.07
 *
 * (c) 2015 Bernd Busse
 * The MIT License (MIT)
 **/

#ifndef _CONFIG_STATUS_H
#define _CONFIG_STATUS_H

#include "config.h"
#include "segments.h"

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
#define USE_POWERLINE_SYMBOLS
#define STATUS_OUTPUT_FORMAT OUTPUT_LEMONBAR

/* segments to use (NULL-terminated!) */
static const segment_generator STATUS_SEGMENTS_LEFT[] = {
    al_segment_status,
    al_segment_host,
    al_segment_cwd_prefix,
    al_segment_cwd,
    NULL
};
static const int NUM_STATUS_SEGMENTS_LEFT = sizeof(STATUS_SEGMENTS_LEFT) / sizeof(segment_generator) - 1;

static const segment_generator STATUS_SEGMENTS_RIGHT[] = {
    al_segment_datetime,
    NULL
};
static const int NUM_STATUS_SEGMENTS_RIGHT = sizeof(STATUS_SEGMENTS_RIGHT) / sizeof(segment_generator) - 1;

static const segment_generator STATUS_SEGMENTS_CENTER[] = {
    NULL
};
static const int NUM_STATUS_SEGMENTS_CENTER = sizeof(STATUS_SEGMENTS_CENTER) / sizeof(segment_generator) - 1;


#endif // _CONFIG_H
