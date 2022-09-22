#ifndef LOCALINFO_LOADGRAPH_H
#define LOCALINFO_LOADGRAPH_H

#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using Graph = std::vector<std::vector<int>>;
void loadGraph(const std::string filename, bool is_undirected, Graph &g);

#endif// LOCALINFO_LOADGRAPH_H
