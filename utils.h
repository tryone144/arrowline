/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: utils.h
 * v0.7.5 / 2016.04.10
 *
 * (c) 2016 Bernd Busse
 * The MIT License (MIT)
 **/

#ifndef _UTILS_H
#define _UTILS_H

#include <pwd.h>

/**
 * ENVIRONMENT HELPER FUNCTIONS AND WRAPPER
 **/

extern int last_exit_status;
extern char* last_error;

/* copy path of current working directory into buf */
int al_get_cwd(char* buf, size_t len);

/* copy date / time according to format string fmt into buf */
int al_get_datetime(char* buf, size_t len, const char* fmt);

/* return directory level of path */
int al_get_dir_count(const char* path);

/* write directory name count levels obove path into buf */
int al_get_dir_name(char* dest, size_t len, const char* path, int count);

/* copy path of users home directory into buf */
int al_get_home(char* buf, size_t len);

/* copy hostname from environment into buf */
int al_get_hostname(char* buf, size_t len);

/* copy username from environment into buf */
int al_get_username(char* buf, size_t len);

/* check if session is running over SSH */
int al_is_ssh_connection();

/* check if user is root */
int al_is_root_session();

#ifdef USE_VCS_GIT
    /* copy name of current git branch into buf */
    int al_git_get_branch(char* buf, size_t len);

    /* check if current git repo is clean or dirty */
    int al_git_is_dirty();
#endif // USE_VCS_GIT

/* check if last command has failed */
int al_last_command_failed();

/* check if str starts with pre */
int al_string_startswith(const char* str, const char* pre);

#endif // _UTILS_H
