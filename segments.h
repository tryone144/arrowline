/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: segments.h
 * v0.6.3 / 2015.07.16
 *
 * (c) 2015 Bernd Busse
 * The MIT License (MIT)
 **/

#ifndef _SEGMENTS_h
#define _SEGMENTS_H

#define ORIENTATION_RIGHT 0
#define ORIENTATION_LEFT 1

/**
 * SEGMENT GENERATORS: GATHER INFORMATION AND FORMAT ACCORDINGLY
 **/

/* generate end separator with terminal color reset */
void al_segment_end(char** dest, unsigned int* maxlen, int bg, int orientation);

/* show current working dir */
int al_segment_cwd(char** prompt, unsigned int* prompt_len, int* is_first, int* last_bg, int orientation);

/* show current working dir prefix */
int al_segment_cwd_prefix(char** prompt, unsigned int* prompt_len, int* is_first, int* last_bg, int orientation);

/* show username and hostname with colorcodes for ROOT or SSH */
int al_segment_host(char** prompt, unsigned int* prompt_len, int* is_first, int* last_bg, int orientation);

/* show last exit status if command failed */
int al_segment_status(char** prompt, unsigned int* prompt_len, int* is_first, int* last_bg, int orientation);

/* show vcs status if cwd is part of a vcs */
int al_segment_vcs(char** prompt, unsigned int* prompt_len, int* is_first, int* last_bg, int orientation);

#endif // _SEGMENTS_H
