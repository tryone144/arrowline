/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: segments.h
 * v0.3 / 2015.05.07
 *
 * (c) 2015 Bernd Busse
 **/

#ifndef _SEGMENTS_h
#define _SEGMENTS_H

/**
 * SEGMENT GENERATORS: GATHER INFORMATION AND FORMAT ACCORDINGLY
 **/

/* show username and hostname with colorcodes for ROOT or SSH */
int al_segment_host(char* prompt, int* is_first, int* last_bg);

/* show current working dir */
int al_segment_cwd(char* prompt, int* is_first, int* last_bg);

/* generate end separator with terminal color reset */
void al_separator_end(char* dest, int bg);


//void al_gen_sep1(char* dest, int prev_bg, int next_bg, int* is_first);



#endif // _SEGMENTS_H
