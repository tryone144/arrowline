/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: segments.h
 * v0.6.2 / 2015.07.07
 *
 * (c) 2015 Bernd Busse
 * The MIT License (MIT)
 **/

#ifndef _SEGMENTS_h
#define _SEGMENTS_H

/**
 * SEGMENT GENERATORS: GATHER INFORMATION AND FORMAT ACCORDINGLY
 **/

/* generate end separator with terminal color reset */
void al_separator_end(char** prompt, int* prompt_len, int bg);

/* show current working dir */
int al_segment_cwd(char** prompt, int* prompt_len, int* is_first, int* last_bg);

/* show username and hostname with colorcodes for ROOT or SSH */
int al_segment_host(char** prompt, int* prompt_len, int* is_first, int* last_bg);

/* show last exit status if command failed */
int al_segment_status(char** prompt, int* prompt_len, int* is_first, int* last_bg);

/* show vcs status if cwd is part of a vcs */
int al_segment_vcs(char** prompt, int* prompt_len, int* is_first, int* last_bg);

#endif // _SEGMENTS_H
