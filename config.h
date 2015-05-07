/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: config.h
 * v0.4 / 2015.05.07
 *
 * (c) 2015 Bernd Busse
 **/

#ifndef _CONFIG_H
#define _CONFIG_H

/**
 * CONFIGURATION HEADER
 **/

/* number of directories to show in hirarchy */
#define CWD_LEN 3

/* color codes for the segments */
#define COLOR_FG_HOST 16 // black
#define COLOR_BG_HOST_USER 47 // green
#define COLOR_BG_HOST_ROOT 196 // red
#define COLOR_BG_HOST_SSH 226 // yellow

#define COLOR_FG_STATUS 196 // red
#define COLOR_BG_STATUS 0 // black

#define COLOR_FG_CWD 250 // light-grey
#define COLOR_BG_CWD 240 // grey
#define COLOR_SEP_CWD 245 // light-grey

/* segment defines */
#define USE_SEGMENT_CWD
#define USE_SEGMENT_HOST
#define USE_SEGMENT_STATUS

#endif // _CONFIG_H
