/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: segments.c
 * v0.6.3 / 2015.07.16
 *
 * (c) 2015 Bernd Busse
 * The MIT License (MIT)
 **/

#include <stdio.h>
#include <string.h>

#include "config.h"
#include "utils.h"

#include "segments.h"

#define FNT_BOLD 1
#define FNT_NORMAL 22

#ifdef USE_POWERLINE_SYMBOLS
    #define SEPARATOR_SEGMENT ""   // \uE0B0
    #define SEPARATOR_PATH ""      // \uE0B1
#else
    #define SEPARATOR_SEGMENT ""
    #define SEPARATOR_PATH "/"
#endif

#define THREE_DOTS "⋯"          // \u22EF
#define BOLD_X "✘"              // \u2718
#define BRANCH ""              // \uE0A0
#define PLUSMINUS "±"           // \u00B1

/**
 * BASH ESCAPESEQUENCES AND SEGMENT GENERATION
 **/

/* escape color codes for BASH prompt */
void al_color_esc(char* dest, int len, int fg, int bg, int style) {
    char buf[BUF_FORMAT_LEN] = { 0 };
#if OUTPUT_FORMAT == OUTPUT_BASH
    snprintf(buf, BUF_FORMAT_LEN, "\\[\\e[0;38;5;%d;48;5;%d;%dm\\]", fg, bg, style);
#elif OUTPUT_FORMAT == OUTPUT_ZSH
    snprintf(buf, BUF_FORMAT_LEN, "%%{\\e[0;38;5;%d;48;5;%d;%dm%%}", fg, bg, style);
#else // PLAIN
    snprintf(buf, BUF_FORMAT_LEN, "\\e[0;38;5;%d;48;5;%d;%dm", fg, bg, style);
#endif // OUTPUT_TERM
    buf[BUF_FORMAT_LEN - 1] = '\0';
    
    strncat(dest, buf, len);
}

/* generate segment separator 1 with color codes */
void al_separator_segment(char* dest, int len, int prev_bg, int next_bg) {
    char buf[BUF_FORMAT_LEN] = { 0 };
    al_color_esc(buf, BUF_FORMAT_LEN, prev_bg, next_bg, FNT_NORMAL);
    strncat(buf, SEPARATOR_SEGMENT, BUF_FORMAT_LEN - strlen(buf));
    buf[BUF_FORMAT_LEN - 1] = '\0';

    strncat(dest, buf, len);
}

/* generate segment separator 2 with color codes */
void al_separator_subsegment(char* dest, int len, int fg, int bg) {
    char buf[BUF_FORMAT_LEN] = { 0 };
    al_color_esc(dest, BUF_FORMAT_LEN, fg, bg, FNT_NORMAL);
    strncat(buf, SEPARATOR_PATH, BUF_FORMAT_LEN - strlen(buf));
    buf[BUF_FORMAT_LEN - 1] = '\0';
    
    strncat(dest, buf, len);
}

/* generate end separator with terminal color reset */
void al_separator_end(char** dest, unsigned int* maxlen, int bg) {
    char buf[BUF_FORMAT_LEN] = { 0 };
#if OUTPUT_FORMAT == OUTPUT_BASH
    snprintf(buf, BUF_FORMAT_LEN, "\\[\\e[0;38;5;%d;49;22m\\]%s\\[\\e[0m\\] ", bg, SEPARATOR_SEGMENT);
#elif OUTPUT_FORMAT == OUTPUT_ZSH
    snprintf(buf, BUF_FORMAT_LEN, "%%{\\e[0;38;5;%d;49;22m%%}%s%%{\\e[0m%%} ", bg, SEPARATOR_SEGMENT);
#else // PLAIN
    snprintf(buf, BUF_FORMAT_LEN, "\\e[0;38;5;%d;49;22m%s\\e[0m ", bg, SEPARATOR_SEGMENT);
#endif // OUTPUT_TERM
    buf[BUF_FORMAT_LEN - 1] = '\0';
    
    // add to promptline
    al_resize_char_buffer(dest, buf, maxlen, BUF_PROMPT_LEN);
    strncat(*dest, buf, *maxlen - strlen(*dest) + 1);
}

/* generate segment with color codes and separator at begining */
void al_gen_segment(char** dest, unsigned int* maxlen, int cur_fg, int cur_bg, int style, const char* text, int* is_first, int *last_bg) {
    char buf[BUF_PROMPT_LEN] = { 0 };

    if ((is_first != NULL) && (last_bg != NULL)) {
        // add separator if not first segment
        if (*is_first == 0) {
            al_separator_segment(buf, BUF_PROMPT_LEN, *last_bg, cur_bg);
        }
        *is_first = 0;
        *last_bg = cur_bg;
    }

    // add color escape
    al_color_esc(buf, BUF_PROMPT_LEN - strlen(buf), cur_fg, cur_bg, style);
    // add text
    strncat(buf, text, BUF_PROMPT_LEN - strlen(buf));
    buf[BUF_PROMPT_LEN - 1] = '\0';
    
    // add to promptline
    al_resize_char_buffer(dest, buf, maxlen, BUF_PROMPT_LEN);
    strncat(*dest, buf, *maxlen - strlen(*dest) + 1);
}

