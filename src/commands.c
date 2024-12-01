#include "../include/commands.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <pwd.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <sys/mman.h>
#include <ctype.h>
#include <signal.h>

volatile sig_atomic_t data_ready = 0;

void handle_usr1(int signo) {
    (void)signo;  // 경고를 방지하기 위해 unused parameter를 void로 캐스팅
    data_ready = 1;
}

void call_help(void) {
    printf("사용 가능한 명령어:\n");
    printf("  ls       - 디렉토리 내용 표시\n");
    printf("  cd       - 디렉토리 변경\n");
    printf("  mkdir    - 디렉토리 생성\n");
    printf("  rmdir    - 디렉토리 삭제\n");
    printf("  rename   - 파일 또는 디렉토리 이름 변경\n");
    printf("  ln       - 링크 생성\n");
    printf("  rm       - 파일 삭제\n");
    printf("  chmod    - 파일 권한 변경\n");
    printf("  cat      - 파일 내용 표시\n");
    printf("  cp       - 파일 복사\n");
    printf("  ps       - 프로세스 상태 표시\n");
    printf("  kill     - 프로세스에 시그널 전송\n");
    printf("  exit     - 쉘 종료\n");
}

int compare_by_name(const void *a, const void *b) {
    return strcmp(((struct file_info *)a)->name, ((struct file_info *)b)->name);
}

int compare_by_time(const void *a, const void *b) {
    return ((struct file_info *)b)->st.st_mtime - ((struct file_info *)a)->st.st_mtime;
}

int compare_by_size(const void *a, const void *b) {
    return ((struct file_info *)b)->st.st_size - ((struct file_info *)a)->st.st_size;
}


void call_ls(const char *current_dir, const struct ls_options *opts) {
    DIR *d = opendir(current_dir);
    if (!d) {
        perror(current_dir);
        return;
    }

    struct file_info *files = NULL;
    int count = 0;
    struct dirent *ent;

    // 파일 정보 수집
    while ((ent = readdir(d))) {
        files = realloc(files, (count + 1) * sizeof(struct file_info));
        char path[MAX_PATH_SIZE];
        snprintf(path, sizeof(path), "%s/%s", current_dir, ent->d_name);
        
        if (lstat(path, &files[count].st) == -1)
            continue;
            
        strcpy(files[count].name, ent->d_name);
        count++;
    }

    // 정렬
    if (opts->sort_by_time)
        qsort(files, count, sizeof(struct file_info), compare_by_time);
    else if (opts->sort_by_size)
        qsort(files, count, sizeof(struct file_info), compare_by_size);
    else
        qsort(files, count, sizeof(struct file_info), compare_by_name);

    if (opts->reverse_sort) {
        // 정렬 결과 뒤집기
        for (int i = 0; i < count/2; i++) {
            struct file_info temp = files[i];
            files[i] = files[count-1-i];
            files[count-1-i] = temp;
        }
    }

    // 출력
    for (int i = 0; i < count; i++) {
        print_permissions(&files[i].st);
        printf(" %ld", (long)files[i].st.st_nlink);
        print_user_group(files[i].st.st_uid, files[i].st.st_gid);
        printf(" %ld", (long)files[i].st.st_size);
        
        if (opts->show_all_times)
            print_all_times(&files[i].st);
        else
            print_time(&files[i].st.st_mtime);

        printf(" %s", files[i].name);
        
        // 심볼릭 링크인 경우 링크 내용 표시
        if (S_ISLNK(files[i].st.st_mode)) {
            char link_path[MAX_PATH_SIZE];
            char abs_path[MAX_PATH_SIZE];
            snprintf(abs_path, sizeof(abs_path), "%s/%s", current_dir, files[i].name);
            ssize_t len = readlink(abs_path, link_path, sizeof(link_path) - 1);
            if (len != -1) {
                link_path[len] = '\0';
                printf(" -> %s", link_path);
            }
        }
        printf("\n");
    }

    free(files);
    closedir(d);
}

void call_cd(const char *current_dir, const char *path, char *new_dir) {
    char abs_path[MAX_PATH_SIZE];
    get_absolute_path(current_dir, path, abs_path);

    if (!is_within_base_dir(abs_path)) {
        printf("오류: %s 외부의 디렉토리에 접근할 수 없습니다\n", BASE_DIR);
        strcpy(new_dir, current_dir);
        return;
    }

    if (chdir(abs_path) == 0) {
        strcpy(new_dir, abs_path);
    } else {
        perror(abs_path);
        strcpy(new_dir, current_dir);
    }
}

