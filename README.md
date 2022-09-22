# genGraph & localInfo

本项目主要有以下两个功能：

1. 对于 [dblp_article.xml](./data/dblp_article.xml.gz) 数据抽取出自定义 co-author graph data

2. 对于graph data进行网络分析提取出 local information

---

### genGraph

> 此功能主要用于从dblp_article.xml文件中根据 `发表年份`、`研究领域`来过滤出感兴趣的数据，并以此为基础构建出co-author graph data

- `dblp_article.xml`数据是从[dblp.xml](https://dblp.org/xml/)数据中提取出的 article 的数据(详见[dblp数据结构](https://dblp.org/xml/docu/dblpxml.pdf))

- 研究领域目前包括三大主流领域: `data_mining`、`cv` 和 `nlp`
  - $data\_mining$: `vldb`, `sigmod`, `icde`, `sigir`, `sigkdd`, `tkde`, `vldbj`, `tods`, `tois`
  - $cv$: `cvpr`, `iccv`, `aaai`, `icml`, `nips`, `acm mm`, `eccv`, `eccv`, `tpami`, `ijcv`, `tip`, `cuiv`, `pr`
  - $nlp$: `acl`, `naacl`, `emnlp`, `aaai`, `ijcai`, `sigkdd`, `sigir`, `www`, `wacl`, `cl`, `tacl`

---
### localInfo
> 此功能主要用于获取network 中 node local information

- 获取节点的k-order neighborhood
- 查询两个节点直接所有路径
- ...

---
### using

1. 解压dblp_article.xml.gz数据
   ```bash
   ungzip dblp_article.xml.gz
   ```

2. 编译链接生成可执行程序 `localInfo`

3. 生成dblp的co-author graph data
   ```bash
   ./localInfo genData <input_filename> <output_filename> <year=2015> <filed=[dm|cv|nlp]>
   ```
   参数解释：
   - `genData`: 向程序指明我需要从dblp中生成数据
   - `<input_filename>`: dblp_article.xml/结构类似的数据 路径
   - `<output_filename>`: 生成的文件名
      不要带后缀名，因为会同时身成三个文件
      - `<output_filename>.xml`: 提取出的xml文件，是dblp_article.xml的子集
      - `<output_filename>.graph`: 生成的co-author graph data数据
      - `<output_filename>.author2index`: 作者名与其index的映射关系
   - `<year=2015>`: 只考虑year之后发布的文章，默认为 year = 2015
   - `<filed=[dm|cv|nlp]>`: 感兴趣的领域，分别表示data_mining、cv和nlp

4. 获取节点的局部信息
  - 获取节点的k阶邻居
    ```bash
    ./localInfo getLocalInfo k-order <filename> <is_undirected[1|0]> <Vertex> <k-order>
    ```
    参数解释：
      - `getLocalInfo`: 向程序指明我需要获取节点的局部信息
      - `k-order`: 向程序指明感兴趣的局部信息为 节点的k-order neighborhoods
      - `<filename>`: graph data路径，具体格式参见genData中生成的数据
      - `<is_undirected[1|0]>`: 是否是无向图，1表示是，0表示不是
      - `<Vertex>`: 关注的节点
      - `<k-order>`: 邻居的阶数


  - 查询两个节点之间的所有路径
    ```bash
    ./localInfo getLocalInfo all-path <filename> <is_undirected[1|0]> <startVertex> <targetVertex>
    ```
    参数解释：
      - `getLocalInfo`: 向程序指明我需要获取节点的局部信息
      - `all-path`: 向程序指明感兴趣的局部信息为  两个节点之间的所有路径
      - `<filename>`: graph data路径，具体格式参见genData中生成的数据
      - `<is_undirected[1|0]>`: 是否是无向图，1表示是，0表示不是
      - `<startVertex>`: 开始节点
      - `<targetVertex>`: 目标节点
