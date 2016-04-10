/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: utils.c
 * v0.7.5 / 2016.04.10
 *
 * (c) 2016 Bernd Busse
 * The MIT License (MIT)
 **/

#include <limits.h>
#include <pwd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "utils.h"

/**
 * ENVIRONMENT HELPER FUNCTIONS AND WRAPPER
 **/

int last_exit_status = 0;
char *last_error = "";

/* copy date / time according to format string fmt into buf */
int al_get_datetime(char* buf, size_t len, const char* fmt) {
    time_t t = time(NULL);
    struct tm* timeinfo = localtime(&t);
    if (timeinfo == NULL) {
        return -1; // error
    }
    if (strftime(buf, len, fmt, timeinfo) == 0) {
        return -1; // possible error
    }
    buf[len-1] = '\0';
    return 0;
}

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

/* check if last command has failed */
int al_last_command_failed() {
    if (last_exit_status != 0) {
        return 1; // COMMAND FAILED
    }
    return 0; // NOT failed
}

/* check if str starts with pre */
int al_string_startswith(const char* str, const char* pre) {
    size_t len = strlen(pre);
    if (strlen(str) < len) {
        return 0; // compare string pre is too long
    }
    return strncmp(str, pre, len) == 0;
}

/* return directory level of path */
int al_get_dir_count(const char* path) {
    int c = 1;
    const char* occ = path;

    if (path == NULL) {
        return -1; // error
    }
    // check if path string is empty: level 0
    if (strlen(path) < 2) {
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
int al_get_dir_name(char* dest, size_t len, const char* path, int count) {
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

#ifdef USE_VCS_GIT
/* copy name of current git branch into buf */
int al_git_get_branch(char* buf, size_t len) {
    FILE *cmd_output;
    char branch_name[32] = { 0 };
    char *newline;

    cmd_output = popen("/usr/bin/git rev-parse --abbrev-ref HEAD 2>/dev/null", "r");
    if (cmd_output == NULL) {
        last_error = "cannot call /usr/bin/git";
        return -1; // error
    }

    if (fgets(branch_name, 32, cmd_output) == NULL) {
        if (pclose(cmd_output) == -1) {
            last_error = "'git rev-parse' failed";
            return -1; // error
        } else {
            return 1; // skip
        }
    }
    
    if (pclose(cmd_output) == -1) {
        return -1; // error
    }

    newline = strstr(branch_name, "\n");
    if (newline != NULL) {
        *newline = '\0';
    }

    strncpy(buf, branch_name, len);
    buf[len-1] = '\0';

    return 0;
}

/* check if current git repo is clean or dirty */
int al_git_is_dirty() {
    FILE *cmd_output;
    char modified_list[8] = { 0 };

    cmd_output = popen("/usr/bin/git status --ignore-submodules --porcelain 2>/dev/null", "r");
    if (cmd_output == NULL) {
        last_error = "cannot call /usr/bin/git";
        return -1; // error
    }

    if (fgets(modified_list, 8, cmd_output) == NULL) {
        if (pclose(cmd_output) != 0) {
            last_error = "'git status' failed";
            return -1; // error
        } else {
            return 0; // not modified
        }
    }

    if (strlen(modified_list) != 0) {
        return 1; // found modification
    } else {
        return 0;
    }
}
#endif // USE_VCS_GIT
