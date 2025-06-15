cmake_minimum_required(VERSION 3.15)
project(MultiProjectWorkspace)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加头文件路径
include_directories(${CMAKE_SOURCE_DIR}/include)

# 添加预编译库目录（libexternal.a 等）
link_directories(${CMAKE_SOURCE_DIR}/lib)

# 添加 core 库
add_subdirectory(src/core)

# 自动添加 src/ 下除 core 外的所有子目录为子项目
file(GLOB SUBDIRS RELATIVE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/*)

foreach(subdir ${SUBDIRS})
    if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/src/${subdir} AND NOT ${subdir} STREQUAL "core")
        add_subdirectory(src/${subdir})
    endif()
endforeach()