/* generate subsegment with color codes and path separator at begining */
void al_gen_subsegment(char** dest, unsigned int* maxlen, int cur_fg, int cur_bg, int sep_fg, int style, const char* text) {
    char buf[BUF_PROMPT_LEN] = { 0 };

    // add path separator
    al_separator_subsegment(buf, BUF_PROMPT_LEN, sep_fg, cur_bg);
    // add color escape
    al_color_esc(buf, BUF_PROMPT_LEN - strlen(buf), cur_fg, cur_bg, style);
    // add text
    strncat(buf, text, BUF_PROMPT_LEN - strlen(buf));
    buf[BUF_PROMPT_LEN - 1] = '\0';
    
    // add to promptline
    al_resize_char_buffer(dest, buf, maxlen, BUF_PROMPT_LEN);
    strncat(*dest, buf, *maxlen - strlen(*dest) + 1);
}


/**
 * SEGMENT GENERATORS: GATHER INFORMATION AND FORMAT ACCORDINGLY
 **/

/* show username and hostname with colorcodes for ROOT or SSH */
int al_segment_host(char** prompt, unsigned int* prompt_len, int* is_first, int* last_bg) {
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
    al_gen_segment(prompt, prompt_len, COLOR_FG_HOST, color, FNT_BOLD, text, is_first, last_bg);

    return 0;
}

/* show last exit status if command failed */
int al_segment_status(char** prompt, unsigned int* prompt_len, int* is_first, int* last_bg) {
    char text[8];

    // add segment to prompt buffer
    if (al_last_command_failed()) {
        snprintf(text, 8, " %s ", BOLD_X);
        al_gen_segment(prompt, prompt_len, COLOR_FG_STATUS, COLOR_BG_STATUS, FNT_BOLD, text, is_first, last_bg);
    }

    return 0;
}

/* show current working dir */
int al_segment_cwd(char** prompt, unsigned int* prompt_len, int* is_first, int* last_bg) {
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
                strcpy(dirs[i], THREE_DOTS); // deep in hirarchy
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
                al_gen_segment(prompt, prompt_len, COLOR_FG_CWD, COLOR_BG_CWD, style, text, is_first, last_bg);
                first_path = 0;
            } else {
                al_gen_subsegment(prompt, prompt_len, COLOR_FG_CWD, COLOR_BG_CWD, COLOR_SEP_CWD, style, text);
            }
        }
    }

    return 0;
}

#ifdef USE_VCS_GIT
/* show branch status for git repository */
int al_segment_git(char** prompt, unsigned int* prompt_len, int* is_first, int* last_bg) {
    char text[64];
    char cwd[512];
    char branch[32];

    char icon[4] = {0};
    int color = 0;
    
    git_repository* repo = NULL;
    
    if (al_get_cwd(cwd, 512) != 0) {
        return -1;
    }

    // get information from repository if exists
    if (al_git_open_repo(cwd, &repo) != 0) {
        return 1;
    }

    // get current branch name
    if (al_git_get_branch(branch, 32, repo) != 0) {
        git_repository_free(repo);
        return -1;
    }

    // change color and icon on branch state
    if (al_git_is_dirty()) {
        color = COLOR_BG_VCS_DIRTY;
        strncpy(icon, PLUSMINUS, 4);
    } else {
        color = COLOR_BG_VCS_CLEAN;
        strncpy(icon, BRANCH, 4);
    }

    // add segment to prompt buffer
    snprintf(text, 64, " %s %s ", icon, branch);
    al_gen_segment(prompt, prompt_len, COLOR_FG_VCS, color, FNT_BOLD, text, is_first, last_bg);

    git_repository_free(repo);
    return 0;
}
#endif // USE_VCS_GIT

/* show vcs status if cwd is part of a vcs */
int al_segment_vcs(char** prompt, unsigned int* prompt_len, int* is_first, int* last_bg) {
    int ret = 1;

#ifdef USE_VCS_GIT
    ret = al_segment_git(prompt, prompt_len, is_first, last_bg);
#endif // USE_VCS_GIT

    if (ret == 0 || ret == 1) {
        // generated or skipped segment
        return 0;
    } else {
        return -1; // error
    }
}
