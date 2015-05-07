/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: arrowline.c
 * v0.3 / 2015.05.07
 *
 * (c) 2015 Bernd Busse
 **/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "segments.h"

#define PROMPT_LEN 512

/* output formatted prompt */
int main(int argc, char** argv) {
    char prompt[PROMPT_LEN] = {0}; // prompt buffer
    int sep_bg = 0;
    int is_first = 1;

#ifdef USE_SEGMENT_HOST
    // USERNAME @ HOSTNAME
    //al_gen_sep1(prompt, sep_bg, COLOR_BG_HOST_USER, &is_first);
    if (al_segment_host(prompt, &is_first, &sep_bg) != 0) {
        perror("FEHLER: Problem beim Erzeugen von 'username/hostname' Segment");
        return EXIT_FAILURE;
    }
#endif

#ifdef USE_SEGMENT_CWD
    // CURRENT WORKING DIRECTORY
    //al_gen_sep1(prompt, sep_bg, COLOR_BG_HOST_USER, &is_first);
    if (al_segment_cwd(prompt, &is_first, &sep_bg) != 0) {
        perror("FEHLER: Problem beim Erzeugen von 'current working dir' Segment");
        return EXIT_FAILURE;
    }
#endif

    al_separator_end(prompt, sep_bg);

    // output prompt buffer to stdout and exit
    fprintf(stdout, prompt);
    return EXIT_SUCCESS;
}
