# Ubuntu 22.04를 기본 이미지로 사용
FROM ubuntu:22.04

# 환경 변수 설정
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Seoul
ENV QT_X11_NO_MITSHM=1
ENV DISPLAY=:0
ENV LANG=ko_KR.UTF-8
ENV LANGUAGE=ko_KR:en
ENV LC_ALL=ko_KR.UTF-8
ENV XDG_RUNTIME_DIR=/tmp/runtime-user
ENV QT_DEBUG_PLUGINS=1

# 기본 패키지 업데이트 및 필요한 패키지 설치
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    cmake \
    git \
    qtbase5-dev \
    qtchooser \
    qt5-qmake \
    qtbase5-dev-tools \
    libqt5widgets5 \
    libqt5gui5 \
    libqt5core5a \
    x11-apps \
    libgl1-mesa-dev \
    libgl1-mesa-glx \
    libglx-mesa0 \
    libegl1-mesa \
    libxkbcommon-x11-0 \
    libx11-xcb1 \
    locales \
    fonts-nanum \
    language-pack-ko \
    mesa-utils \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# 로케일 설정
RUN locale-gen ko_KR.UTF-8 && \
    update-locale LANG=ko_KR.UTF-8

# GUI 애플리케이션 실행을 위한 사용자 설정
RUN useradd -m -s /bin/bash user && \
    mkdir -p /app && \
    chown -R user:user /app && \
    mkdir -p /tmp/runtime-user && \
    chmod 700 /tmp/runtime-user && \
    chown user:user /tmp/runtime-user

# 작업 디렉토리 설정 및 사용자 전환
WORKDIR /app
USER user

# 소스 코드 복사 및 권한 설정
COPY --chown=user:user . .

# 빌드 수행
RUN rm -rf build && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make -j$(nproc) && \
    chmod +x bin/file_system_gui

# 볼륨 설정
VOLUME ["/home/user"]

# 작업 디렉토리를 build/bin으로 변경
WORKDIR /app/build/bin

# 실행 파일 존재 확인
RUN test -x file_system_gui || exit 1

# 컨테이너 실행 시 실행할 명령어
CMD ["./file_system_gui"] 