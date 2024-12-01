cmake_minimum_required(VERSION 3.16)
project(file_system_gui)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt 설정
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt6 패키지 찾기
find_package(Qt6 REQUIRED COMPONENTS 
    Core
    Gui
    Widgets
)

# 소스 파일 목록
set(SOURCES
    src/main.cpp
    src/commands.c
    src/utils.c
    src/mainwindow.cpp
)

# 헤더 파일 목록
set(HEADERS
    include/commands.h
    include/utils.h
    include/config.h
    include/mainwindow.h
)

# C 소스 파일들은 C 컴파일러로 컴파일
set_source_files_properties(
    src/commands.c
    src/utils.c
    PROPERTIES
    LANGUAGE C
)

# C++ 소스 파일들은 C++ 컴파일러로 컴파일
set_source_files_properties(
    src/main.cpp
    src/mainwindow.cpp
    PROPERTIES
    LANGUAGE CXX
)

# 실행 파일 생성
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 헤더 파일 경로 추가
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Qt 라이브러리 링크
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    pthread
)

# 컴파일 옵션 설정
target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -pedantic
) 