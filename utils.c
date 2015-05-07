/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: utils.c
 * v0.3 / 2015.05.07
 *
 * (c) 2015 Bernd Busse
 **/

#include <limits.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "utils.h"

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

int al_last_command_failed() {
    return 0; // NOT failed
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
    int c = 1;
    char* occ = path;

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