void call_mkdir(const char *current_dir, const char *path) {
    char abs_path[MAX_PATH_SIZE];
    get_absolute_path(current_dir, path, abs_path);

    if (!is_within_base_dir(abs_path)) {
        printf("오류: %s 외부에 디렉토리를 생성할 수 없습니다\n", BASE_DIR);
        return;
    }

    if (mkdir(abs_path, DEFAULT_DIR_MODE) < 0) {
        perror(abs_path);
    }
}

void call_rmdir(const char *current_dir, const char *path) {
    char abs_path[MAX_PATH_SIZE];
    get_absolute_path(current_dir, path, abs_path);

    if (!is_within_base_dir(abs_path)) {
        printf("오류: %s 외부의 디렉토리를 삭제할 수 없습니다\n", BASE_DIR);
        return;
    }

    if (rmdir(abs_path) < 0) {
        perror(abs_path);
    }
}

void call_rename(const char *current_dir, const char *source, const char *target) {
    char abs_source[MAX_PATH_SIZE], abs_target[MAX_PATH_SIZE];
    get_absolute_path(current_dir, source, abs_source);
    get_absolute_path(current_dir, target, abs_target);

    if (!is_within_base_dir(abs_source) || !is_within_base_dir(abs_target)) {
        printf("오류: %s 외부의 파일 이름을 변경할 수 없습니다\n", BASE_DIR);
        return;
    }

    if (rename(abs_source, abs_target) < 0) {
        perror("rename");
    }
}

void call_ln(const char *current_dir, const char *original, const char *new_link, int symbolic) {
    char abs_original[MAX_PATH_SIZE], abs_new_link[MAX_PATH_SIZE];
    get_absolute_path(current_dir, original, abs_original);
    get_absolute_path(current_dir, new_link, abs_new_link);

    if (!is_within_base_dir(abs_original) || !is_within_base_dir(abs_new_link)) {
        printf("오류: %s 외부에 링크를 생성할 수 없습니다\n", BASE_DIR);
        return;
    }

    if (symbolic) {
        if (symlink(abs_original, abs_new_link) < 0) {
            perror("symlink");
        }
    } else {
        if (link(abs_original, abs_new_link) < 0) {
            perror("link");
        }
    }
}

void call_rm(const char *current_dir, const char *path, int recursive) {
    char abs_path[MAX_PATH_SIZE];
    get_absolute_path(current_dir, path, abs_path);

    if (!is_within_base_dir(abs_path)) {
        printf("오류: %s 외부의 파일을 삭제할 수 없습니다\n", BASE_DIR);
        return;
    }

    struct stat st;
    if (lstat(abs_path, &st) == -1) {
        perror(abs_path);
        return;
    }

    if (S_ISDIR(st.st_mode)) {
        if (!recursive) {
            printf("rm: %s: 디렉토리입니다. -r 옵션을 사용하세요\n", path);
            return;
        }
        remove_directory_recursive(abs_path);
    } else {
        if (unlink(abs_path) == -1) {
            perror(abs_path);
        }
    }
}

void call_chmod(const char *current_dir, const char *path, const char *mode_str) {
    char abs_path[MAX_PATH_SIZE];
    get_absolute_path(current_dir, path, abs_path);

    if (!is_within_base_dir(abs_path)) {
        printf("오류: %s 외부의 파일 권한을 변경할 수 없습니다\n", BASE_DIR);
        return;
    }

    struct stat st;
    if (stat(abs_path, &st) < 0) {
        perror("stat");
        return;
    }

    mode_t new_mode = parse_mode_str(mode_str, st.st_mode);
    if (chmod(abs_path, new_mode) < 0) {
        perror("chmod");
    }
}

void call_cat(const char *current_dir, const char *path) {
    char abs_path[MAX_PATH_SIZE];
    get_absolute_path(current_dir, path, abs_path);

    if (!is_within_base_dir(abs_path)) {
        printf("오류: %s 외부의 파일을 읽을 수 없습니다\n", BASE_DIR);
        return;
    }

    FILE *fp = fopen(abs_path, "r");
    if (!fp) {
        perror(abs_path);
        return;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp)) {
        printf("%s", buffer);
    }

    fclose(fp);
}

