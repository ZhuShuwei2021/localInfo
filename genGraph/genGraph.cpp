#include "genGraph.h"

using namespace tinyxml2;

std::string getXmlText(XMLNode *second_layer) {
    char title[256] = {};
    while (second_layer) {
        // 根据是否存在第三层来判断是否包含html标签
        if (second_layer->FirstChild()) {
            // 存在第三层,表明这是包含在html标签中的数据
            if (title == nullptr) {
                strncpy(title, second_layer->FirstChild()->Value(),
                        sizeof(title));
                // title = second_layer->FirstChild()->Value();
            } else {
                // strcat(title, " ");
                strcat(title, second_layer->FirstChild()->Value());
            }
        } else {
            // 此时获得的是字符串
            if (title == nullptr) {
                strncpy(title, second_layer->Value(), sizeof(title));
            } else {
                // strcat(title, " ");
                strcat(title, second_layer->Value());
            }
        }

        second_layer = second_layer->NextSibling();
    }
    return std::string(title);
}

void filterByYear(XMLNode *root, int last_year) {
    XMLNode *article = root->FirstChild();
    std::istringstream record;
    while (article) {
        XMLElement *tempElem = article->ToElement();
        // get the article publish year
        int article_year = -1;
        const char *yearStr = tempElem->FirstChildElement("year")->GetText();
        if (yearStr == nullptr) {
            std::cerr << "year has error!"
                      << "\t";
        } else {
            record.str(yearStr);
            article_year = -1;
            record >> article_year;
            record.clear();
        }

        // get the article which published after last_year
        if (article_year < last_year) {
            XMLNode *tempNode = article;
            article = article->NextSibling();
            root->DeleteChild(tempNode);
            std::cerr << "missing!" << std::endl;
            continue;
        }

        article = article->NextSibling();
    }
}

void filterByFiled(XMLNode *root, std::string filed) {
    std::vector<std::string> filed_name;
    XMLNode *article = root->FirstChild();
    std::istringstream record;
    if (filed == "dm") {
        // 数据挖掘领域
        filed_name = {"vldb", "sigmod", "icde", "sigir", "sigkdd",
                      "tkde", "vldbj", "tods", "tois"};
    } else if (filed == "cv") {
        // CV领域
        filed_name = {"cvpr", "iccv", "aaai", "icml", "nips", "mm", "acm mm",
                      "eccv", "tpami", "ijcv", "tip", "cuiv", "pr"};
    } else if (filed == "nlp") {
        // NLP领域
        filed_name = {"acl", "naacl", "emnlp", "aaai", "ijcai", "sigkdd",
                      "sigir", "www", "wacl", "cl", "tacl"};
    }

    while (article) {
        XMLElement *tempElem = article->ToElement();
        const char *my_string = tempElem->Attribute("key");
        record.str(my_string);
        std::string type, name;
        std::getline(record, type, '/');
        std::getline(record, name, '/');
        record.clear();

        if (std::find(filed_name.begin(), filed_name.end(), name) != filed_name.end()) {
            article = article->NextSibling();
        } else {
            XMLNode *tempNode = article;
            article = article->NextSibling();
            root->DeleteChild(tempNode);
            continue;
        }
    }
}

void constructGraph(XMLNode *root, std::string outFilePath) {
    XMLNode *article = root->FirstChild();
    std::istringstream record;

    struct coInfo {
        std::vector<std::string> confWithTitles;
        int coNums;
    };
    std::map<std::pair<std::string, std::string>, coInfo> graph;
    std::set<std::string> authors_set;
    std::map<std::string, int> author2index;

    while (article) {
        XMLNode *first_layer = article->FirstChild();

        std::vector<std::string> authors;
        std::string article_title;
        std::string conf_name;

        while (first_layer) {
            // 1. 获取title
            if (strcmp(first_layer->Value(), "title") == 0) {
                article_title = std::string(getXmlText(first_layer->FirstChild()));
            }

            // 2. 获取authors
            if (strcmp(first_layer->Value(), "author") == 0) {
                std::string author = getXmlText(first_layer->FirstChild());
                authors_set.insert(author);
                authors.push_back(author);
            }

            // 3. 获取期刊/会议信息
            XMLElement *tempElem = article->ToElement();
            const char *my_string = tempElem->Attribute("key");
            record.str(my_string);
            std::string type;
            std::getline(record, type, '/');
            getline(record, conf_name, '/');
            record.clear();

            // 进行下一个tag,继续获取信息
            first_layer = first_layer->NextSibling();
        }

        // 构建author2index
        int num = 0;
        for (std::string author: authors_set) {
            author2index[author] = num++;
        }
        // 4. 对于获取到的该篇文章信息, 根据authors构建co-author边

        if (authors.size() >= 2) {
            std::vector<std::string>::iterator author_a, author_b;
            for (author_a = authors.begin(); author_a != authors.end() - 1;
                 ++author_a) {
                for (author_b = author_a + 1; author_b != authors.end();
                     ++author_b) {
                    // 4.1 创建统一的key
                    std::pair<std::string, std::string> key;
                    if (*author_a <= *author_b) {
                        key.first = *author_a;
                        key.second = *author_b;
                    } else {
                        key.first = *author_b;
                        key.second = *author_a;
                    }
                    // 4.2 将数据插入graph中
                    // 判断key是否存在于graph中
                    if (graph.find(key) != graph.end()) {
                        // key存在于graph中
                        graph[key].confWithTitles.push_back(conf_name + "/" +
                                                            article_title);
                        ++graph[key].coNums;
                    } else {
                        // key不存在于graph中,直接添加
                        coInfo co_info;
                        co_info.confWithTitles.push_back(conf_name + "/" +
                                                         article_title);
                        co_info.coNums = 1;
                        graph[key] = co_info;
                    }
                }
            }
        } else {
            std::cerr << "only have one author" << std::endl;
        }

        // 遍历下一篇文章
        article = article->NextSibling();
    }

    // 遍历map, 输出结果到文件中去
    // 输出co-author 带权图
    std::ofstream out(outFilePath + ".graph");
    // 写入头信息，节点数与边数
    out << author2index.size() << "\t" << graph.size() << std::endl;
    for (auto &it: graph) {
        std::pair<std::string, std::string> edge = it.first;
        // it.second.confWithTitles: 保存了共同合作的会议与文章信息
        out << author2index[edge.first] << "\t" << author2index[edge.second]
            << "\t" << it.second.coNums << std::endl;
    }
    out.close();

    // 输出author2index
    out.open(outFilePath + ".author2index");
    for (auto &it: author2index) {
        out << it.first << ",\t" << it.second << std::endl;
    }
    out.close();
}

void genGraph(std::string inFilePath, std::string outFilePath, int year = 2015, std::string filed = "dm") {
    // 仅仅适用于xml数据，
    XMLDocument doc;
    XMLError err = doc.LoadFile(inFilePath.c_str());
    if (err != XML_SUCCESS) {
        std::cout << "Error loading file: " << (int) err << std::endl;
    }
    // 仅仅适用于dblp data
    XMLNode *root = doc.FirstChild()->NextSibling()->NextSibling();
    if (root == nullptr) {
        std::cout << "err! root is null" << std::endl;
    }
    std::cout << root->Value() << std::endl;

    filterByYear(root, year);// 根据年份过滤
    std::cout << "filter by year over" << std::endl;
    filterByFiled(root, filed);// 根据领域过滤
    std::cout << "filter by filed over" << std::endl;
    // 保存xml文件
    doc.SaveFile((outFilePath + ".xml").c_str());

    constructGraph(root, outFilePath);
}