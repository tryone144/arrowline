/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: renderer.h
 * v0.7.5 / 2016.04.10
 *
 * (c) 2016 Bernd Busse
 * The MIT License (MIT)
 **/

#ifndef _RENDERER_H
#define _RENDERER_H

#define FNT_BOLD 1
#define FNT_NORMAL 22

#define POSITION_LEFT 0     // points right
#define POSITION_RIGHT 1    // points left
#define POSITION_CENTER 2   // points left and right

#ifdef USE_POWERLINE_SYMBOLS
    #define SEPARATOR_SEGMENT_RIGHT ""     // \uE0B0
    #define SEPARATOR_SEGMENT_LEFT ""     // \uE0B2
    #define SEPARATOR_PATH_RIGHT ""        // \uE0B1
    #define SEPARATOR_PATH_LEFT ""        // \uE0B3
#else
    #define SEPARATOR_SEGMENT_RIGHT ""
    #define SEPARATOR_SEGMENT_LEFT ""
    #define SEPARATOR_PATH_RIGHT "/"
    #define SEPARATOR_PATH_LEFT "\\"
#endif

#define THREE_DOTS "⋯"          // \u22EF
#define BOLD_X "✘"              // \u2718
#define BRANCH ""              // \uE0A0
#define PLUSMINUS "±"           // \u00B1

/**
 * SEGMENT RENDERERS
 **/

/* generate segment with color codes and separator at begining */
void al_gen_segment(int cur_fg, int cur_bg, int style, const char* text, int* is_first, int *last_bg, int position);

/* generate subsegment with color codes and path separator at begining */
void al_gen_subsegment(int cur_fg, int cur_bg, int sep_fg, int style, const char* text, int position);

/* generate start separator */
void al_segment_start(int bg, int position);

/* generate end separator with terminal color reset */
void al_segment_end(int bg, int position);

#endif // _RENDERER_H
