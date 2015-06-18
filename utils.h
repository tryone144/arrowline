/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: utils.c
 * v0.5 / 2015.06.18
 *
 * (c) 2015 Bernd Busse
 **/

#ifndef _UTILS_H
#define _UTILS_H

#include <pwd.h>
#include <git2.h>

/**
 * ENVIRONMENT HELPER FUNCTIONS AND WRAPPER
 **/

extern int last_exit_status;

/* copy path of current working directory into buf */
int al_get_cwd(char* buf, size_t len);

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

// TODO: Documentation
int al_git_open_repo(const char* path, git_repository** repo);
int al_git_get_branch(char* buf, size_t len, git_repository* repo);
int al_git_is_dirty(git_repository* repo);

/* check if last command has failed */
int al_last_command_failed();

/* check if str starts with pre */
int al_string_startswith(const char* str, const char* pre);

#endif // _UTILS_H
