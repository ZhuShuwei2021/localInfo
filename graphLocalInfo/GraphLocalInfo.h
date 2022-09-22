#ifndef LOCALINFO_GRAPHLOCALINFO_H
#define LOCALINFO_GRAPHLOCALINFO_H

#include <algorithm>
#include <deque>
#include <iostream>
#include <map>

#include "../genGraph/loadGraph.h"
using Path = std::vector<std::vector<int>>;
using Neigborhoods = std::map<int, std::vector<int>>;

// 返回节点的k阶邻居
void get_k_order_neighborhoods(const Graph &graph, const int startVertex,
                               const int k, Neigborhoods &results);

void displayNeighborhoods(const Neigborhoods &n);

// 返回两个节点之间的所有路径
bool all_path(const Graph &graph, const int source, const int target, Path &path);
void displayAllPaths(const Path &path);

#endif// LOCALINFO_GRAPHLOCALINFO_H