void call_cp(const char *current_dir, const char *path, const char *target) {
    char abs_path[MAX_PATH_SIZE], abs_target[MAX_PATH_SIZE];
    get_absolute_path(current_dir, path, abs_path);
    get_absolute_path(current_dir, target, abs_target);

    if (!is_within_base_dir(abs_path) || !is_within_base_dir(abs_target)) {
        printf("오류: %s 외부의 파일을 복사할 수 없습니다\n", BASE_DIR);
        return;
    }

    FILE *src = fopen(abs_path, "r");
    if (!src) {
        perror(abs_path);
        return;
    }

    FILE *dst = fopen(abs_target, "w");
    if (!dst) {
        perror(abs_target);
        fclose(src);
        return;
    }

    char buffer[1024];
    size_t n;
    while ((n = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, n, dst);
    }

    fclose(src);
    fclose(dst);
}

void call_ps(const char *options) {
    if (!ENABLE_PS) {
        printf("PS 명령어가 비활성화되어 있습니다\n");
        return;
    }

    struct ps_options ps_opts = {0, 0};
    
    // 옵션 파싱
    if (options) {
        char *opt_copy = strdup(options);
        char *token = strtok(opt_copy, " ");
        while (token) {
            if (strcmp(token, "-a") == 0) {
                ps_opts.show_all = 1;
            } else if (strcmp(token, "-l") == 0) {
                ps_opts.long_format = 1;
            }
            token = strtok(NULL, " ");
        }
        free(opt_copy);
    }

    DIR *dir = opendir("/proc");
    if (!dir) {
        perror("opendir");
        return;
    }

    // 헤더 출력
    if (ps_opts.long_format) {
        printf("  PID   PPID  PGRP   UID   GID    VSZ    RSS STATE CMD\n");
    } else {
        printf("  PID   TTY          TIME CMD\n");
    }

    struct dirent *entry;
    while ((entry = readdir(dir))) {
        if (!isdigit(*entry->d_name)) continue;

        // 프로세스 ID가 아닌 경우 건너뛰기
        pid_t pid = atoi(entry->d_name);
        if (pid == 0) continue;

        char stat_path[MAX_PATH_SIZE];
        snprintf(stat_path, sizeof(stat_path), "/proc/%s/stat", entry->d_name);
        
        FILE *f = fopen(stat_path, "r");
        if (!f) continue;

        // stat 파일 파싱
        char state;
        int ppid, pgrp;
        unsigned long vsize, rss;
        char comm[MAX_CMD_SIZE];
        
        fscanf(f, "%*d %s %c %d %d", comm, &state, &ppid, &pgrp);
        fclose(f);

        // 프로세스 소유자 정보 얻기
        char proc_path[MAX_PATH_SIZE];
        snprintf(proc_path, sizeof(proc_path), "/proc/%s", entry->d_name);
        struct stat st;
        if (stat(proc_path, &st) == -1) continue;

        // -a 옵션이 없으면 자신의 프로세스만 표시
        if (!ps_opts.show_all && st.st_uid != getuid()) continue;

        // cmdline 읽기
        char cmdline[MAX_CMD_SIZE] = {0};
        snprintf(proc_path, sizeof(proc_path), "/proc/%s/cmdline", entry->d_name);
        f = fopen(proc_path, "r");
        if (f) {
            size_t n = fread(cmdline, 1, sizeof(cmdline)-1, f);
            fclose(f);
            if (n > 0) {
                cmdline[n] = '\0';
                for (size_t i = 0; i < n; i++) {
                    if (cmdline[i] == '\0') cmdline[i] = ' ';
                }
            }
        }
        if (cmdline[0] == '\0') {
            strncpy(cmdline, comm, sizeof(cmdline)-1);
        }

        // 메모리 정보 읽기
        snprintf(proc_path, sizeof(proc_path), "/proc/%s/statm", entry->d_name);
        f = fopen(proc_path, "r");
        if (f) {
            fscanf(f, "%lu %lu", &vsize, &rss);
            fclose(f);
        }

        // 결과 출력
        if (ps_opts.long_format) {
            printf("%5d %5d %5d %5d %5d %6lu %6lu %c %s\n",
                pid, ppid, pgrp, st.st_uid, st.st_gid,
                vsize * 4, rss * 4, state, cmdline);
        } else {
            printf("%5d ?        00:00:00 %s\n", pid, cmdline);
        }
    }
    closedir(dir);
}

void call_kill(const char *pid_str, const char *sig_str) {
    pid_t pid = atoi(pid_str);
    int sig = sig_str ? atoi(sig_str) : SIGTERM;
    
    if (kill(pid, sig) == -1) {
        perror("kill");
    }
}

