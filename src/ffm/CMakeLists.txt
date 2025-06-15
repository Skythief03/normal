cmake_minimum_required(VERSION 3.15)
# MSVC 编码设置
if (MSVC)
    add_compile_options(/utf-8)
endif()
# 获取当前目录名称作为目标名
get_filename_component(APP_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${APP_NAME})
# 添加可执行文件
add_executable(${APP_NAME} main.cpp)

set_target_properties(${APP_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${APP_NAME}"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${APP_NAME}"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${APP_NAME}"
)

# 链接 core 库（静态或接口库）
target_link_libraries(${APP_NAME} PRIVATE core)

# 添加 include 下所有一级子目录为头文件路径
file(GLOB CHILD_INCLUDE_DIRS RELATIVE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/include/*)
foreach(child ${CHILD_INCLUDE_DIRS})
    if(IS_DIRECTORY "${CMAKE_SOURCE_DIR}/include/${child}")
        message(STATUS "Adding include path: ${CMAKE_SOURCE_DIR}/include/${child}")
        target_include_directories(${APP_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include/${child}")
    endif()
endforeach()

# 库文件根目录
set(LIB_ROOT "${CMAKE_SOURCE_DIR}/lib")

# 判断使用的编译器（MSVC 或 MinGW）
if (MSVC)
    set(TARGET_LIB_DIR "${LIB_ROOT}/msvc")
    message(STATUS "Detected MSVC, using libraries in: ${TARGET_LIB_DIR}")
elseif(MINGW)
    set(TARGET_LIB_DIR "${LIB_ROOT}/mingw")
    message(STATUS "Detected MinGW, using libraries in: ${TARGET_LIB_DIR}")
else()
    message(FATAL_ERROR "Unsupported compiler: Only MSVC and MinGW are supported.")
endif()

# 添加目标链接目录
link_directories(${TARGET_LIB_DIR})

# 查找所有库文件（.lib for MSVC, .a for MinGW）
if (MSVC)
    file(GLOB_RECURSE LIB_FILES "${TARGET_LIB_DIR}/*.lib")
elseif(MINGW)
    file(GLOB_RECURSE LIB_FILES "${TARGET_LIB_DIR}/*.a")
endif()

# 打印并链接库文件
foreach(LIB_FILE ${LIB_FILES})
    if(NOT IS_DIRECTORY "${LIB_FILE}")
        get_filename_component(LIB_NAME ${LIB_FILE} NAME_WE)
        message(STATUS "Linking library: ${LIB_NAME} (${LIB_FILE})")
        target_link_libraries(${APP_NAME} PRIVATE "${LIB_FILE}")
    endif()
endforeach()
