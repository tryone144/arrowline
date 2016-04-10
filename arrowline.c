/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: arrowline.c
 * v0.7.5 / 2016.04.10
 *
 * (c) 2016 Bernd Busse
 * The MIT License (MIT)
 **/

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "renderer.h"
#include "segments.h"
#include "utils.h"

/* clear term settings and exit with -1 */
void die(const char* msg, ...) {
    va_list va_args;
    va_start(va_args, msg);

    al_segment_end(0, POSITION_RIGHT);
    fprintf(stderr, "Error: ");
    vfprintf(stderr, msg, va_args);
    fprintf(stderr, "\n");
    
    va_end(va_args);
    exit(EXIT_FAILURE);
}

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
    int s = 0;
    segment_generator gen;

    if (position == POSITION_LEFT) {
        while ((gen = SEGMENTS_LEFT[s]) != NULL) {
            if (gen(&is_first, &sep_bg, POSITION_LEFT) != 0) {
                die("can't generate segment '%s': %s\n", al_get_name_for_segment(gen), last_error);
            }
            s++;
        }
        s = 0;

        // end prompt and add trailing space
        al_segment_end(sep_bg, POSITION_LEFT);
        fprintf(stdout, " ");
    } else {
        while ((gen = SEGMENTS_RIGHT[s]) != NULL) {
            if (gen(&is_first, &sep_bg, POSITION_RIGHT) != 0) {
                die("can't generate segment '%s': %s\n", al_get_name_for_segment(gen), last_error);
            }
            s++;
        }

        // end prompt without spaces
        al_segment_end(sep_bg, POSITION_RIGHT);
    }

    // flush stdout and exit
    fflush(stdout);
    
    return EXIT_SUCCESS;
}
