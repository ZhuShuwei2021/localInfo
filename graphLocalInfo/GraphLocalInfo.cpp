#include "GraphLocalInfo.h"

struct QueuedNode {
    int index;
    int layer;
};

bool is_visited(const std::vector<int> &nodes, int target) {
    // STL中find的时间复杂度为logN
    if (std::find(nodes.begin(), nodes.end(), target) == nodes.end()) {
        return false;
    } else {
        return true;
    }
}

// 用BFS实现寻找所有k阶邻居
void get_k_order_neighborhoods(const Graph &graph, const int startVertex,
                               const int k, Neigborhoods &results) {
    // 基础条件k判断
    if (k < 1) {
        std::cerr << "k must bigger 0" << std::endl;
        return;
    }
    results[startVertex] = graph[startVertex];
    if (k == 1) {
        return;
    }

    // 访问过的节点
    std::vector<int> visited{startVertex};
    std::deque<QueuedNode> queue;// 待访问节点
    for (auto index: graph[startVertex]) {
        queue.push_back(QueuedNode{index, 1});
        visited.push_back(index);
    }

    while (!queue.empty()) {
        QueuedNode currVertex = queue.front();
        queue.pop_front();

        // 如果当前访问的节点是第k层，则不需要去访问其邻居节点
        if (currVertex.layer == k) continue;

        for (auto const i: graph[currVertex.index]) {
            results[currVertex.index].push_back(i);
            if (!is_visited(visited, i)) {
                // 如果没有被访问
                visited.push_back(i);
                // 将其邻居节点添加到待访问队列中n
                queue.push_back(QueuedNode{
                        i,
                        currVertex.layer + 1});
            }
        }
    }
    return;
}

void displayNeighborhoods(const Neigborhoods &n) {
    Neigborhoods::const_iterator it;
    for (it = n.begin(); it != n.end(); it++) {
        std::cout << it->first << ": {";
        bool flag = true;
        for (auto v: it->second) {
            if (flag) {
                std::cout << v;
                flag = false;
            } else {
                std::cout << ", " << v;
            }
        }
        std::cout << "}" << std::endl;
    }
}

/**
 * 根据当前节点构建双栈
 *
 * @param {int} vertex - 当前节点
 * @param {std::vector<int>} main_stack - 主栈
 * @param {std::vector<std::vector<int>>} neighbor_stack - 邻接节点栈
 * @param {std::vector<int>} visited - 已访问缓存
 */
void buildDualStacks(const Graph &graph, int vertex,
                     std::vector<int> &main_stack,
                     std::vector<std::vector<int>> &neighbor_stack,
                     std::vector<int> &visited) {

    main_stack.push_back(vertex);
    visited.push_back(vertex);

    // 获取当前节点的未访问邻居
    std::vector<int> neighbor{};
    for (auto node: graph[vertex]) {
        if (!is_visited(visited, node)) {
            neighbor.push_back(node);
        }
    }
    // 压入辅助栈中去
    neighbor_stack.push_back(neighbor);
    return;
}

/**
 * 削减双栈（让双栈都减少一层）
 *
 * @param {std::vector<int>} main_stack
 * @param {std::vector<std::vector<int>>} neighbor_stack
 * @param {std::vector<int>} visited - 已访问缓存
 */
void cutDualStack(std::vector<int> &main_stack,
                  std::vector<std::vector<int>> &neighbor_stack,
                  std::vector<int> &visited) {
    // 弹出主栈的栈顶元素
    const int dropMain = main_stack.back();
    main_stack.pop_back();

    // 将其标记为可以访问
    auto eraseNode = std::find(visited.begin(), visited.end(), dropMain);
    visited.erase(eraseNode);

    // 弹出辅助栈的栈顶元素
    neighbor_stack.pop_back();
}

// 用双栈实现的DFS来查找两个节点之间所有路径
bool all_path(const Graph &graph, const int source, const int target, Path &path) {
    bool get_path = false;
    if (source == target) {
        std::cerr << "source must not equal with target" << std::endl;
        return get_path;
    }

    std::vector<int> visited{};                    // 保存访问过的节点
    std::vector<int> main_stack{};                 // 主栈
    std::vector<std::vector<int>> neighbor_stack{};// 辅栈

    buildDualStacks(graph, source, main_stack, neighbor_stack, visited);

    while (!main_stack.empty()) {
        std::vector<int> curNodeNeighbor = neighbor_stack.back();
        neighbor_stack.pop_back();

        // 如果当前节点邻居都被访问过，则继续寻找上一个节点的邻居，否则取出邻居进行后续dfs
        if (curNodeNeighbor.size()) {
            // 取出一个邻居
            const int nextVertex = curNodeNeighbor.back();
            curNodeNeighbor.pop_back();
            // 将剩余的邻居节点放回去
            neighbor_stack.push_back(curNodeNeighbor);
            // 建栈
            buildDualStacks(graph, nextVertex, main_stack, neighbor_stack, visited);
        } else {
            neighbor_stack.push_back(curNodeNeighbor);// 将当前辅助栈复原，否则辅助栈会多少一层
            cutDualStack(main_stack, neighbor_stack, visited);
        }

        // 检查main_stack的栈顶元素是否是target,如果是则为一条路径
        const int peekNode = main_stack.back();
        if (peekNode == target) {
            // 添加到结果中去
            path.push_back(main_stack);
            if (!get_path) {
                get_path = true;
            }
            // 削减一层
            cutDualStack(main_stack, neighbor_stack, visited);
        }
    }
    return get_path;
}

void displayAllPaths(const Path &path) {
    int i = 1;
    for (auto p: path) {
        std::cout << "第" << i << "条路径: ";
        bool flag = true;
        for (auto node: p) {
            if (flag) {
                std::cout << node;
                flag = false;
            } else {
                std::cout << " -> " << node;
            }
        }
        std::cout << std::endl;
        ++i;
    }
}