void execute_program(const char *program_name, char **args) {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
    } else if (pid == 0) {
        execvp(program_name, args);
        perror("execvp");
        exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}

void call_mmap_test(const char *filename) {
    int fd = open(filename, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("open");
        return;
    }
    
    if (ftruncate(fd, SHARED_SIZE) == -1) {
        perror("ftruncate");
        close(fd);
        return;
    }

    char *shared_memory = mmap(NULL, SHARED_SIZE, 
                             PROT_READ | PROT_WRITE, 
                             MAP_SHARED, fd, 0);
    if (shared_memory == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return;
    }

    struct sigaction sa;
    sa.sa_handler = handle_usr1;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        munmap(shared_memory, SHARED_SIZE);
        close(fd);
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        munmap(shared_memory, SHARED_SIZE);
        close(fd);
        return;
    } else if (pid == 0) {  // 자식 프로세스
        // 시그널 핸들러 재설정
        struct sigaction sa;
        sa.sa_handler = handle_usr1;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(SIGUSR1, &sa, NULL);
        
        printf("자식 프로세스 시작 (PID: %d)\n", getpid());
        data_ready = 0;
        while (!data_ready) {
            pause();
        }
        printf("자식: 읽은 메시지 - %s\n", shared_memory);
        
        strcpy(shared_memory, "자식이 응답합니다");
        kill(getppid(), SIGUSR1);
        
        munmap(shared_memory, SHARED_SIZE);
        close(fd);
        exit(0);
    } else {  // 부모 프로세스
        printf("부모 프로세스 시작 (PID: %d)\n", getpid());
        usleep(100000);  // 0.1초 대기
        strcpy(shared_memory, "부모가 보낸 메시지");
        kill(pid, SIGUSR1);
        
        data_ready = 0;
        while (!data_ready) {
            pause();
        }
        printf("부모: 읽은 응답 - %s\n", shared_memory);
        
        wait(NULL);
        munmap(shared_memory, SHARED_SIZE);
        close(fd);
    }
}

void remove_directory_recursive(const char *path) {
    DIR *d = opendir(path);
    if (!d) return;

    struct dirent *ent;
    while ((ent = readdir(d))) {
        // "." 와 ".." 디렉토리는 건너뜀
        if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
            continue;

        char full_path[MAX_PATH_SIZE];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, ent->d_name);

        struct stat st;
        if (lstat(full_path, &st) == -1)
            continue;

        if (S_ISDIR(st.st_mode)) {
            // 디렉토인 경우 재적으로 삭제
            remove_directory_recursive(full_path);
        } else {
            // 파일인 경우 삭제
            unlink(full_path);
        }
    }
    closedir(d);
    rmdir(path);  // 빈 디렉토리 삭제
}

mode_t parse_mode_str(const char *mode_str, mode_t current_mode) {
    // 8진수 모드인 경우
    if (isdigit(mode_str[0])) {
        return strtol(mode_str, NULL, 8);
    }

    mode_t new_mode = current_mode;
    char op = '\0';
    char who = '\0';
    
    for (const char *p = mode_str; *p; p++) {
        if (*p == 'u' || *p == 'g' || *p == 'o' || *p == 'a') {
            who = *p;
        } else if (*p == '+' || *p == '-' || *p == '=') {
            op = *p;
        } else if (*p == 'r' || *p == 'w' || *p == 'x') {
            mode_t bits = 0;
            if (*p == 'r') bits = S_IRUSR;
            else if (*p == 'w') bits = S_IWUSR;
            else if (*p == 'x') bits = S_IXUSR;

            if (who == 'u' || who == 'a') {
                if (op == '+') new_mode |= bits;
                else if (op == '-') new_mode &= ~bits;
                else if (op == '=') new_mode = (new_mode & ~S_IRWXU) | bits;
            }
            if (who == 'g' || who == 'a') {
                bits >>= 3;
                if (op == '+') new_mode |= bits;
                else if (op == '-') new_mode &= ~bits;
                else if (op == '=') new_mode = (new_mode & ~S_IRWXG) | bits;
            }
            if (who == 'o' || who == 'a') {
                bits >>= 3;
                if (op == '+') new_mode |= bits;
                else if (op == '-') new_mode &= ~bits;
                else if (op == '=') new_mode = (new_mode & ~S_IRWXO) | bits;
            }
        }
    }
    return new_mode;
}