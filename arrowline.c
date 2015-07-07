/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: arrowline.c
 * v0.6 / 2015.07.07
 *
 * (c) 2015 Bernd Busse
 **/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <git2.h>

#include "config.h"
#include "utils.h"
#include "segments.h"

/* output formatted prompt */
int main(int argc, char** argv) {
    // read arguments
    if (argc == 2) {
        last_exit_status = atoi(argv[1]);
    }

    char* prompt; // prompt buffer
    int prompt_len = PROMPT_LEN_STEP;
    if ((prompt = calloc(prompt_len, sizeof(char))) == NULL) {
        perror("ERROR: can't allocate prompt buffer");
        return EXIT_FAILURE;
    }

    int sep_bg = 0;
    int is_first = 1;

    // init libgit2
    git_libgit2_init();

#ifdef USE_SEGMENT_STATUS
    // EXIT STATUS
    if (al_segment_status(&prompt, &prompt_len, &is_first, &sep_bg) != 0) {
        perror("ERROR: can't generate 'exit status' segment");
        return EXIT_FAILURE;
    }
#endif // USE_SEGMENT_STATUS

#ifdef USE_SEGMENT_HOST
    // USERNAME @ HOSTNAME
    if (al_segment_host(&prompt, &prompt_len, &is_first, &sep_bg) != 0) {
        perror("FEHLER: can't generate 'username/hostname' segment");
        return EXIT_FAILURE;
    }
#endif // USE_SEGMENT_HOST

#ifdef USE_SEGMENT_CWD
    // CURRENT WORKING DIRECTORY
    if (al_segment_cwd(&prompt, &prompt_len, &is_first, &sep_bg) != 0) {
        perror("ERROR: can't generate 'current working dir' segment");
        return EXIT_FAILURE;
    }
#endif // USE_SEGMENT_CWD

#ifdef USE_SEGMENT_VCS
    // VCS BRANCH STATUS (GIT)
    if (al_segment_vcs(&prompt, &prompt_len, &is_first, &sep_bg) != 0) {
        perror("ERROR: can't generate 'vcs stats' segment");
        return EXIT_FAILURE;
    }
#endif

    // shutdown libgit2
    git_libgit2_shutdown();

    // END PROMPT / RESET SEPARATOR
    al_separator_end(&prompt, &prompt_len, sep_bg);

    // output prompt buffer to stdout and exit
    fprintf(stdout, prompt);
    free(prompt);
    return EXIT_SUCCESS;
}
