# OI 模板库（精选版）

本目录模板的代码主体提取自 `all.cpp`（一位 OI 选手的 1135 份题解合并文件），
另有部分冷门算法为**新编写**（码风对齐原作者的 `#define int long long` / 2 空格缩进 / 紧凑语句）。

## 码风约定（全库统一）

- 每个文件**只有纯代码体**：`#include` + `using namespace std;`（+ `#define int long long` 视算法而定）+ 实现代码；
- **所有注释统一放在文件尾部的注释块**中（名称 / 复杂度 / 用途 / 来源 / 使用示例）；
- 使用示例以注释形式给出，需要时取消注释即可编译运行；
- 新算法与 all.cpp 中抽取的代码风格保持一致。

## 目录结构

```
Template/
├── DataStructure/   数据结构（平衡树 / 堆 / 树 / 线段树变体 / bitset / 字典树）
│   └── Heaps/       15 种堆实现合集（洛谷《对优先队列的爱》）
├── Graph/           图论（网络流 / Tarjan 家族 / 树上技巧 / 计数）
├── String/          字符串（回文 / 后缀结构 / 哈希）
├── Math/            数学（模运算 / 数论 / 高精度 / 变换 / 多项式）
├── Misc/            技巧与杂项
└── Trash/           被移除的常见/基础/重复模板（保留备查）
```

全库共 76 个模板（DataStructure 44 / Graph 14 / String 5 / Math 13 / Misc 6）+ 本 README。

## 索引

### DataStructure/（30 个）

| 文件 | 内容 |
|---|---|
| `BalancedTree_WBST_Seq.cpp` | 权值平衡树·序列版（确定性平衡，懒标记反转，可替代文艺平衡树） |
| `BalancedTree_WBST_Set.cpp` | 权值平衡树·集合版（有序多重集，ins/remove/rnk/kth） |
| `BalancedTree_Treap.cpp` | 无旋 Treap（fhq，split/merge，按值或按大小分裂） |
| `BalancedTree_Splay.cpp` | Splay 伸展树（文艺平衡树：区间反转/序列操作） |
| `LeftistHeap.cpp` | 左偏树（可并堆，小根堆，O(log n) 合并） |
| `CartesianTree.cpp` | 笛卡尔树（单调栈 O(n) 构建，RMQ/最大矩形） |
| `CatTree.cpp` | 猫树（静态可结合区间查询 O(1)，基于 operator+） |
| `ODT.cpp` | 珂朵莉树（map 区间赋值，spl/assign/query） |
| `SegmentTreeGCD.cpp` | gcd 线段树（单点改 + 前缀 gcd 查询） |
| `SegmentTreeBeats.cpp` | 势能线段树（区间加 / chmin / chmax / 和与最值，均摊 O((n+q)log n)） |
| `SegmentTree_Semigroup.cpp` | 半群懒标记线段树（info/tag 自定义，动态开点 + 清空，update/set/query 全套） |
| `SegmentTree_IterativeLazy.cpp` | 非递归线段树（zkw 式，区间加 + 区间和，懒标记，无递归常数小） |
| `Bitset_Dynamic.cpp` | 手写动态 bitset（std::bitset 全部操作 + resize 动态长度，ctz 加速枚举） |
| `YFastTrie.cpp` | y-fast trie（x-fast trie + 分块，O(log 64) 查找/前驱/后继/插入/删除） |
| `RangeSemigroup_ACK.cpp` | 静态区间半群查询（阿克曼分块，O(n) 预处理 O(α(n)) 查询，需 C++20） |
| `RMQ_Linear.cpp` | 线性 RMQ（OI-Wiki：笛卡尔树 + ±1 分块状压，O(n)-O(1)） |
| `SqrtTree.cpp` | Sqrt Tree（静态区间可结合查询，O(log log n)） |
| `LCT.cpp` | Link-Cut Tree（动态树：link/cut/makeroot/路径查询，均摊 O(log n)） |
| `GlobalBST.cpp` | 全局平衡二叉树（静态树路径加/路径最大值，O(log n) 级） |
| `TopTree.cpp` | 静态 Top Tree（簇分解 + rake 轻链聚合，路径查询 + 子树簇查询） |
| `WaveletMatrix_Dynamic.cpp` | 二进制分组动态 Wavelet Matrix（插入/全局第 k 小/rank） |
| `KDT_BinaryGroup.cpp` | K-D Tree（矩形和/最近点，注释给出二进制分组动态版） |
| `Heaps/` | 15 种堆实现合集（见下表，均支持 newnode/top/join/decrease_key/erase） |

