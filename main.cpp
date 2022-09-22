#include "genGraph/genGraph.h"
#include "graphLocalInfo/GraphLocalInfo.h"

int main(int arc, char *argv[]) {
    if (!strcmp(argv[1], "genData")) {
        // 抽取数据集数据
        std::string input_filename = argv[2];
        std::string output_filename = argv[3];
        int year;
        sscanf(argv[4], "%d", &year);
        std::string filed = argv[5];
        genGraph(input_filename, output_filename, year, filed);
    } else if (!strcmp(argv[1], "getLocalInfo")) {
        // 获取节点的局部信息
        std::string filename = argv[3];// 数据文件路径
        int is_undirected;             // 是否是无向图，1表示ture，0表示false
        sscanf(argv[4], "%d", &is_undirected);
        int startVertex;// 开始节点
        sscanf(argv[5], "%d", &startVertex);

        Graph graph;
        loadGraph(filename, is_undirected, graph);

        if (!strcmp(argv[2], "k-order")) {
            // 获取节点的k阶邻居信息
            int k;// 展示k层邻居
            sscanf(argv[6], "%d", &k);

            Neigborhoods n;
            get_k_order_neighborhoods(graph, startVertex, k, n);
            displayNeighborhoods(n);
        } else if (!strcmp(argv[2], "all-path")) {
            // 获取两个节点的所有路径
            int targetVertex;// 目标节点
            sscanf(argv[6], "%d", &targetVertex);

            Path p;
            bool ok = all_path(graph, startVertex, targetVertex, p);
            if (ok) {
                displayAllPaths(p);
            } else {
                std::cout << startVertex << " is no path to " << targetVertex << std::endl;
            }
        }
    } else {
        std::cout << "error using\n"
                  << "./localInfo genData <input_filename> <output_filename> <year=2015> <filed=[dm|cv|nlp]>\n"
                  << "./localInfo getLocalInfo k-order <filename> <is_undirected[1|0]> <Vertex> <k-order>\n"
                  << "./localInfo getLocalInfo all-path <filename> <is_undirected[1|0]> <startVertex> <targetVertex>"
                  << std::endl;
    }

    return 0;
}