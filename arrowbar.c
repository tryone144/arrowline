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

    const int position = POSITION_LEFT;
    int sep_bg = 0;
    int is_first = 1;

    for (int s = 0; s < NUM_STATUS_SEGMENTS; s++) {
        if (STATUS_SEGMENTS[s](&status, &status_len, &is_first, &sep_bg, position) != 0) {
            perror("ERROR: can't generate segment");
            exit(EXIT_FAILURE);
        }
    }

    // END STATUSLINE / RESET SEPARATOR
    al_segment_end(&status, &status_len, sep_bg, position);
    al_resize_char_buffer(&status, " ", &status_len, 2);
    al_string_cat(status, " ", status_len);

    // output prompt buffer to stdout and exit
    fprintf(stdout, status);
    free(status);
    
    return EXIT_SUCCESS;
}
