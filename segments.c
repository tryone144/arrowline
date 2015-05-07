/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: segments.c
 * v0.3 / 2015.05.07
 *
 * (c) 2015 Bernd Busse
 **/

#include <stdio.h>
#include <string.h>

#include "config.h"
#include "utils.h"

#include "segments.h"

#define FNT_BOLD 1
#define FNT_NORMAL 22

#define SEPARATOR1 ""
#define SEPARATOR2 ""

/**
 * BASH ESCAPESEQUENCES AND SEGMENT GENERATION
 **/

/* escape color codes for BASH prompt */
void al_color_esc(char* dest, int fg, int bg, int style) {
    char buf[48];
    sprintf(buf, "\\[\\e[0;38;5;%d;48;5;%d;%dm\\]", fg, bg, style);
    strcat(dest, buf);
}

/* generate segment separator 1 with color codes */
void al_separator_segment(char* dest, int prev_bg, int next_bg) {
    al_color_esc(dest, prev_bg, next_bg, FNT_NORMAL);
    strcat(dest, SEPARATOR1);
}

/* generate segment separator 2 with color codes */
void al_separator_subsegment(char* dest, int fg, int bg) {
    al_color_esc(dest, fg, bg, FNT_NORMAL);
    strcat(dest, SEPARATOR2);
}

/* generate end separator with terminal color reset */
void al_separator_end(char* dest, int bg) {
    char col[48];
    sprintf(col, "\\[\\e[0;38;5;%d;49;22m\\]", bg);

    strcat(dest, col);
    strcat(dest, SEPARATOR1);
    strcat(dest, "\\[\\e[0m\\] ");
}

/* generate segment with color codes and separator at begining */
void al_gen_segment(char* dest, int cur_fg, int cur_bg, int style, char* text, int* is_first, int *last_bg) {
    if ((is_first != NULL) && (last_bg != NULL)) {
        // add separator if not first segment
        if (*is_first == 0) {
            al_separator_segment(dest, *last_bg, cur_bg);
        }
        *is_first = 0;
        *last_bg = cur_bg;
    }

    // add color escape
    al_color_esc(dest, cur_fg, cur_bg, style);
    // add text
    strcat(dest, text);
}

/* generate subsegment with color codes and path separator at begining */
void al_gen_subsegment(char* dest, int cur_fg, int cur_bg, int sep_fg, int style, char* text) {
    // add path separator
    al_separator_subsegment(dest, sep_fg, cur_bg);
    // add color escape
    al_color_esc(dest, cur_fg, cur_bg, style);
    // add text
    strcat(dest, text);
}


/**
 * SEGMENT GENERATORS: GATHER INFORMATION AND FORMAT ACCORDINGLY
 **/

/* show username and hostname with colorcodes for ROOT or SSH */
int al_segment_host(char* prompt, int* is_first, int* last_bg) {
    char text[64];
    int color = COLOR_BG_HOST_USER;

    char username[32];
    char hostname[32];

    if (al_get_username(username, 32) != 0) {
        return -1; // error
    }
    if (al_get_hostname(hostname, 32) != 0) {
        return -1; // error
    }

    // change color on ssh or root sessions
    if (al_is_ssh_connection()) {
        color = COLOR_BG_HOST_SSH;
    } else if (al_is_root_session()) {
        color = COLOR_BG_HOST_ROOT;
    }

    // add segment to prompt buffer
    snprintf(text, 64, " %s@%s ", username, hostname);
    al_gen_segment(prompt, COLOR_FG_HOST, color, FNT_BOLD, text, is_first, last_bg);

    return 0;
}

/* show current working dir */
int al_segment_cwd(char* prompt, int* is_first, int* last_bg) {
    char dirs[CWD_LEN + 1][64];

    char home[64];
    char cwd[512];
    char *base = cwd;

    if (al_get_home(home, 64) != 0) {
        return -1;
    }
    if (al_get_cwd(cwd, 512) != 0) {
        return -1;
    }

    // check if in home directory
    if (al_string_startswith(cwd, home)) {
        base = cwd+strlen(home);
    }

    // copy directory names into dirs buffer
    for (int i = 0; i <= CWD_LEN; i++) {
        al_get_dir_name(dirs[i], 64, base, i);
    }

    // copy first path element into dirs buffer
    for (int i = 0; i <= CWD_LEN; i++) {
        if ((strlen(dirs[i]) == 0) || (i == CWD_LEN)) {
            if (al_get_dir_count(base) > CWD_LEN) {
                strcpy(dirs[i], "⋯"); // deep in hirarchy
            } else if (al_string_startswith(cwd, home)) {
                strcpy(dirs[i], "~"); // home directory
            } else {
                strcpy(dirs[i], "/"); // everywhere else
            }
            break;
        }
    }

    char text[66];
    int style;
    int first_path = 1;

    for (int i = CWD_LEN; i >= 0; i--) {
        if (strlen(dirs[i]) != 0) {
            // render path
            snprintf(text, 66, " %s ", dirs[i]);

            // add segment to prompt buffer
            style = i == 0 ? FNT_BOLD : FNT_NORMAL;
            if (first_path) {
                al_gen_segment(prompt, COLOR_FG_CWD, COLOR_BG_CWD, style, text, is_first, last_bg);
                first_path = 0;
            } else {
                al_gen_subsegment(prompt, COLOR_FG_CWD, COLOR_BG_CWD, COLOR_SEP_CWD, style, text);
            }
        }
    }

    return 0;
}
