/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: arrowline.c
 * v0.6.3 / 2015.07.16
 *
 * (c) 2015 Bernd Busse
 * The MIT License (MIT)
 **/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef USE_VCS_GIT
    #include <git2.h>
#endif // USE_VCS_GIT

#include "config.h"
#include "utils.h"
#include "segments.h"

/* output formatted prompt */
int main(int argc, char** argv) {
    // read arguments
    if (argc > 1) { // first argument is exit status
        last_exit_status = atoi(argv[1]);
    }

    char* prompt; // prompt buffer
    unsigned int prompt_len = BUF_PROMPT_LEN;
    if ((prompt = calloc(prompt_len, sizeof(char))) == NULL) {
        perror("ERROR: can't allocate prompt buffer");
        return EXIT_FAILURE;
    }

    int sep_bg = 0;
    int is_first = 1;

#ifdef USE_VCS_GIT
    // init libgit2
    git_libgit2_init();
#endif // USE_VCS_GIT

    for (int s = 0; s < NUM_SEGMENTS; s++) {
        int retval;
        switch (USE_SEGMENTS[s]) {
            case SEGMENT_STATUS:
                // EXIT STATUS
                retval = al_segment_status(&prompt, &prompt_len, &is_first, &sep_bg);
                break;
            case SEGMENT_HOST:
                // USERNAME @ HOSTNAME
                retval = al_segment_host(&prompt, &prompt_len, &is_first, &sep_bg);
                break;
            case SEGMENT_CWD:
                // CURRENT WORKING DIRECTORY
                retval = al_segment_cwd(&prompt, &prompt_len, &is_first, &sep_bg);
                break;
            case SEGMENT_VCS:
                // VCS BRANCH STATUS (GIT)
                retval = al_segment_vcs(&prompt, &prompt_len, &is_first, &sep_bg);
                break;
            default:
                perror("ERROR: can't generate unknown segment");
                exit(EXIT_FAILURE);
        }
        if (retval != 0) {
            perror("ERROR: can't generate segment");
            exit(EXIT_FAILURE);
        }
    }

#ifdef USE_VCS_GIT
    // shutdown libgit2
    git_libgit2_shutdown();
#endif // USE_VCS_GIT

    // END PROMPT / RESET SEPARATOR
    al_separator_end(&prompt, &prompt_len, sep_bg);

    // output prompt buffer to stdout and exit
    fprintf(stdout, prompt);
    free(prompt);
    
    return EXIT_SUCCESS;
}
