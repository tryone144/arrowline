/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: segments.c
 * v0.6.4 / 2015.07.18
 *
 * (c) 2015 Bernd Busse
 * The MIT License (MIT)
 **/

#include <stdio.h>
#include <string.h>

#include "config-prompt.h"
#include "config-status.h"
#include "utils.h"

#include "segments.h"

#define FNT_BOLD 1
#define FNT_NORMAL 22

#ifdef USE_POWERLINE_SYMBOLS
    #define SEPARATOR_SEGMENT_RIGHT ""     // \uE0B0
    #define SEPARATOR_SEGMENT_LEFT ""     // \uE0B2
    #define SEPARATOR_PATH_RIGHT ""        // \uE0B1
    #define SEPARATOR_PATH_LEFT ""        // \uE0B3
#else
    #define SEPARATOR_SEGMENT_RIGHT ""
    #define SEPARATOR_SEGMENT_LEFT ""
    #define SEPARATOR_PATH_RIGHT "/"
    #define SEPARATOR_PATH_LEFT "\\"
#endif

#define THREE_DOTS "⋯"          // \u22EF
#define BOLD_X "✘"              // \u2718
#define BRANCH ""              // \uE0A0
#define PLUSMINUS "±"           // \u00B1

/**
 * COLOR ESCAPESEQUENCES AND SEGMENT GENERATION
 **/

/* escape ANSI color codes for shell prompt */
void al_ansi_escape_color(char* dest, int maxlen, int fg, int bg, int style) {
    char buf[BUF_FORMAT_LEN] = { 0 };
#if PROMPT_OUTPUT_FORMAT == OUTPUT_BASH
    snprintf(buf, BUF_FORMAT_LEN, "\\[\\e[0;38;5;%d;48;5;%d;%dm\\]", fg, bg, style);
#elif PROMPT_OUTPUT_FORMAT == OUTPUT_ZSH
    snprintf(buf, BUF_FORMAT_LEN, "%%{\\e[0;38;5;%d;48;5;%d;%dm%%}", fg, bg, style);
#else // PLAIN
    snprintf(buf, BUF_FORMAT_LEN, "\\e[0;38;5;%d;48;5;%d;%dm", fg, bg, style);
#endif // OUTPUT_TERM
    
    al_string_cat(dest, buf, maxlen);
}

/* escape ANSI fg color and reset bg color for shell prompt (end segment) */
void al_ansi_escape_bg_reset(char* dest, int maxlen, int bg) {
    char buf[BUF_FORMAT_LEN] = { 0 };
#if PROMPT_OUTPUT_FORMAT == OUTPUT_BASH
    snprintf(buf, BUF_FORMAT_LEN, "\\[\\e[0;38;5;%d;49;22m\\]", bg);
#elif PROMPT_OUTPUT_FORMAT == OUTPUT_ZSH
    snprintf(buf, BUF_FORMAT_LEN, "%%{\\e[0;38;5;%d;49;22m%%}", bg);
#else // PLAIN
    snprintf(buf, BUF_FORMAT_LEN, "\\e[0;38;5;%d;49;22m", bg);
#endif // OUTPUT_TERM
    
    al_string_cat(dest, buf, maxlen);
}

/* reset ANSI color codes for shell prompt */
void al_ansi_escape_reset(char* dest, int maxlen) {
    char buf[BUF_FORMAT_LEN] = { 0 };
#if PROMPT_OUTPUT_FORMAT == OUTPUT_BASH
    snprintf(buf, BUF_FORMAT_LEN, "\\[\\e[0m\\]");
#elif PROMPT_OUTPUT_FORMAT == OUTPUT_ZSH
    snprintf(buf, BUF_FORMAT_LEN, "%%{\\e[0m%%}");
#else // PLAIN
    snprintf(buf, BUF_FORMAT_LEN, "\\e[0m");
#endif // OUTPUT_TERM

    al_string_cat(dest, buf, maxlen);
}

/* generate segment separator 1 with color codes */
void al_separator_segment(char* dest, int maxlen, int prev_bg, int next_bg, int position) {
    char buf[BUF_FORMAT_LEN] = { 0 };
    if (position != POSITION_RIGHT) {
        al_ansi_escape_color(buf, BUF_FORMAT_LEN, prev_bg, next_bg, FNT_NORMAL);
        al_string_cat(buf, SEPARATOR_SEGMENT_RIGHT, BUF_FORMAT_LEN);
    }
    if (position != POSITION_LEFT) {
        al_ansi_escape_color(buf, BUF_FORMAT_LEN, next_bg, prev_bg, FNT_NORMAL);
        al_string_cat(buf, SEPARATOR_SEGMENT_LEFT, BUF_FORMAT_LEN);
    }

    al_string_cat(dest, buf, maxlen);
}

