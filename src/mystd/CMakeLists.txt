# 获取当前目录名称作为目标
get_filename_component(APP_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# 添加可执行文件（假设 main.cpp 存在）
add_executable(${APP_NAME} main.cpp)

# 链接 core（头文件-only或库，视你core/CMakeLists而定）
target_link_libraries(${APP_NAME} PRIVATE core)

#递归添加 include 下的所有目录到头文件路径
file(GLOB_RECURSE PUBLIC_INCLUDES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/include/*)
foreach(INC ${PUBLIC_INCLUDES})
    if(IS_DIRECTORY ${INC})
        target_include_directories(${APP_NAME} PRIVATE ${INC})
    endif()
endforeach()

#递归链接 lib/ 下所有静态库（*.a 或 *.lib）
file(GLOB_RECURSE LIBS CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/lib/*.a ${CMAKE_SOURCE_DIR}/lib/*.lib)

# 链接找到的库文件（使用完整路径）
target_link_libraries(${APP_NAME} PRIVATE ${LIBS})
