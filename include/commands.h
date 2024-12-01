#pragma once
#ifndef COMMANDS_H
#define COMMANDS_H

#include "config.h"
#include <sys/mman.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#define SHARED_SIZE 4096

// 시그널 관련 전역 변수와 함수
extern volatile sig_atomic_t data_ready;

#ifdef __cplusplus
extern "C" {
#endif

void handle_usr1(int signo);

struct ps_options {
    int long_format;
    int show_all;
};

struct ls_options {
    int sort_by_time;    // -t 옵션
    int sort_by_size;    // -S 옵션
    int reverse_sort;    // -r 옵션
    int show_all_times;  // -T 옵션
};

// 파일 정보를 위한 구조체
struct file_info {
    char name[MAX_PATH_SIZE];
    struct stat st;
};

// 모든 함수 선언을 여기로 이동
void remove_directory_recursive(const char *path);
void call_help(void);
void call_ls(const char *current_dir, const struct ls_options *opts);
void call_cd(const char *current_dir, const char *path, char *new_dir);
void call_mkdir(const char *current_dir, const char *path);
void call_rmdir(const char *current_dir, const char *path);
void call_rename(const char *current_dir, const char *source, const char *target);
void call_ln(const char *current_dir, const char *source, const char *target, int symbolic);
void call_rm(const char *current_dir, const char *path, int recursive);
void call_chmod(const char *current_dir, const char *path, const char *mode);
void call_cat(const char *current_dir, const char *path);
void call_cp(const char *current_dir, const char *source, const char *target);
void call_ps(const char *options);
void call_kill(const char *pid_str, const char *sig_str);
void call_mmap_test(const char *filename);
void execute_program(const char *program_name, char **args);
int setup_chroot(const char* path);

#ifdef __cplusplus
}
#endif 

#endif /* COMMANDS_H */ 