/* generate segment separator 2 with color codes */
void al_separator_subsegment(char* dest, int maxlen, int fg, int bg, int position) {
    char buf[BUF_FORMAT_LEN] = { 0 };
    char *sep = position ? SEPARATOR_PATH_LEFT : SEPARATOR_PATH_RIGHT;
    al_ansi_escape_color(buf, BUF_FORMAT_LEN, fg, bg, FNT_NORMAL);
    al_string_cat(buf, sep, BUF_FORMAT_LEN);
    
    al_string_cat(dest, buf, maxlen);
}

/* generate start separator */
void al_segment_start(char* dest, int maxlen, int bg, int position) {
    char buf[BUF_FORMAT_LEN] = { 0 };
    if (position != POSITION_LEFT) {
        al_ansi_escape_bg_reset(buf, BUF_FORMAT_LEN, bg);
        al_string_cat(buf, SEPARATOR_SEGMENT_LEFT, BUF_FORMAT_LEN);
    }
    
    al_string_cat(dest, buf, maxlen);
}

/* generate end separator with color reset */
void al_segment_end(char** dest, unsigned int* maxlen, int bg, int position) {
    char buf[BUF_FORMAT_LEN] = { 0 };
    if (position != POSITION_RIGHT) {
        al_ansi_escape_bg_reset(buf, BUF_FORMAT_LEN, bg);
        al_string_cat(buf, SEPARATOR_SEGMENT_RIGHT, BUF_FORMAT_LEN);
    }
    al_ansi_escape_reset(buf, BUF_FORMAT_LEN);
    
    // add to promptline
    al_resize_char_buffer(dest, buf, maxlen, BUF_PROMPT_LEN);
    al_string_cat(*dest, buf, *maxlen);
}

/* generate segment with color codes and separator at begining */
void al_gen_segment(char** dest, unsigned int* maxlen, int cur_fg, int cur_bg, int style, const char* text, int* is_first, int *last_bg, int position) {
    char buf[BUF_PROMPT_LEN] = { 0 };

    if ((is_first != NULL) && (last_bg != NULL)) {
        // add separator if not first segment
        if (*is_first == 1) {
            al_segment_start(buf, BUF_PROMPT_LEN, cur_bg, position);
        } else {
            al_separator_segment(buf, BUF_PROMPT_LEN, *last_bg, cur_bg, position);
        }
        *is_first = 0;
        *last_bg = cur_bg;
    }

    // add color escape
    al_ansi_escape_color(buf, BUF_PROMPT_LEN, cur_fg, cur_bg, style);
    // add text
    al_string_cat(buf, text, BUF_PROMPT_LEN);
    
    // add to promptline
    al_resize_char_buffer(dest, buf, maxlen, BUF_PROMPT_LEN);
    al_string_cat(*dest, buf, *maxlen);
}

/* generate subsegment with color codes and path separator at begining */
void al_gen_subsegment(char** dest, unsigned int* maxlen, int cur_fg, int cur_bg, int sep_fg, int style, const char* text, int position) {
    char buf[BUF_PROMPT_LEN] = { 0 };

    // add path separator
    al_separator_subsegment(buf, BUF_PROMPT_LEN, sep_fg, cur_bg, position);
    // add color escape
    al_ansi_escape_color(buf, BUF_PROMPT_LEN, cur_fg, cur_bg, style);
    // add text
    al_string_cat(buf, text, BUF_PROMPT_LEN);
    
    // add to promptline
    al_resize_char_buffer(dest, buf, maxlen, BUF_PROMPT_LEN);
    al_string_cat(*dest, buf, *maxlen);
}


/**
 * SEGMENT GENERATORS: GATHER INFORMATION AND FORMAT ACCORDINGLY
 **/

/* show username and hostname with colorcodes for ROOT or SSH */
int al_segment_host(char** prompt, unsigned int* prompt_len, int* is_first, int* last_bg, int position) {
    char text[64];
    int color_fg = COLOR_FG_HOST_USER;
    int color_bg = COLOR_BG_HOST_USER;

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
        color_fg = COLOR_FG_HOST_SSH;
        color_bg = COLOR_BG_HOST_SSH;
    } else if (al_is_root_session()) {
        color_fg = COLOR_FG_HOST_ROOT;
        color_bg = COLOR_BG_HOST_ROOT;
    }

    // add segment to prompt buffer
    snprintf(text, 64, " %s@%s ", username, hostname);
    al_gen_segment(prompt, prompt_len, color_fg, color_bg, FNT_BOLD, text, is_first, last_bg, position);

    return 0;
}

