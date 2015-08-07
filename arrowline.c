/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: arrowline.c
 * v0.6.4 / 2015.07.28
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

    const int orientation = ORIENTATION_RIGHT;
    int sep_bg = 0;
    int is_first = 1;

#ifdef USE_VCS_GIT
    // init libgit2
    git_libgit2_init();
#endif // USE_VCS_GIT

    for (int s = 0; s < NUM_PROMPT_SEGMENTS; s++) {
        if (PROMPT_SEGMENTS[s](&prompt, &prompt_len, &is_first, &sep_bg, orientation) != 0) {
            perror("ERROR: can't generate segment");
            exit(EXIT_FAILURE);
        }
    }

#ifdef USE_VCS_GIT
    // shutdown libgit2
    git_libgit2_shutdown();
#endif // USE_VCS_GIT

    // END PROMPT / RESET SEPARATOR
    al_segment_end(&prompt, &prompt_len, sep_bg, orientation);
    al_resize_char_buffer(&prompt, " ", &prompt_len, 2);
    al_string_cat(prompt, " ", prompt_len);

    // output prompt buffer to stdout and exit
    fprintf(stdout, prompt);
    free(prompt);
    
    return EXIT_SUCCESS;
}
