/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: utils.c
 * v0.4 / 2015.05.07
 *
 * (c) 2015 Bernd Busse
 **/

#ifndef _UTILS_H
#define _UTILS_H

#include <pwd.h>

/**
 * ENVIRONMENT HELPER FUNCTIONS AND WRAPPER
 **/

extern int last_exit_status;

/* copy path of current working directory into buf */
int al_get_cwd(char* buf, size_t len);

/* return directory level of path */
int al_get_dir_count(char* path);

/* write directory name count levels obove path into buf */
int al_get_dir_name(char* dest, size_t len, char* path, int count);

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

/* check if last command has failed */
int al_last_command_failed();

/* check if str starts with pre */
int al_string_startswith(char* str, const char* pre);

#endif // _UTILS_H
