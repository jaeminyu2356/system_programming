#!/bin/bash

# 색상 코드 정의
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}파일 시스템 GUI 프로그램 의존성 설치 스크립트${NC}"
echo "======================================"

# root 권한 체크
if [ "$EUID" -ne 0 ]; then 
    echo -e "${RED}오류: 이 스크립트는 root 권한으로 실행해야 합니다.${NC}"
    echo "다음 명령어로 다시 실행하세요: sudo $0"
    exit 1
fi

# 시스템 업데이트
echo -e "${YELLOW}시스템 패키지 목록 업데이트 중...${NC}"
apt-get update

# 필수 개발 도구 설치
echo -e "${YELLOW}필수 개발 도구 설치 중...${NC}"
apt-get install -y build-essential cmake git

# Qt 라이브러리 설치
echo -e "${YELLOW}Qt 라이브러리 설치 중...${NC}"
apt-get install -y \
    qtbase5-dev \
    qtchooser \
    qt5-qmake \
    qtbase5-dev-tools \
    qttools5-dev-tools

# 추가 Qt 모듈 설치
echo -e "${YELLOW}추가 Qt 모듈 설치 중...${NC}"
apt-get install -y \
    libqt5widgets5 \
    libqt5gui5 \
    libqt5core5a \
    libgl1-mesa-dev \
    libxkbcommon-x11-0 \
    libx11-xcb1

# 개발 도구 설치
echo -e "${YELLOW}개발 도구 설치 중...${NC}"
apt-get install -y qtcreator gdb

# 문서 도구 설치
echo -e "${YELLOW}문서 도구 설치 중...${NC}"
apt-get install -y doxygen graphviz

# 설치 확인
echo -e "${YELLOW}설치된 버전 확인 중...${NC}"
echo "GCC 버전:"
gcc --version | head -n1
echo "CMake 버전:"
cmake --version | head -n1
echo "Qt 버전:"
qmake --version | head -n1

# 설치 완료 메시지
echo -e "${GREEN}모든 의존성 설치가 완료되었습니다!${NC}"
echo "이제 다음 명령어로 프로그램을 빌드할 수 있습니다:"
echo "mkdir build && cd build"
echo "cmake .."
echo "make"

exit 0 