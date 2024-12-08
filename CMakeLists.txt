cmake_minimum_required(VERSION 3.16)
project(file_system_gui)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 빌드 디렉토리 설정
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Qt 설정
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt5 키지 찾기
find_package(Qt5 COMPONENTS 
    Core
    Gui
    Widgets
    REQUIRED
)

# 소스 파일 목록
set(SOURCES
    src/main.cpp
    src/commands.c
    src/utils.c
    src/mainwindow.cpp
    src/mainwindow_ui.cpp
    src/mainwindow_file_actions.cpp
    src/mainwindow_process_actions.cpp
    src/mainwindow_test_actions.cpp
)

# 헤더 파일 목록
set(HEADERS
    include/commands.h
    include/utils.h
    include/config.h
    include/mainwindow.h
    include/mainwindow_ui.h
    include/mainwindow_file_actions.h
    include/mainwindow_process_actions.h
    include/mainwindow_test_actions.h
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
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
)

# Qt 라이브러리 링크
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    pthread
)

# 컴파일 옵션 설정
target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -pedantic
)

# Qt MOC 처리를 위한 설정
set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
) 