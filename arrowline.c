/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: arrowline.c
 * v0.7 / 2015.10.07
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

    int sep_bg = 0;
    int is_first = 1;
    segment_generator gen;

#ifdef USE_VCS_GIT
    // init libgit2
    git_libgit2_init();
#endif // USE_VCS_GIT
    
    if (position == POSITION_LEFT) {
        for (int s = 0; s < NUM_SEGMENTS_LEFT; s++) {
            if ((gen = SEGMENTS_LEFT[s]) == NULL) {
                break;
            }
            if (gen(&is_first, &sep_bg, POSITION_LEFT) != 0) {
                perror("ERROR: can't generate segment");
                exit(EXIT_FAILURE);
            }
        }

        // end prompt and add trailing space
        al_segment_end(sep_bg, POSITION_LEFT);
        fprintf(stdout, " ");
    } else {
        for (int s = 0; s < NUM_SEGMENTS_RIGHT; s++) {
            if ((gen = SEGMENTS_RIGHT[s]) == NULL) {
                break;
            }
            if (gen(&is_first, &sep_bg, POSITION_RIGHT) != 0) {
                perror("ERROR: can't generate segment");
                exit(EXIT_FAILURE);
            }
        }

        // end prompt without spaces
        al_segment_end(sep_bg, POSITION_RIGHT);
    }

#ifdef USE_VCS_GIT
    // shutdown libgit2
    git_libgit2_shutdown();
#endif // USE_VCS_GIT

    // flush stdout and exit
    fflush(stdout);
    
    return EXIT_SUCCESS;
}
