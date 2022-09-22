//
// Created by demo on 22-9-9.
//
#include "loadGraph.h"

void loadGraph(std::string filename, bool is_undirected, Graph &graph) {
    std::ifstream in(filename);
    std::string line;
    // 读取第一行
    std::getline(in, line);
    std::istringstream record(line);

    // 读取节点数和边数(节点编号为0 - node_nums-1)
    int source, target;
    record >> source;
    record.clear();

    // 初始化graph
    const int node_nums = source;
    graph = Graph(node_nums);

    // 开始读取边信息，构建graph
    while (getline(in, line)) {
        record.str(line);
        record >> source >> target;
        record.clear();

        graph[source].push_back(target);
        if (is_undirected) {
            graph[target].push_back(source);
        }
    }
    in.close();// 关闭文件
}