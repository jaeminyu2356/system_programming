#pragma once
#ifndef UTILS_H
#define UTILS_H

#include "config.h"
#include <time.h>
#include <signal.h>
#include <sys/stat.h>

// 유틸리티 함수 선언
void print_permissions(struct stat *file_stat);
void print_time(const time_t *time);
int is_within_base_dir(const char *path);
void get_absolute_path(const char *base, const char *path, char *result);
int setup_chroot(const char *path);
void print_user_group(uid_t uid, gid_t gid);
void print_all_times(const struct stat *st);
mode_t parse_mode_str(const char *mode_str, mode_t current_mode);

#endif // UTILS_H 