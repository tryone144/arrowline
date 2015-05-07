/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: arrowline.c
 * v0.4 / 2015.05.07
 *
 * (c) 2015 Bernd Busse
 **/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "utils.h"
#include "segments.h"

#define PROMPT_LEN 512

/* output formatted prompt */
int main(int argc, char** argv) {
    // read arguments
    if (argc == 2) {
        last_exit_status = atoi(argv[1]);
    }

    char prompt[PROMPT_LEN] = {0}; // prompt buffer
    int sep_bg = 0;
    int is_first = 1;

#ifdef USE_SEGMENT_STATUS
    // EXIT STATUS
    if (al_segment_status(prompt, &is_first, &sep_bg) != 0) {
        perror("ERROR: can't generate 'exit status' segment");
        return EXIT_FAILURE;
    }
#endif // USE_SEGMENT_STATUS

#ifdef USE_SEGMENT_HOST
    // USERNAME @ HOSTNAME
    if (al_segment_host(prompt, &is_first, &sep_bg) != 0) {
        perror("FEHLER: can't generate 'username/hostname' segment");
        return EXIT_FAILURE;
    }
#endif // USE_SEGMENT_HOST

#ifdef USE_SEGMENT_CWD
    // CURRENT WORKING DIRECTORY
    if (al_segment_cwd(prompt, &is_first, &sep_bg) != 0) {
        perror("ERROR: can't generate 'current working dir' segment");
        return EXIT_FAILURE;
    }
#endif // USE_SEGMENT_CWD

    // END PROMPT / RESET SEPARATOR
    al_separator_end(prompt, sep_bg);

    // output prompt buffer to stdout and exit
    fprintf(stdout, prompt);
    return EXIT_SUCCESS;
}
