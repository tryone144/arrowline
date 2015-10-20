/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: renderer.c
 * v0.7 / 2015.10.07
 *
 * (c) 2015 Bernd Busse
 * The MIT License (MIT)
 **/

#include <stdio.h>
#include <string.h>

#include "config.h"
#include "utils.h"

#include "renderer.h"

/**
 * COLOR ESCAPESEQUENCES AND SEGMENT GENERATION
 **/

/* escape ANSI color codes for shell prompt */
void al_ansi_escape_color(int fg, int bg, int style) {
#if OUTPUT_FORMAT == OUTPUT_BASH
    fprintf(stdout, "\\[\\e[0;38;5;%d;48;5;%d;%dm\\]", fg, bg, style);
#elif OUTPUT_FORMAT == OUTPUT_ZSH
    fprintf(stdout, "%%{\\e[0;38;5;%d;48;5;%d;%dm%%}", fg, bg, style);
#else // PLAIN
    fprintf(stdout, "\\e[0;38;5;%d;48;5;%d;%dm", fg, bg, style);
#endif // OUTPUT_TERM
}

/* escape ANSI fg color and reset bg color for shell prompt (end segment) */
void al_ansi_escape_bg_reset(int bg) {
#if OUTPUT_FORMAT == OUTPUT_BASH
    fprintf(stdout, "\\[\\e[0;38;5;%d;49;22m\\]", bg);
#elif OUTPUT_FORMAT == OUTPUT_ZSH
    fprintf(stdout, "%%{\\e[0;38;5;%d;49;22m%%}", bg);
#else // PLAIN
    fprintf(stdout, "\\e[0;38;5;%d;49;22m", bg);
#endif // OUTPUT_TERM
}

/* reset ANSI color codes for shell prompt */
void al_ansi_escape_reset() {
#if OUTPUT_FORMAT == OUTPUT_BASH
    fprintf(stdout, "\\[\\e[0m\\]");
#elif OUTPUT_FORMAT == OUTPUT_ZSH
    fprintf(stdout, "%%{\\e[0m%%}");
#else // PLAIN
    fprintf(stdout, "\\e[0m");
#endif // OUTPUT_TERM
}

/* generate segment separator 1 with color codes */
void al_separator_segment(int prev_bg, int next_bg, int position) {
    if (position != POSITION_RIGHT) {
        al_ansi_escape_color(prev_bg, next_bg, FNT_NORMAL);
        fprintf(stdout, SEPARATOR_SEGMENT_RIGHT);
    }
    if (position != POSITION_LEFT) {
        al_ansi_escape_color(next_bg, prev_bg, FNT_NORMAL);
        fprintf(stdout, SEPARATOR_SEGMENT_LEFT);
    }
}

/* generate segment separator 2 with color codes */
void al_separator_subsegment(int fg, int bg, int position) {
    al_ansi_escape_color(fg, bg, FNT_NORMAL);
    
    if (position != POSITION_RIGHT) {
        fprintf(stdout, SEPARATOR_PATH_RIGHT);
    }
    if (position != POSITION_LEFT) {
        fprintf(stdout, SEPARATOR_PATH_LEFT);
    }
}

/* generate start separator */
void al_segment_start(int bg, int position) {
    if (position != POSITION_LEFT) {
        al_ansi_escape_bg_reset(bg);
        fprintf(stdout, SEPARATOR_SEGMENT_LEFT);
    }
}

/* generate end separator with color reset */
void al_segment_end(int bg, int position) {
    if (position != POSITION_RIGHT) {
        al_ansi_escape_bg_reset(bg);
        fprintf(stdout, SEPARATOR_SEGMENT_RIGHT);
    }
    
    al_ansi_escape_reset();
}

/* generate segment with color codes and separator at begining */
void al_gen_segment(int cur_fg, int cur_bg, int style, const char* text, int* is_first, int *last_bg, int position) {
    if ((is_first != NULL) && (last_bg != NULL)) {
        // add separator if not first segment
        if (*is_first == 1) {
            al_segment_start(cur_bg, position);
        } else {
            al_separator_segment(*last_bg, cur_bg, position);
        }
        *is_first = 0;
        *last_bg = cur_bg;
    }

    // add color escape
    al_ansi_escape_color(cur_fg, cur_bg, style);
    // add text
    fprintf(stdout, text);
}

/* generate subsegment with color codes and path separator at begining */
void al_gen_subsegment(int cur_fg, int cur_bg, int sep_fg, int style, const char* text, int position) {
    // add path separator
    al_separator_subsegment(sep_fg, cur_bg, position);
    // add color escape
    al_ansi_escape_color(cur_fg, cur_bg, style);
    // add text
    fprintf(stdout, text);
}
