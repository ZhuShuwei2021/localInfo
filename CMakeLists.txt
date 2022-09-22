cmake_minimum_required(VERSION 3.5)

project(localInfo)

# 指定为C++11 版本
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the genGraph、graphLocalInfo library
add_subdirectory(genGraph)
add_subdirectory(graphLocalInfo)

# add the executable
add_executable(${PROJECT_NAME} main.cpp)

# 链接可执行文件和静态库
target_link_libraries(${PROJECT_NAME} PUBLIC
        genGraph
        graphLocalInfo
        )

# add the binary tree to the search path for include files
target_include_directories(${PROJECT_NAME} PUBLIC
        ${PROJECT_BINARY_DIR}
        ${PROJECT_SOURCE_DIR}/genGraph
        ${PROJECT_SOURCE_DIR}/graphLocalInfo
        )