#### Heaps/（16 个，洛谷《对优先队列的爱》）

| 文件 | 实现 |
|---|---|
| `Heap_PBDS.cpp` | 配对堆（__gnu_pbds::priority_queue） |
| `Heap_Leftist.cpp` | 左偏树（可并堆，O(log n) merge） |
| `Heap_Skew.cpp` | 斜堆（可并堆，均摊 O(log n)） |
| `Heap_Binomial.cpp` | 二项堆（无均摊常数的插入特化） |
| `Heap_Pairing.cpp` | 配对堆（均摊 O(log n)） |
| `Heap_RankPairing_A.cpp` | 赋级配对堆·甲类半树写法 |
| `Heap_RankPairing_B.cpp` | 赋级配对堆·乙类多叉树写法 |
| `Heap_Fibonacci.cpp` | 斐波那契堆（期望 O(1) 插入/减键） |
| `Heap_Hollow.cpp` | hollow heap（奉先堆） |
| `Heap_BinaryExt.cpp` | STL 二叉堆扩展（懒删除 + 小缓冲快速插入） |
| `Heap_Thin.cpp` | 瘦堆（thin heap） |
| `Heap_LazyBinomial.cpp` | 懒二项堆（O(1) 合并） |
| `Heap_OneRootFibonacci.cpp` | 单根斐波那契堆 |
| `Heap_Quake.cpp` | 地震堆（quake heap） |
| `Heap_Slim.cpp` | 纤细堆（slim heap） |
| `Heap_WeightLeftist.cpp` | 重左偏（按子树大小合并） |

> Heaps 系列来自洛谷《对优先队列的爱》，代码原样保留（已统一补 `#define int long long`
> 使 LLONG_MIN/MAX 哨兵语义正确）。统一接口：`newnode(x,i)/top(x)/join/merge/decrease_key(h,p,v)/erase(h,x)`，
> 小根堆，堆以"根节点下标"标识。注意：文章自述"部分实现未处理合并空堆/自合并边界"；
> **经随机压力测试**：Leftist、WeightLeftist、Skew、Pairing、Binomial、Fibonacci、Hollow、
> LazyBinomial、OneRootFibonacci、Quake、PBDS 共 11 个可正常工作；**RankPairing_A、
> RankPairing_B、Thin、Slim、BinaryExt 存在文章原版的固有缺陷（减键/删除路径错误），
> 标注为参考实现**；全局数组大小（N、M）按题调整。

### Graph/（14 个）

| 文件 | 内容 |
|---|---|
| `Dinic.cpp` | 最大流（vector 版 `dinic<T>`，当前弧优化） |
| `MCMF.cpp` | 最小费用最大流（SPFA + 沿最短路推流） |
| `Flow_Bounds.cpp` | 上下界网络流（无源汇可行流 + 有源汇最大/最小流，基于 Dinic） |
| `HopcroftKarp.cpp` | 二分图最大匹配（O(E√V)） |
| `KM.cpp` | 二分图最大权完美匹配（O(n³)，slack 优化） |
| `TarjanSCC.cpp` | 强连通分量 + 缩点 |
| `TarjanCutVertex.cpp` | 割点（low[y] >= dfn[x]，根节点特判） |
| `TarjanBridge.cpp` | 桥 / 边双连通分量（e-DCC，按边编号处理重边） |
| `TarjanBCC.cpp` | 点双连通分量 / 圆方树 |
| `PointDivide.cpp` | 点分治 / 动态点分树（重心 + 双 BIT 容斥） |
| `KruskalWQS.cpp` | WQS 二分 + Kruskal（恰好 k 条特殊边的最小生成树） |
| `DominatorTree.cpp` | 支配树（Lengauer-Tarjan） |
| `KruskalRebuildTree.cpp` | Kruskal 重构树（瓶颈边 / 边权限制连通性） |
| `TriangleCount.cpp` | 三元环计数（重定向 O(m√m)） |

### String/（5 个）

| 文件 | 内容 |
|---|---|
| `Manacher.cpp` | 回文串（最长回文子串，可扩展计数） |
| `SuffixArray.cpp` | 后缀数组（倍增 + 基数排序）+ height |
| `SuffixAutomaton.cpp` | 后缀自动机 SAM（endpos 统计 / 本质不同子串） |
| `StringHash.cpp` | 字符串哈希（mod 2^61-1，随机种子 + shift 混淆） |
| `ACAutomaton.cpp` | AC 自动机（多模式串匹配，Trie + fail + 拓扑计数） |

