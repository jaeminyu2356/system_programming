#!/bin/bash

# 색상 코드 정의
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${YELLOW}파일 시스템 GUI Docker 실행 스크립트${NC}"
echo "======================================"

# X11 소켓 권한 설정
xhost +local:docker

# Docker 이미지 빌드
echo -e "${YELLOW}Docker 이미지 빌드 중...${NC}"
docker build -t file_system_gui .

# Docker 컨테이너 실행
echo -e "${YELLOW}Docker 컨테이너 실행 중...${NC}"
docker run -it --rm \
    -e DISPLAY=$DISPLAY \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    -v $HOME:/home/user \
    -v /dev/dri:/dev/dri \
    --device /dev/dri \
    --privileged \
    --network host \
    file_system_gui

# X11 소켓 권한 복구
xhost -local:docker

echo -e "${GREEN}Docker 컨테이너가 종료되었습니다.${NC}" 