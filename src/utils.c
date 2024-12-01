#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <grp.h>

void print_permissions(struct stat *file_stat) {
    char type = '-';
    if (S_ISDIR(file_stat->st_mode)) type = 'd';
    else if (S_ISLNK(file_stat->st_mode)) type = 'l';
    else if (S_ISFIFO(file_stat->st_mode)) type = 'p';
    else if (S_ISCHR(file_stat->st_mode)) type = 'c';
    else if (S_ISBLK(file_stat->st_mode)) type = 'b';
    else if (S_ISSOCK(file_stat->st_mode)) type = 's';

    printf("%c%c%c%c%c%c%c%c%c%c",
        type,
        (file_stat->st_mode & S_IRUSR) ? 'r' : '-',
        (file_stat->st_mode & S_IWUSR) ? 'w' : '-',
        (file_stat->st_mode & S_IXUSR) ? 'x' : '-',
        (file_stat->st_mode & S_IRGRP) ? 'r' : '-',
        (file_stat->st_mode & S_IWGRP) ? 'w' : '-',
        (file_stat->st_mode & S_IXGRP) ? 'x' : '-',
        (file_stat->st_mode & S_IROTH) ? 'r' : '-',
        (file_stat->st_mode & S_IWOTH) ? 'w' : '-',
        (file_stat->st_mode & S_IXOTH) ? 'x' : '-');
}

void print_time(const time_t *time) {
    struct tm *tm_info = localtime(time);
    char buffer[26];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
    printf(" %s", buffer);
}

void print_all_times(const struct stat *st) {
    printf("\n  Access: ");
    print_time(&st->st_atime);
    printf("\n  Modify: ");
    print_time(&st->st_mtime);
    printf("\n  Change: ");
    print_time(&st->st_ctime);
    printf("\n");
}

int is_within_base_dir(const char *path) {
    char abs_path[MAX_PATH_SIZE];
    char abs_base[MAX_PATH_SIZE];
    char *real_path, *real_base;

    real_path = realpath(path, abs_path);
    real_base = realpath(BASE_DIR, abs_base);

    if (!real_path || !real_base) {
        char parent_path[MAX_PATH_SIZE];
        strncpy(parent_path, path, sizeof(parent_path) - 1);
        char *last_slash = strrchr(parent_path, '/');
        if (last_slash) {
            *last_slash = '\0';
            real_path = realpath(parent_path, abs_path);
            if (!real_path) return 0;
            return strncmp(real_path, real_base, strlen(real_base)) == 0;
        }
        return 0;
    }

    return strncmp(real_path, real_base, strlen(real_base)) == 0;
}

void get_absolute_path(const char *base, const char *path, char *result) {
    if (path[0] == '/') {
        strncpy(result, path, MAX_PATH_SIZE - 1);
    } else {
        snprintf(result, MAX_PATH_SIZE, "%s/%s", base, path);
    }
    result[MAX_PATH_SIZE - 1] = '\0';
}

int setup_chroot(const char *path) {
    if (!ENABLE_CHROOT) return 0;

    struct stat st;
    if (stat(path, &st) == -1) {
        if (mkdir(path, DEFAULT_DIR_MODE) == -1) {
            perror("mkdir");
            return -1;
        }
    }

    const char *required_dirs[] = {"/bin", "/lib", "/lib64", "/usr", NULL};
    char full_path[MAX_PATH_SIZE];
    
    for (int i = 0; required_dirs[i]; i++) {
        snprintf(full_path, sizeof(full_path), "%s%s", path, required_dirs[i]);
        if (mkdir(full_path, DEFAULT_DIR_MODE) == -1 && errno != EEXIST) {
            perror("mkdir");
            return -1;
        }
        if (chmod(full_path, DEFAULT_DIR_MODE) == -1) {
            perror("chmod");
            return -1;
        }
    }

    if (chdir(path) == -1) {
        perror("chdir");
        return -1;
    }

    return 0;
}

void print_user_group(uid_t uid, gid_t gid) {
    struct passwd *pw = getpwuid(uid);
    struct group *gr = getgrgid(gid);
    
    const char *user_str = pw ? pw->pw_name : "unknown";
    const char *group_str = gr ? gr->gr_name : "unknown";
    
    printf(" %s %s", user_str, group_str);
}