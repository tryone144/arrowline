/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: renderer.c
 * v0.7 / 2015.08.07
 *
 * (c) 2015 Bernd Busse
 * The MIT License (MIT)
 **/

#include <stdio.h>
#include <string.h>

#include "config-prompt.h"
#include "config-status.h"
#include "utils.h"

#include "renderer.h"

/**
 * COLOR ESCAPESEQUENCES AND SEGMENT GENERATION
 **/

/* escape ANSI color codes for shell prompt */
void al_ansi_escape_color(char* dest, unsigned int maxlen, int fg, int bg, int style) {
    char buf[BUF_FORMAT_LEN] = { 0 };
#if PROMPT_OUTPUT_FORMAT == OUTPUT_BASH
    snprintf(buf, BUF_FORMAT_LEN, "\\[\\e[0;38;5;%d;48;5;%d;%dm\\]", fg, bg, style);
#elif PROMPT_OUTPUT_FORMAT == OUTPUT_ZSH
    snprintf(buf, BUF_FORMAT_LEN, "%%{\\e[0;38;5;%d;48;5;%d;%dm%%}", fg, bg, style);
#else // PLAIN
    snprintf(buf, BUF_FORMAT_LEN, "\\e[0;38;5;%d;48;5;%d;%dm", fg, bg, style);
#endif // OUTPUT_TERM
    
    al_string_cat(dest, buf, maxlen);
}

/* escape ANSI fg color and reset bg color for shell prompt (end segment) */
void al_ansi_escape_bg_reset(char* dest, unsigned int maxlen, int bg) {
    char buf[BUF_FORMAT_LEN] = { 0 };
#if PROMPT_OUTPUT_FORMAT == OUTPUT_BASH
    snprintf(buf, BUF_FORMAT_LEN, "\\[\\e[0;38;5;%d;49;22m\\]", bg);
#elif PROMPT_OUTPUT_FORMAT == OUTPUT_ZSH
    snprintf(buf, BUF_FORMAT_LEN, "%%{\\e[0;38;5;%d;49;22m%%}", bg);
#else // PLAIN
    snprintf(buf, BUF_FORMAT_LEN, "\\e[0;38;5;%d;49;22m", bg);
#endif // OUTPUT_TERM
    
    al_string_cat(dest, buf, maxlen);
}

/* reset ANSI color codes for shell prompt */
void al_ansi_escape_reset(char* dest, unsigned int maxlen) {
    char buf[BUF_FORMAT_LEN] = { 0 };
#if PROMPT_OUTPUT_FORMAT == OUTPUT_BASH
    snprintf(buf, BUF_FORMAT_LEN, "\\[\\e[0m\\]");
#elif PROMPT_OUTPUT_FORMAT == OUTPUT_ZSH
    snprintf(buf, BUF_FORMAT_LEN, "%%{\\e[0m%%}");
#else // PLAIN
    snprintf(buf, BUF_FORMAT_LEN, "\\e[0m");
#endif // OUTPUT_TERM

    al_string_cat(dest, buf, maxlen);
}

/* generate segment separator 1 with color codes */
void al_separator_segment(char* dest, unsigned int maxlen, int prev_bg, int next_bg, int position) {
    char buf[BUF_FORMAT_LEN] = { 0 };
    if (position != POSITION_RIGHT) {
        al_ansi_escape_color(buf, BUF_FORMAT_LEN, prev_bg, next_bg, FNT_NORMAL);
        al_string_cat(buf, SEPARATOR_SEGMENT_RIGHT, BUF_FORMAT_LEN);
    }
    if (position != POSITION_LEFT) {
        al_ansi_escape_color(buf, BUF_FORMAT_LEN, next_bg, prev_bg, FNT_NORMAL);
        al_string_cat(buf, SEPARATOR_SEGMENT_LEFT, BUF_FORMAT_LEN);
    }

    al_string_cat(dest, buf, maxlen);
}

/* generate segment separator 2 with color codes */
void al_separator_subsegment(char* dest, unsigned int maxlen, int fg, int bg, int position) {
    char buf[BUF_FORMAT_LEN] = { 0 };
    char *sep = position ? SEPARATOR_PATH_LEFT : SEPARATOR_PATH_RIGHT;
    al_ansi_escape_color(buf, BUF_FORMAT_LEN, fg, bg, FNT_NORMAL);
    al_string_cat(buf, sep, BUF_FORMAT_LEN);
    
    al_string_cat(dest, buf, maxlen);
}

/* generate start separator */
void al_segment_start(char* dest, unsigned int maxlen, int bg, int position) {
    char buf[BUF_FORMAT_LEN] = { 0 };
    if (position != POSITION_LEFT) {
        al_ansi_escape_bg_reset(buf, BUF_FORMAT_LEN, bg);
        al_string_cat(buf, SEPARATOR_SEGMENT_LEFT, BUF_FORMAT_LEN);
        al_string_cat(dest, buf, maxlen);
    }
}

/* generate end separator with color reset */
void al_segment_end(char** dest, unsigned int* maxlen, int bg, int position) {
    char buf[BUF_FORMAT_LEN] = { 0 };
    if (position != POSITION_RIGHT) {
        al_ansi_escape_bg_reset(buf, BUF_FORMAT_LEN, bg);
        al_string_cat(buf, SEPARATOR_SEGMENT_RIGHT, BUF_FORMAT_LEN);
    }
    al_ansi_escape_reset(buf, BUF_FORMAT_LEN);
    
    // add to promptline
    al_resize_char_buffer(dest, buf, maxlen, BUF_PROMPT_LEN);
    al_string_cat(*dest, buf, *maxlen);
}

/* generate segment with color codes and separator at begining */
void al_gen_segment(char** dest, unsigned int* maxlen, int cur_fg, int cur_bg, int style, const char* text, int* is_first, int *last_bg, int position) {
    char buf[BUF_PROMPT_LEN] = { 0 };

    if ((is_first != NULL) && (last_bg != NULL)) {
        // add separator if not first segment
        if (*is_first == 1) {
            al_segment_start(buf, BUF_PROMPT_LEN, cur_bg, position);
        } else {
            al_separator_segment(buf, BUF_PROMPT_LEN, *last_bg, cur_bg, position);
        }
        *is_first = 0;
        *last_bg = cur_bg;
    }

    // add color escape
    al_ansi_escape_color(buf, BUF_PROMPT_LEN, cur_fg, cur_bg, style);
    // add text
    al_string_cat(buf, text, BUF_PROMPT_LEN);
    
    // add to promptline
    al_resize_char_buffer(dest, buf, maxlen, BUF_PROMPT_LEN);
    al_string_cat(*dest, buf, *maxlen);
}

/* generate subsegment with color codes and path separator at begining */
void al_gen_subsegment(char** dest, unsigned int* maxlen, int cur_fg, int cur_bg, int sep_fg, int style, const char* text, int position) {
    char buf[BUF_PROMPT_LEN] = { 0 };

    // add path separator
    al_separator_subsegment(buf, BUF_PROMPT_LEN, sep_fg, cur_bg, position);
    // add color escape
    al_ansi_escape_color(buf, BUF_PROMPT_LEN, cur_fg, cur_bg, style);
    // add text
    al_string_cat(buf, text, BUF_PROMPT_LEN);
    
    // add to promptline
    al_resize_char_buffer(dest, buf, maxlen, BUF_PROMPT_LEN);
    al_string_cat(*dest, buf, *maxlen);
}
