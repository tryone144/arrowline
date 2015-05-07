/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: arrowline.c
 * v0.2 / 2015.05.07
 *
 * (c) 2015 Bernd Busse
 **/

#include <limits.h>
#include <pwd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "config.h"

#define PROMPT_LEN 512

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

/* generate segment with color codes */
void al_gen_segment(char* dest, int fg, int bg, int style, char* text) {
    al_color_esc(dest, fg, bg, style);
    strcat(dest, text);
}

/* generate segment separator 1 with color codes */
void al_gen_sep1(char* dest, int prev_bg, int next_bg) {
    al_color_esc(dest, prev_bg, next_bg, FNT_NORMAL);
    strcat(dest, SEPARATOR1);
}

/* generate segment separator 2 with color codes */
void al_gen_sep2(char* dest, int fg, int bg) {
    al_color_esc(dest, fg, bg, FNT_NORMAL);
    strcat(dest, SEPARATOR2);
}

/* generate end separator with terminal color reset */
void al_gen_end(char* dest, int bg) {
    char col[48];
    sprintf(col, "\\[\\e[0;38;5;%d;49;22m\\]", bg);

    strcat(dest, col);
    strcat(dest, SEPARATOR1);
    strcat(dest, "\\[\\e[0m\\] ");
}


/**
 * ENVIRONMENT HELPER FUNCTIONS AND WRAPPER
 **/

/* copy username from environment into buf */
int al_get_username(char* buf, size_t len) {
    struct passwd* pw = getpwuid(geteuid());
    if (pw == NULL) {
        return -1; // error
    }
    strncpy(buf, pw->pw_name, len);
    buf[len-1] = '\0';
    return 0;
}

/* copy hostname from environment into buf */
int al_get_hostname(char* buf, size_t len) {
    if (gethostname(buf, len) != 0) {
        return -1; // error
    }
    buf[len-1] = '\0';
    return 0;
}

/* copy path of users home directory into buf */
int al_get_home(char* buf, size_t len) {
    struct passwd* pw = getpwuid(geteuid());
    if (pw == NULL) {
        return -1; // error
    }
    strncpy(buf, pw->pw_dir, len);
    buf[len-1] = '\0';
    return 0;
}

/* copy path of current working directory into buf */
int al_get_cwd(char* buf, size_t len) {
    getcwd(buf, len);
    if (buf == NULL) {
        return -1; // error
    }
    buf[len-1] = '\0';
    return 0;
}

/* check if session is running over SSH */
int al_is_ssh_connection() {
    char* val = getenv("SSH_CONNECTION");
    if (val != NULL) {
        return 1; // SSH SESSION
    }
    return 0; // NOT running over SSH
}

/* check if user is root */
int al_is_root_session() {
    uid_t uid = getuid();
    if (uid == 0) {
        return 1; // USER IS ROOT
    }
    return 0; // NOT user root
}

/* check if str starts with pre */
int al_string_startswith(char* str, const char* pre) {
    size_t len = strlen(pre);
    if (strlen(str) < len) {
        return 0; // compare string pre is too long
    }
    return strncmp(str, pre, len) == 0;
}

/* return directory level of path */
int al_get_dir_count(char* path) {
    int c = 0;
    char* occ = path;

    if (path == NULL) {
        return -1; // error
    }
    // check if path string is empty: level 0
    if (strlen(path) == 0) {
        return 0;
    }

    for (int i = 0; i < INT_MAX; i++) {
        occ = strchr(occ+1, '/');
        if (occ == NULL) {
            break; // return c
        }
        c++;
    }

    return c;
}

/* write directory name count levels obove path into buf */
int al_get_dir_name(char* dest, size_t len, char* path, int count) {
    char buf[256];
    char* occ;

    // work on a local copy of path
    strncpy(buf, path, 256);
    buf[255] = '\0';

    for (int i = 0; i <= count; i++) {
        occ = strrchr(buf, '/');
        if (occ == NULL) {
            *dest = '\0';
            return -1;
        }
        *occ = '\0';
    }

    strncpy(dest, occ+1, len);
    dest[len-1] = '\0';

    return 0;
}


/**
 * SEGMENT GENERATORS: GATHER INFORMATION AND FORMAT ACCORDINGLY
 **/

/* show username and hostname with colorcodes for ROOT or SSH */
int al_seg_host(char* prompt, int* last_bg) {
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
    al_gen_segment(prompt, COLOR_FG_HOST, color, FNT_BOLD, text);

    // return background color for separator
    *last_bg = color;
    return 0;
}

/* show current working dir */
int al_seg_cwd(char* prompt, int* last_bg) {
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

    for (int i = CWD_LEN; i >= 0; i--) {
        if (strlen(dirs[i]) != 0) {
            // add segment to prompt buffer
            snprintf(text, 66, " %s ", dirs[i]);
            al_gen_segment(prompt, COLOR_FG_CWD, COLOR_BG_CWD, i == 0 ? FNT_BOLD : FNT_NORMAL, text);
            if (i > 0) { // add path separator to segment
                al_gen_sep2(prompt, COLOR_SEP_CWD, COLOR_BG_CWD);
            }
        }
    }

    // return background color for separator
    *last_bg = COLOR_BG_CWD;
    return 0;
}

/* output formatted prompt */
int main(int argc, char** argv) {
    char prompt[PROMPT_LEN] = {0}; // prompt buffer
    int sep_bg = 0;

    // USERNAME @ HOSTNAME
    if (al_seg_host(prompt, &sep_bg) != 0) {
        perror("FEHLER: Problem beim Erzeugen von 'username/hostname' Segment");
        return EXIT_FAILURE;
    }

    al_gen_sep1(prompt, sep_bg, COLOR_BG_CWD);

    // CURRENT WORKING DIRECTORY
    if (al_seg_cwd(prompt, &sep_bg) != 0) {
        perror("FEHLER: Problem beim Erzeugen von 'current working dir' Segment");
        return EXIT_FAILURE;
    }

    al_gen_end(prompt, sep_bg);

    // output prompt buffer to stdout and exit
    fprintf(stdout, prompt);
    return EXIT_SUCCESS;
}
