## 自动匹配 core 目录下所有 cpp 文件
#file(GLOB CORE_SRC CONFIGURE_DEPENDS *.cpp)
#
## 创建动态库 core（Windows: core.dll，Linux: libcore.so，macOS: libcore.dylib）
#add_library(core SHARED ${CORE_SRC})
#
## 自动包含本目录头文件（.h/.hpp），使其它项目可用
#target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 创建一个接口库（不会生成任何编译文件）
add_library(core INTERFACE)

# 设置头文件包含路径，供依赖它的目标使用
target_include_directories(core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