/* show last exit status if command failed */
int al_segment_status(char** prompt, unsigned int* prompt_len, int* is_first, int* last_bg, int position) {
    char text[8];

    // add segment to prompt buffer
    if (al_last_command_failed()) {
        snprintf(text, 8, " %s ", BOLD_X);
        al_gen_segment(prompt, prompt_len, COLOR_FG_STATUS, COLOR_BG_STATUS, FNT_BOLD, text, is_first, last_bg, position);
    }

    return 0;
}

/* show current working dir */
int al_segment_cwd(char** prompt, unsigned int* prompt_len, int* is_first, int* last_bg, int position) {
    char dirs[CWD_LEN][64];

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
    for (int i = 0; i < CWD_LEN; i++) {
        al_get_dir_name(dirs[i], 64, base, i);
    }

    char text[66];
    int style;
    int first_path = 1;

    for (int i = CWD_LEN - 1; i >= 0; i--) {
        if (strlen(dirs[i]) != 0) {
            // render path
            snprintf(text, 66, " %s ", dirs[i]);

            // add segment to prompt buffer
            style = i == 0 ? FNT_BOLD : FNT_NORMAL;
            if (first_path) {
                al_gen_segment(prompt, prompt_len, COLOR_FG_CWD, COLOR_BG_CWD, style, text, is_first, last_bg, position);
                first_path = 0;
            } else {
                al_gen_subsegment(prompt, prompt_len, COLOR_FG_CWD, COLOR_BG_CWD, COLOR_SEP_CWD, style, text, position);
            }
        }
    }

    return 0;
}

/* show current working dir prefix */
int al_segment_cwd_prefix(char** prompt, unsigned int* prompt_len, int* is_first, int* last_bg, int position) {
    char text[16];

    char prefix[16];
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

    // copy first path element into dirs buffer
    if (al_get_dir_count(base) > CWD_LEN) {
        strcpy(prefix, THREE_DOTS); // deep in hirarchy
    } else if (al_string_startswith(cwd, home)) {
        strcpy(prefix, "~"); // home directory
    } else {
        strcpy(prefix, "/"); // everywhere else
    }

    // add segment to prompt buffer
    snprintf(text, 16, " %s ", prefix);
    al_gen_segment(prompt, prompt_len, COLOR_FG_CWD_PREFIX, COLOR_BG_CWD_PREFIX, FNT_BOLD, text, is_first, last_bg, position);

    return 0;
}

#ifdef USE_VCS_GIT
/* show branch status for git repository */
int al_segment_git(char** prompt, unsigned int* prompt_len, int* is_first, int* last_bg, int position) {
    char text[64];
    char cwd[512];
    char branch[32];

    char icon[4] = {0};
    int color_fg = 0;
    int color_bg = 0;
    
    git_repository* repo = NULL;
    int repo_dirty = 0;
    
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
    git_repository_free(repo);

    // change color and icon on branch state
    if ((repo_dirty = al_git_is_dirty()) == -1) {
            return -1;
    }

    if (repo_dirty) {
        color_fg = COLOR_FG_VCS_DIRTY;
        color_bg = COLOR_BG_VCS_DIRTY;
        strncpy(icon, PLUSMINUS, 4);
    } else {
        color_fg = COLOR_FG_VCS_CLEAN;
        color_bg = COLOR_BG_VCS_CLEAN;
        strncpy(icon, BRANCH, 4);
    }

    // add segment to prompt buffer
    snprintf(text, 64, " %s %s ", icon, branch);
    al_gen_segment(prompt, prompt_len, color_fg, color_bg, FNT_BOLD, text, is_first, last_bg, position);

    return 0;
}
#endif // USE_VCS_GIT

/* show vcs status if cwd is part of a vcs */
int al_segment_vcs(char** prompt, unsigned int* prompt_len, int* is_first, int* last_bg, int position) {
    int ret = 1;

#ifdef USE_VCS_GIT
    ret = al_segment_git(prompt, prompt_len, is_first, last_bg, position);
#endif // USE_VCS_GIT

    if (ret == 0 || ret == 1) {
        // generated or skipped segment
        return 0;
    } else {
        return -1; // error
    }
}
