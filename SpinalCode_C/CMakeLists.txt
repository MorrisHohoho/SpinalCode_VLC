cmake_minimum_required (VERSION 3.16.3)

project (Test)


#设置执行文件输出目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#添加头文件路径
include_directories(${PROJECT_SOURCE_DIR}/include)

#递归获取目录下所有的C文件
file(GLOB_RECURSE c_files ${PROJECT_SOURCE_DIR}/src/*.c)

add_executable(${PROJECT_NAME} ${c_files} src/RNG/RNG.c)