### Math/（11 个）

| 文件 | 内容 |
|---|---|
| `ModInt.cpp` | Montgomery 模数封装（固定模数 `mint = modint<md>`） |
| `ModInt_Full.cpp` | 完整 modint（蒙哥马利约简，+ - * / % 幂 逆元 开方 比较 输入输出） |
| `DynamicModInt.cpp` | Barrett 动态模数 modint（运行期 set_mod） |
| `BigInt.cpp` | 高精度整数（2^32 基底，+ - * / % 幂 gcd，Knuth D 除法，向零取整） |
| `MatrixMaxPlus.cpp` | (max,+) 矩阵快速幂（Floyd 式转移） |
| `MillerRabin.cpp` | Miller-Rabin 素性测试（7 个确定性基，64 位） |
| `PollardRho.cpp` | Pollard-Rho 质因数分解（配套 Miller-Rabin） |
| `Gcd_Binary.cpp` | 二进制 GCD（__builtin_ctz 加速，均摊 O(1) 级） |
| `Gcd_Table.cpp` | 查表法 O(1) GCD（预计算 2^11 表 + 提前终止） |
| `FWT.cpp` | 快速沃尔什变换（or / and / xor 卷积） |
| `GaussJordan.cpp` | 高斯-约旦消元（模域解方程组/求秩，附双精度版注释） |
| `LagrangeInterpolation.cpp` | 拉格朗日插值（一般点 O(n²) + 横坐标 1..n 的 O(n) 版） |
| `NTT.cpp` | NTT + 多项式全家桶（convolution / inv / ln / exp / deri / inte） |

### Misc/（6 个）

| 文件 | 内容 |
|---|---|
| `Bitset.cpp` | bitset 技巧（26 位通配符字符串匹配） |
| `HashTable_PBDS.cpp` | 哈希表实现一：gp_hash_table + splitmix64（抗卡） |
| `HashTable_Chain.cpp` | 哈希表实现二：手写链式引用计数表（槽位池回收） |
| `DSUonTree.cpp` | 树上启发式合并（DSU on tree，子树统计） |
| `SimulatedAnnealing.cpp` | 模拟退火（通用模板，连续/离散优化） |
| `ExpressionEval.cpp` | 表达式求值（递归下降，+ - * / % ^ 括号 一元负号 变量） |

### Trash/（24 个，已移除）

因"代码简单 / 思想基础 / OI 常见 / 不易出错"而移除：
`FenwickTree`、`DSU`、`SparseTable`、`SegmentTreeLazy`、`SegmentTreeIterative`、
`Dijkstra`、`LCA_BinaryLifting`、`LCA_EulerTourRMQ`、`HLD`、`VirtualTree`、
`KMP`、`ZFunction`、`RollingHash`、`Matrix`、`Combinatorics`、`EulerSieve`、`Exgcd`、
`SlopeOptimization`、`TreeReroot`、`MonotoneQueue`、`DigitDP`、`FastIO`、`Random`、`LogTrick`

移除原因分类：
- 常见/基础：FenwickTree、DSU、SparseTable、Dijkstra、LCA×2、HLD、Matrix、Combinatorics、EulerSieve、Exgcd、FastIO
- 难以封装/强藕连：KMP、ZFunction、VirtualTree
- 结构简单但可拓展（按需自写）：SlopeOptimization、TreeReroot、MonotoneQueue、DigitDP、SegmentTreeLazy、SegmentTreeIterative
- 内容重复/被包含：RollingHash（hash_t 与 StringHash 重复）、Random（splitmix64 与 HashTable_PBDS 重复）、LogTrick（gcd 段合并被 ODT 包含）

## 说明

- 全部模板为 C++17（结构化绑定、`__int128`、`__lg` 等）；g++ 编译示例：
  `g++ -std=c++17 -O2 文件.cpp -c`（纯模板，无 main，注释掉尾部示例后可直接粘贴进题解）。
- 数组大小（`N`、`M`）多为原文常量，使用时按题目调整。
- `#define int long long` 的文件注意 `memset`/`0x3f` 对 64 位 int 的效果。
- 同算法的不同实现按文件名后缀区分：平衡树（Treap / Splay / WBST_Seq / WBST_Set）、
  GCD（Binary / Table）、哈希表（PBDS / Chain），按题目环境与偏好选用。
