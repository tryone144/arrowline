/**
 * ARROWLINE
 * powerline-like shell prompt and statusline generator
 *
 * file: arrowbar.c
 * v0.7 / 2015.08.07
 *
 * (c) 2015 Bernd Busse
 * The MIT License (MIT)
 **/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "config-status.h"
#include "renderer.h"
#include "utils.h"

/* output formatted statusline */
int main() {
    char* status; // statusline buffer
    unsigned int status_len = BUF_PROMPT_LEN;
    if ((status = calloc(status_len, sizeof(char))) == NULL) {
        perror("ERROR: can't allocate statusline buffer");
        return EXIT_FAILURE;
    }

    int sep_bg = 0;
    int is_first = 1;
    segment_generator gen;

    if (NUM_STATUS_SEGMENTS_LEFT > 0) {
        for (int s = 0; s < NUM_STATUS_SEGMENTS_LEFT; s++) {
            if ((gen = STATUS_SEGMENTS_LEFT[s]) == NULL) {
                break;
            }
            if (gen(&status, &status_len, &is_first, &sep_bg, POSITION_LEFT) != 0) {
                perror("ERROR: can't generate segment");
                exit(EXIT_FAILURE);
            }
        }
        
        // end left segment
        al_segment_end(&status, &status_len, sep_bg, POSITION_LEFT);
    }
    
    if (NUM_STATUS_SEGMENTS_RIGHT > 0) {
        is_first = 1;
        for (int s = 0; s < NUM_STATUS_SEGMENTS_RIGHT; s++) {
            if ((gen = STATUS_SEGMENTS_RIGHT[s]) == NULL) {
                break;
            }
            if (gen(&status, &status_len, &is_first, &sep_bg, POSITION_RIGHT) != 0) {
                perror("ERROR: can't generate segment");
                exit(EXIT_FAILURE);
            }
        }

        // end right segment
        al_segment_end(&status, &status_len, sep_bg, POSITION_RIGHT);
    }
    
    if (NUM_STATUS_SEGMENTS_CENTER > 0) {
        is_first = 1;
        for (int s = 0; s < NUM_STATUS_SEGMENTS_CENTER; s++) {
            if ((gen = STATUS_SEGMENTS_CENTER[s]) == NULL) {
                break;
            }
            if (gen(&status, &status_len, &is_first, &sep_bg, POSITION_CENTER) != 0) {
                perror("ERROR: can't generate segment");
                exit(EXIT_FAILURE);
            }
        }
        
        // end centerd segment
        al_segment_end(&status, &status_len, sep_bg, POSITION_CENTER);
    }

    // add newline (flush)
    al_resize_char_buffer(&status, "\n", &status_len, 2);
    al_string_cat(status, "\n", status_len);

    // output prompt buffer to stdout and exit
    fprintf(stdout, status);
    free(status);
    
    return EXIT_SUCCESS;
}
