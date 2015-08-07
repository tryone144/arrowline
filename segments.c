/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: segments.c
 * v0.7 / 2015.08.07
 *
 * (c) 2015 Bernd Busse
 * The MIT License (MIT)
 **/

#include <stdio.h>
#include <string.h>

#include "config-prompt.h"
#include "config-status.h"
#include "renderer.h"
#include "utils.h"

#include "segments.h"

/**
 * SEGMENT GENERATORS: GATHER INFORMATION AND FORMAT ACCORDINGLY
 **/

/* show date and time */
int al_segment_datetime(char** dest, unsigned int* maxlen, int* is_first, int* last_bg, int position) {
    char text[64];
    int color_fg = 0;
    int color_bg = 2;

    char date[32];
    char time[32];

    //al_get_date(date, 32);
    //al_get_time(time, 32);

    // add segment to prompt buffer
    snprintf(text, 64, " %s-%s ", date, time);
    al_gen_segment(dest, maxlen, color_fg, color_bg, FNT_NORMAL, text, is_first, last_bg, position);

    return 0;
}

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
