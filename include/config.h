#pragma once
#ifndef CONFIG_H
#define CONFIG_H

// 기본 설정
#define MAX_CMD_SIZE    128
#define MAX_PATH_SIZE   256
#define BASE_DIR        "/tmp/test"

// 기능 활성화/비활성화 플래그
#define ENABLE_CHROOT   1
#define ENABLE_SIGNALS  1
#define ENABLE_PS       1

// 권한 설정
#define DEFAULT_DIR_MODE    0777
#define DEFAULT_FILE_MODE   0644

#endif // CONFIG_H 