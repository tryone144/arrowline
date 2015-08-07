/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: arrowline.c
 * v0.7 / 2015.08.07
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

#include "config-prompt.h"
#include "renderer.h"
#include "utils.h"

/* output formatted prompt */
int main(int argc, char** argv) {
    int position = POSITION_LEFT;
    
    // read arguments
    if (argc > 1) { // first argument is exit status
        last_exit_status = atoi(argv[1]);

        if (argc > 2) { // second argument is position / orientation
            position = atoi(argv[2]) == 0 ? POSITION_LEFT : POSITION_RIGHT;
        }
    }

    char* prompt; // prompt buffer
    unsigned int prompt_len = BUF_PROMPT_LEN;
    if ((prompt = calloc(prompt_len, sizeof(char))) == NULL) {
        perror("ERROR: can't allocate prompt buffer");
        return EXIT_FAILURE;
    }

    int sep_bg = 0;
    int is_first = 1;
    segment_generator gen;

#ifdef USE_VCS_GIT
    // init libgit2
    git_libgit2_init();
#endif // USE_VCS_GIT
    
    if (position == POSITION_LEFT) {
        for (int s = 0; s < NUM_PROMPT_SEGMENTS_LEFT; s++) {
            if ((gen = PROMPT_SEGMENTS_LEFT[s]) == NULL) {
                break;
            }
            if (gen(&prompt, &prompt_len, &is_first, &sep_bg, POSITION_LEFT) != 0) {
                perror("ERROR: can't generate segment");
                exit(EXIT_FAILURE);
            }
        }

        // end prompt and add trailing space
        al_segment_end(&prompt, &prompt_len, sep_bg, POSITION_LEFT);
        al_resize_char_buffer(&prompt, " ", &prompt_len, 2);
        al_string_cat(prompt, " ", prompt_len);
    } else {
        for (int s = 0; s < NUM_PROMPT_SEGMENTS_RIGHT; s++) {
            if ((gen = PROMPT_SEGMENTS_RIGHT[s]) == NULL) {
                break;
            }
            if (gen(&prompt, &prompt_len, &is_first, &sep_bg, POSITION_RIGHT) != 0) {
                perror("ERROR: can't generate segment");
                exit(EXIT_FAILURE);
            }
        }

        // end prompt without spaces
        al_segment_end(&prompt, &prompt_len, sep_bg, POSITION_RIGHT);
    }

#ifdef USE_VCS_GIT
    // shutdown libgit2
    git_libgit2_shutdown();
#endif // USE_VCS_GIT

    // output prompt buffer to stdout and exit
    fprintf(stdout, prompt);
    free(prompt);
    
    return EXIT_SUCCESS;
}
