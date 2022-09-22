project("genGraph")

# 指明CMakeLists.txt 是用于静态库生成静态库的
add_library(genGraph genGraph.cpp loadGraph.cpp tinyxml2.cpp)