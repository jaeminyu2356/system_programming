#include "../include/commands.h"
#include "../include/utils.h"
#include "../include/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>

char current_dir[MAX_PATH_SIZE] = BASE_DIR;

// 전역 변수로 추가
volatile sig_atomic_t interrupted = 0;

// main.c에 핸들러 함수 추가
void handle_sigint(int signo) {
    interrupted = 1;
    printf("\n종료하려면 'exit' 또는 'quit'를 입력하세요\n");
    printf("%s $ ", current_dir);
    fflush(stdout);
}

int main(int argc, char **argv) {
    // 기본 디렉토리 생성
    mkdir(BASE_DIR, DEFAULT_DIR_MODE);
    
    // chroot 설정
    if (ENABLE_CHROOT) {
        if (setup_chroot(BASE_DIR) != 0) {
            fprintf(stderr, "Failed to setup chroot environment\n");
            return 1;
        }
    }

    // 시그널 핸들러 설정
    if (ENABLE_SIGNALS) {
        struct sigaction sa;
        
        // SIGUSR1 핸들러 설정
        sa.sa_handler = handle_usr1;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(SIGUSR1, &sa, NULL);
        
        // SIGINT 핸들러 설정
        sa.sa_handler = handle_sigint;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(SIGINT, &sa, NULL);
    }

    char *command = (char *)malloc(MAX_CMD_SIZE);
    if (!command) {
        perror("malloc");
        exit(1);
    }

    while (1) {
        interrupted = 0;
        printf("%s $ ", current_dir);
        
        if (!fgets(command, MAX_CMD_SIZE - 1, stdin)) {
            if (interrupted) {
                clearerr(stdin);
                continue;
            }
            break;
        }

        char *tok_str = strtok(command, " \n");
        if (!tok_str) continue;

        if (strcmp(tok_str, "quit") == 0 || strcmp(tok_str, "exit") == 0) {
            break;
        } else if (strcmp(tok_str, "help") == 0) {
            call_help();
        } else if (strcmp(tok_str, "ls") == 0) {
            struct ls_options opts = {0};
            char *option;
            while ((option = strtok(NULL, " \n"))) {
                if (strcmp(option, "-t") == 0) opts.sort_by_time = 1;
                else if (strcmp(option, "-S") == 0) opts.sort_by_size = 1;
                else if (strcmp(option, "-r") == 0) opts.reverse_sort = 1;
                else if (strcmp(option, "-T") == 0) opts.show_all_times = 1;
            }
            call_ls(current_dir, &opts);
        } else if (strcmp(tok_str, "cd") == 0) {
            tok_str = strtok(NULL, " \n");
            if (tok_str) {
                char new_dir[MAX_PATH_SIZE];
                call_cd(current_dir, tok_str, new_dir);
                strcpy(current_dir, new_dir);
            } else {
                printf("cd: 인자가 누락되었습니다\n");
            }
        } else if (strcmp(tok_str, "mkdir") == 0) {
            tok_str = strtok(NULL, " \n");
            if (tok_str) {
                call_mkdir(current_dir, tok_str);
            } else {
                printf("mkdir: 인자가 누락되었습니다\n");
            }
        } else if (strcmp(tok_str, "rmdir") == 0) {
            tok_str = strtok(NULL, " \n");
            if (tok_str) {
                call_rmdir(current_dir, tok_str);
            } else {
                printf("rmdir: 인자가 누락되었습니다\n");
            }
        } else if (strcmp(tok_str, "rename") == 0) {
            char *source = strtok(NULL, " \n");
            char *target = strtok(NULL, " \n");
            if (source && target) {
                call_rename(current_dir, source, target);
            } else {
                printf("rename: 인자가 누락되었습니다\n");
            }
        } else if (strcmp(tok_str, "ln") == 0) {
            int symbolic = 0;
            tok_str = strtok(NULL, " \n");
            if (tok_str && strcmp(tok_str, "-s") == 0) {
                symbolic = 1;
                tok_str = strtok(NULL, " \n");
            }
            char *target = strtok(NULL, " \n");
            if (tok_str && target) {
                call_ln(current_dir, tok_str, target, symbolic);
            } else {
                printf("ln: 인자가 누락되었습니다\n");
            }
        } else if (strcmp(tok_str, "rm") == 0) {
            int recursive = 0;
            tok_str = strtok(NULL, " \n");
            
            if (tok_str && strcmp(tok_str, "-r") == 0) {
                recursive = 1;
                tok_str = strtok(NULL, " \n");
            }
            
            if (tok_str) {
                call_rm(current_dir, tok_str, recursive);
            } else {
                printf("rm: 인자가 누락되었습니다\n");
                printf("사용법: rm [-r] <파일/디렉토리>\n");
            }
        } else if (strcmp(tok_str, "chmod") == 0) {
            char *mode = strtok(NULL, " \n");
            char *path = strtok(NULL, " \n");
            if (mode && path) {
                call_chmod(current_dir, path, mode);
            } else {
                printf("chmod: 인자가 누락되었습니다\n");
            }
        } else if (strcmp(tok_str, "cat") == 0) {
            tok_str = strtok(NULL, " \n");
            if (tok_str) {
                call_cat(current_dir, tok_str);
            } else {
                printf("cat: 인자가 누락되었습니다\n");
            }
        } else if (strcmp(tok_str, "cp") == 0) {
            char *source = strtok(NULL, " \n");
            char *target = strtok(NULL, " \n");
            if (source && target) {
                call_cp(current_dir, source, target);
            } else {
                printf("cp: 인자가 누락되었습니다\n");
            }
        } else if (strcmp(tok_str, "ps") == 0) {
            char *options = strtok(NULL, "\n");
            call_ps(options);
        } else if (strcmp(tok_str, "kill") == 0) {
            char *pid = strtok(NULL, " \n");
            char *sig = strtok(NULL, " \n");
            if (pid) {
                call_kill(pid, sig);
            } else {
                printf("kill: 인자가 누락되었습니다\n");
            }
        } else if (strcmp(tok_str, "mmap_test") == 0) {
            tok_str = strtok(NULL, " \n");
            if (tok_str) {
                call_mmap_test(tok_str);
            } else {
                printf("mmap_test: 파일 이름을 지정해주세요\n");
            }
        } else {
            // 외부 프로그램 실행
            char *args[MAX_CMD_SIZE/2];
            int i = 0;
            args[i++] = tok_str;
            while ((tok_str = strtok(NULL, " \n")) && i < MAX_CMD_SIZE/2 - 1) {
                args[i++] = tok_str;
            }
            args[i] = NULL;
            execute_program(args[0], args);
        }
    }

    free(command);
    return 0;
}
