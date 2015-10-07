/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: segments.h
 * v0.7 / 2015.10.07
 *
 * (c) 2015 Bernd Busse
 * The MIT License (MIT)
 **/

#ifndef _SEGMENTS_h
#define _SEGMENTS_H

/* signature of a segment generation function */
typedef int (*segment_generator)(int* is_first, int* last_bg, int position);

/**
 * SEGMENT GENERATORS: GATHER INFORMATION AND FORMAT ACCORDINGLY
 **/

/* show date and time */
int al_segment_datetime(int* is_first, int* last_bg, int position);

/* show current working dir */
int al_segment_cwd(int* is_first, int* last_bg, int position);

/* show current working dir prefix */
int al_segment_cwd_prefix(int* is_first, int* last_bg, int position);

/* show username and hostname with colorcodes for ROOT or SSH */
int al_segment_host(int* is_first, int* last_bg, int position);

/* show last exit status if command failed */
int al_segment_status(int* is_first, int* last_bg, int position);

/* show vcs status if cwd is part of a vcs */
int al_segment_vcs(int* is_first, int* last_bg, int position);

#endif // _SEGMENTS_H
