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
├── String/          字符串（回文 / 后缀结构 / 哈希 / Z 函数）
├── Math/            数学（模运算 / 数论 / 高精度 / 变换 / 多项式 / 同余与类欧 / 组合 / 递推 / 积分）
├── Geometry/        计算几何（二维基础 / 半平面交 / 最小圆覆盖）
├── Misc/            技巧与杂项
└── Trash/           被移除的常见/基础/重复模板（保留备查）
```

全库共 121 个模板（DataStructure 48 / Graph 22 / String 8 / Math 29 / Misc 11 / Geometry 3）+ 本 README。

## 索引

### DataStructure/（48 个，含 Heaps/ 16 个）

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
| `SegmentTree_Semigroup.cpp` | 半群懒标记线段树（info/tag 自定义，动态开点，全程非递归；ACL 对齐接口 get/all_prod/max_right/min_left + O(n) vector 建树） |
| `SegmentTree_IterativeLazy.cpp` | 非递归线段树（zkw 式，区间加 + 区间和，懒标记，无递归常数小） |
| `SegmentTree_ACL.cpp` | 懒标记线段树·ACL 式（静态满树，泛型 op/mapping，非递归；接口与 atcoder::lazy_segtree 逐函数对齐，支持依赖段长的作用如区间仿射+和，含 max_right/min_left） |
| `SegmentTree_LiChao.cpp` | 李超线段树（线段/直线插入，区间最小查询，O(log)） |
| `PersistentSegmentTree.cpp` | 主席树（静态区间第 k 小 / 区间 rank，前缀版本化） |
| `PersistentDSU.cpp` | 可持久化并查集（线段树存 fa/siz + 按大小合并，历史版本查询，无路径压缩） |
| `PersistentTreap.cpp` | 可持久化平衡树（fhq-Treap 全持久化，历史版本 ins/erase/kth/rnk/前驱/后继） |
| `SegmentTree_Merge.cpp` | 线段树合并（动态开点权值树，均摊 O(log V)，子树统计） |
| `Trie_Binary.cpp` | 可持久化 01-Trie（版本区间异或最值 / rank，免离散化） |
| `DSU_Weighted_Rollback.cpp` | 带权并查集（相对关系维护）+ 可撤销并查集（线段树分治用） |
| `Bitset_Dynamic.cpp` | 手写动态 bitset（std::bitset 全部操作 + resize 动态长度，ctz 加速枚举） |
| `YFastTrie.cpp` | y-fast trie（x-fast trie + 分块，O(log 64) 查找/前驱/后继/插入/删除） |
| `RangeSemigroup_ACK.cpp` | 静态区间半群查询（阿克曼分块，O(n) 预处理 O(α(n)) 查询；已改写为 C++14 兼容） |
| `MonoidOfflineQuery.cpp` | 离线静态区间半群乘积（猫树分治按 topbit 分桶，O((n+q) log n) 总 / O(1) 单查） |
| `RMQ_Linear.cpp` | 线性 RMQ（OI-Wiki：笛卡尔树 + ±1 分块状压，O(n)-O(1)） |
| `SqrtTree.cpp` | Sqrt Tree（静态区间可结合查询，O(log log n)） |
| `LCT.cpp` | Link-Cut Tree（动态树：link/cut/makeroot/路径查询，均摊 O(log n)） |
| `LCT_Monoid.cpp` | 泛型半群 LCT（任意可逆幺半群路径乘积；修复原稿 find 下放顺序与哨兵污染） |
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

### Graph/（22 个）

| 文件 | 内容 |
|---|---|
| `Dinic.cpp` | 最大流（vector 版 `dinic<T>`，当前弧优化） |
| `MCMF.cpp` | 最小费用最大流（SPFA + 沿最短路推流） |
| `Flow_Bounds.cpp` | 上下界网络流（无源汇可行流 + 有源汇最大/最小流，基于 Dinic） |
| `HopcroftKarp.cpp` | 二分图最大匹配（O(E√V)） |
| `KM.cpp` | 二分图最大权完美匹配（O(n³)，slack 优化） |
| `TarjanSCC.cpp` | 强连通分量 + 缩点 |
| `TwoSat.cpp` | 2-SAT（蕴含图 + 迭代 Tarjan SCC，接口对齐 ACL twosat：add_clause/satisfiable/answer + if/not/xor/eq） |
| `TarjanCutVertex.cpp` | 割点（low[y] >= dfn[x]，根节点特判） |
| `TarjanBridge.cpp` | 桥 / 边双连通分量（e-DCC，按边编号处理重边） |
| `TarjanBCC.cpp` | 点双连通分量 / 圆方树 |
| `PointDivide.cpp` | 点分治 / 动态点分树（重心 + 双 BIT 容斥；已补 LCA 稀疏表构建） |
| `KruskalWQS.cpp` | WQS 二分 + Kruskal（恰好 k 条特殊边的最小生成树） |
| `DominatorTree.cpp` | 支配树（Lengauer-Tarjan） |
| `KruskalRebuildTree.cpp` | Kruskal 重构树（瓶颈边 / 边权限制连通性） |
| `TriangleCount.cpp` | 三元环计数（重定向 O(m√m)） |
| `SPFA.cpp` | SPFA 最短路（SLF 优化，负环检测 + 差分约束系统） |
| `EulerPath.cpp` | 欧拉路 / 欧拉回路（Hierholzer 迭代，无向 + 有向） |
| `GeneralMatching.cpp` | 一般图最大匹配（带花树，O(n³)，对照 tourist 实现校准） |
| `DirectedMST.cpp` | 最小树形图（朱刘算法，O(nm)，缩环 do-while 修正版） |
| `MatrixTree.cpp` | Matrix-Tree 定理（无向生成树 / 有向外向树计数，mod 998244353） |
| `StoerWagner.cpp` | Stoer-Wagner 全局最小割（无向正权图，不指定源汇，O(nm + n²log n)） |
| `SteinerTree.cpp` | 最小斯坦纳树（连通 k 个关键点的最小边权和，子集 DP + Dijkstra） |

### String/（8 个）

| 文件 | 内容 |
|---|---|
| `Manacher.cpp` | 回文串（最长回文子串，可扩展计数） |
| `SuffixArray.cpp` | 后缀数组（倍增 + 基数排序）+ height |
| `SuffixAutomaton.cpp` | 后缀自动机 SAM（endpos 统计 / 本质不同子串） |
| `StringHash.cpp` | 字符串哈希（mod 2^61-1，随机种子 + shift 混淆） |
| `ZAlgorithm.cpp` | Z 函数 / 扩展 KMP（对齐 ACL z_algorithm，string 与泛型容器双版本，O(n)） |
| `PalindromicTree.cpp` | 回文树 / PAM（本质不同回文 + 出现次数统计，O(n)） |
| `Lyndon.cpp` | Lyndon 分解（Duval）+ 最小表示（循环同构最小起点） |
| `ACAutomaton.cpp` | AC 自动机（多模式串匹配，Trie + fail + 拓扑计数） |

### Math/（29 个）

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
| `NTT.cpp` | NTT + 多项式全家桶（convolution / inv / ln / exp / deri / inte；固定模最快） |
| `Poly_fstdlib.cpp` | fstdlib 多项式库（固定模 NTT + 任意模 MTT（m_poly）/ inv / log / exp；功能更全） |
| `Min25.cpp` | Min25 筛（积性函数前缀和，f(p) 为多项式；已补 power_sum 依赖并修 prime 越界） |
| `CRT.cpp` | 中国剩余定理 CRT/exCRT（非互质模数可合并，对齐 ACL crt，无解返回 {0,0}；附 exgcd/inv_gcd） |
| `FloorSum.cpp` | 类欧几里得 floor_sum（Σ⌊(ai+b)/m⌋，对齐 ACL，支持负 a/b，O(log)） |
| `XORBasis.cpp` | 线性基（插入 / 表出判断 / 最值 / 第 k 小 / ≤x 计数 / 合并） |
| `DiscreteLog.cpp` | BSGS / exBSGS 离散对数（最小解，giant 步用 a^{-k}） |
| `QuadraticResidue.cpp` | 二次剩余 Cipolla（模奇素数开平方） |
| `PrimitiveRoot.cpp` | 原根（最小原根 / 全体原根） |
| `Lucas.cpp` | Lucas / exLucas（大组合数取模，素数与任意模数） |
| `Cantor.cpp` | 康托展开 / 逆康托展开（BIT O(n log n)） |
| `DuSieve.cpp` | 杜教筛（Σφ、Σμ 前缀和，O(n^{2/3})） |
| `LinearRecurrence.cpp` | Berlekamp-Massey + Kitamasa（线性递推第 k 项，O(d²log k)） |
| `Simpson.cpp` | 自适应辛普森积分（Richardson 外推） |
| `SubsetConvolution.cpp` | 子集卷积（h[S]=Σ_{T⊆S} f[T]g[S\T]，popcount 分层 + OR 的 zeta/Möbius，O(n²2^n)） |
| `Pell.cpp` | Pell 方程 x²-Dy²=1 基本解（√D 连分数周期 + 收敛分数） |
| `NimProduct.cpp` | Nim 积（nimber 乘法，最大费马 2 幂分治，64 位） |

### Misc/（11 个）

| 文件 | 内容 |
|---|---|
| `Bitset.cpp` | bitset 技巧（26 位通配符字符串匹配） |
| `HashTable_PBDS.cpp` | 哈希表实现一：gp_hash_table + splitmix64（抗卡） |
| `HashTable_Chain.cpp` | 哈希表实现二：手写链式引用计数表（槽位池回收） |
| `HashMap_OpenAddress.cpp` | 哈希表实现三：开放寻址线性探测 + backward-shift 删除（实测 1M 插查约为 unordered_map 的 3 倍速） |
| `DSUonTree.cpp` | 树上启发式合并（DSU on tree，子树统计） |
| `SimulatedAnnealing.cpp` | 模拟退火（通用模板，连续/离散优化） |
| `ExpressionEval.cpp` | 表达式求值（递归下降，+ - * / % ^ 括号 一元负号 变量） |
| `MoAlgorithm.cpp` | 莫队算法（普通 + 带修改，区间不同数示例，奇偶块优化） |
| `CDQDivide.cpp` | CDQ 分治（三维偏序可比对计数，O(n log² n)） |
| `ParallelBinarySearch.cpp` | 整体二分（离线静态区间第 k 小，值域二分 + BIT 分流） |
| `RollbackMo.cpp` | 回滚莫队（不删除莫队，区间相同数最远距离，右端永久 + 左端回滚） |

### Geometry/（3 个）

| 文件 | 内容 |
|---|---|
| `Geometry_2D.cpp` | 二维基础（整点精确运算：凸包 / 旋转卡壳直径 / 最近点对 / 点在多边形 / 线段判交） |
| `Geometry_HalfPlane.cpp` | 半平面交（S&I 单调队列，O(n log n)，附面积计算） |
| `Geometry_MinCircle.cpp` | 最小圆覆盖（随机增量，期望 O(n)，附两点/三点定圆） |

### Trash/（23 个，已移除）

因"代码简单 / 思想基础 / OI 常见 / 不易出错"而移除：
`FenwickTree`、`DSU`、`SparseTable`、`SegmentTreeLazy`、`SegmentTreeIterative`、
`Dijkstra`、`LCA_BinaryLifting`、`LCA_EulerTourRMQ`、`HLD`、`VirtualTree`、
`KMP`、`ZFunction`、`RollingHash`、`Matrix`、`Combinatorics`、`EulerSieve`、`Exgcd`、
`SlopeOptimization`、`TreeReroot`、`MonotoneQueue`、`DigitDP`、`FastIO`、`Random`、`LogTrick`

移除原因分类：
- 常见/基础：FenwickTree、DSU、SparseTable、Dijkstra、LCA×2、HLD、Matrix、Combinatorics、EulerSieve、Exgcd、FastIO
- 难以封装/强藕连：KMP、ZFunction、VirtualTree（Z 函数现已按 ACL 对齐补录为 `String/ZAlgorithm.cpp`）
- 结构简单但可拓展（按需自写）：SlopeOptimization、TreeReroot、MonotoneQueue、DigitDP、SegmentTreeLazy、SegmentTreeIterative
- 内容重复/被包含：RollingHash（hash_t 与 StringHash 重复）、Random（splitmix64 与 HashTable_PBDS 重复）、LogTrick（gcd 段合并被 ODT 包含）

## 说明

- 全部模板在 `g++ -std=c++14 -O2` 下编译通过（含 Trash/）；结构化绑定等少量 C++17 语法
  在 -std=c++14 下仅产生 warning（-Wc++17-extensions）不影响运行，`__lg` / `__int128` 为
  GNU 扩展。编译示例：`g++ -std=c++14 -O2 文件.cpp -c`。
- 模板库经 ASan + UBSan 随机压力对拍矩阵验证（46 项数据结构/图论/数学模板 + 新增 5 项），
  合法使用下无未定义行为 / 数组越界 / 读写已释放内存。
- 数组大小（`N`、`M`）多为原文常量，使用时按题目调整。
- `#define int long long` 的文件注意 `memset`/`0x3f` 对 64 位 int 的效果。
- 同算法的不同实现按文件名后缀区分：平衡树（Treap / Splay / WBST_Seq / WBST_Set）、
  GCD（Binary / Table）、哈希表（PBDS / Chain），按题目环境与偏好选用。

## 常数优化记录（2026-08）

对主库做过一轮系统的常数优化（不改接口与复杂度，新旧版本随机数据对拍全部一致）：

| 文件 | 手法 | 实测提速* |
|---|---|---|
| `Math/BigInt.cpp` | 字符串构造按 9 位一段乘加（O(n²)→O(n²/9) 次大数乘加） | x3.8 |
| `Math/PollardRho.cpp` | rho 差值连乘 128 步做一次 gcd（摊薄逐步 gcd 的除法） | x6.5 |
| `Graph/KruskalWQS.cpp` | 特殊/普通边各排序一次，每轮 WQS 二分线性归并（不再每轮整体 sort） | x1.6 |
| `Math/FWT.cpp` | 内层 `% md` 全部换条件加减 | x1.45 |
| `Graph/Flow_Bounds.cpp` | Dinic BFS 换扁平数组队列 + `nv` 收缩 memset/memcpy 到实际点数 | x1.3 |
| `Math/GaussJordan.cpp` | 消元内层两次取模减为一次 + 条件减 | x1.27 |
| `Math/MatrixMaxPlus.cpp` | (max,+) 乘法改 i-k-j 序 + 行指针（cache 友好，大矩阵显著） | x1.14 |
| `Graph/HopcroftKarp.cpp` | BFS 数组队列 + vis 时间戳代替每阶段 memset | x1.1 |
| `Math/NTT.cpp` | Barrett 约减替代 `% Mod`；`inte` 换线性逆元表；删死代码 | x1.04** |
| `String/SuffixArray.cpp` | 倍增排序去掉 pair 构造，第二关键字内联比较 | x1.06 |
| `Graph/Dinic.cpp`、`Graph/MCMF.cpp` | `std::queue` 换数组/环形队列，层号外提，`nv` 收缩 memset | x1.0~1.1 |
| `String/ACAutomaton.cpp` | ac_build 复用全局数组队列（省 8MB 且免链表开销） | 持平 |
| `DataStructure/GlobalBST.cpp` | `point_set` 的 1.6MB 栈上 tmp 数组改全局 | 持平 |
| `DataStructure/WaveletMatrix_Dynamic.cpp` | build/查询提行指针、`cur.swap(nxt)` 免拷贝 | 持平 |
| `DataStructure/Bitset_Dynamic.cpp` | `&=` 拆两段循环免逐元素边界判断、去掉冗余 norm | 持平 |
| `Graph/TarjanBCC.cpp`、`KruskalRebuildTree.cpp` | 倍增表构造越顶即止（浅树免空转 19 层，值不变） | 持平~ |
| `DataStructure/KDT_BinaryGroup.cpp` | 比较器不读全局 dim（消每次比较的访存+分支） | 持平 |

\* g++ 12.2 `-O2`、随机负载、3~7 次取最小值；数字仅供量级参考。
\** 本机 g++ 12 已把常量 `% 998244353` 自动编成乘法序列，故 Barrett 收益体现在老编译器/OJ 环境；
逆元表与死代码清理是净收益。平衡树/LCT/TopTree/猫树/Heaps 等原本即为数组实现的紧凑形态，未改动。

已知环境问题（非本轮引入）：`String/SuffixAutomaton.cpp` 的 `link`、`DataStructure/TopTree.cpp` 的
`chroot` 在部分 glibc 环境与全局符号冲突，遇到编译错误时改名即可；`RangeSemigroup_ACK.cpp` 需 `-std=c++20`。

## C++14 兼容与安全修复记录（2026-08 第二轮）

目标：全库 `g++ -std=c++14 -O2` 编译运行、合法使用下无 UB / 越界（含 string 越界、
模板实例化后才暴露的问题）。已用 104 个文件全量编译 + 51 项 ASan/UBSan 随机对拍验证。

**编译修复**：
- `std::gcd`（C++17）→ `__gcd`（GNU 扩展）：PollardRho / Gcd_Table / Trash::Exgcd
- `ModInt_Full`：constexpr lambda（C++17）→ constexpr 成员函数（`-c` 不实例化时被掩盖，实例化即炸）
- `SuffixAutomaton::link`、`TopTree::chroot` 与 glibc 全局符号冲突 → 改名 `lnk` / `crt`
- `RangeSemigroup_ACK`：span / requires / auto 模板参（C++20）→ 数组+计数 / 函数指针（C++14 重写）
- `SegmentTree_Semigroup`：文件尾注释块缺 `*/` 无法编译

**运行期 bug 修复（ASan/UBSan 或对拍发现）**：
- `SegmentTree_Semigroup`：`pool[u].lc = new_node()` 求值顺序未指定 → 扩容后写已释放块（UAF）；
  改为先取返回值再写成员
- `PointDivide`：LCA 稀疏表 `mi[d][x]`（d>=1）从未构建 → dist/path 统计全错；补惰性构建
- `RMQ_Linear`：欧拉序长 2n，块表（stp/lg2/bmask/mnv/mnp）按 N/B 声明会越界 → 扩至 2N/B
- `ExpressionEval`：parse_pri 增加越界防御行
- 新收录代码的修复见各文件尾注（Min25 的 prime 越界读、HashMap 主模板缺失与 erase 截断
  探测链、LCT_Monoid 的 find 下放顺序颠倒与空儿子哨兵污染）

**新增收录（用户提供，相似实现并存择优）**：
- `Math/Min25.cpp`（新算法）、`DataStructure/MonoidOfflineQuery.cpp`（离线形态与 CatTree 互补）、
  `DataStructure/LCT_Monoid.cpp`（泛型版与 LCT.cpp 并存）、`Math/Poly_fstdlib.cpp`
  （任意模 MTT 与 NTT.cpp 并存：固定模用 NTT.cpp 更快，任意模用 fstdlib）、
  `Misc/HashMap_OpenAddress.cpp`（1M 插+查 40ms vs unordered_map 132ms；链式版与开放寻址
  并存，前者支持计数-回收语义）

**已验证接口语义说明**：Treap 的 erase 删除所有同值节点；YFastTrie 的 predecessor/successor
为严格前驱/后继（0 表示不存在，值域应从 1 起）；Splay 示例用法为 build(0, n+1) 含双哨兵。

## ACL 对齐记录（2026-08 第三轮）

以 AtCoder Library（v1.5）的 12 个公开模块为清单逐项核对，补齐缺失部分：

| ACL 模块 | 本库对应 | 本轮动作 |
|---|---|---|
| `lazy_segtree` | `SegmentTree_Semigroup.cpp` | **改为全程非递归**（动态开点保留：分割节点 + 双边界路径迭代分解，
  路径栈逆序回拉），并补齐 ACL 接口：`get/all_prod/max_right/min_left` 与 `init(n, a)` O(n) 向量建树 |
| `lazy_segtree`（静态形态） | `SegmentTree_ACL.cpp`（新增） | ACL 逐函数移植（set/get/prod/all_prod/apply/max_right/min_left）；
  静态满树不要求 `mapping(f,e())==e()`，支持区间仿射+区间和等依赖段长的作用——与动态版互补 |
| `twosat` | `Graph/TwoSat.cpp`（新增） | 蕴含图 + **迭代 Tarjan**（深图不爆栈），接口对齐 add_clause/satisfiable/answer |
| `z_algorithm` | `String/ZAlgorithm.cpp`（新增） | ACL 移植（z[0]=n 约定），string 与泛型容器双版本 |
| `math::crt` | `Math/CRT.cpp`（新增） | 非互质模数合并，无解返回 {0,0}；附 exgcd/inv_gcd 可复用 |
| `math::floor_sum` | `Math/FloorSum.cpp`（新增） | 类欧几里得辗转，支持负 a/b，内部无符号乘法对齐 ACL |
| `convolution` | `Math/NTT.cpp` / `Poly_fstdlib.cpp` | 已覆盖（固定模 / 任意模 MTT），未动 |
| `maxflow` / `mincostflow` | `Graph/Dinic.cpp` / `MCMF.cpp` | 已覆盖，未动 |
| `scc` | `Graph/TarjanSCC.cpp` | 已覆盖，未动 |
| `modint` | `Math/ModInt*.cpp` 三种 | 已覆盖（pow_mod/inv_mod 即其内建运算），未动 |
| `suffix_array`（+lcp_array） | `String/SuffixArray.cpp`（含 height） | 已覆盖，未动 |
| `segtree`（单点改普通版） | 退化用 `SegmentTree_ACL.cpp`（恒等作用）或 `SegmentTree_IterativeLazy.cpp` | 不单独收录（沿用本库裁剪原则） |
| `dsu` / `fenwicktree` / `sparse_table` | `Trash/`（DSU / FenwickTree / SparseTable） | 维持既定裁剪（保留备查），未动 |

验证：新增/重写模板经 `g++ -std=c++14 -O2` 与 ASan/UBSan 随机对拍——
半群线段树（非交换矩阵共轭作用，dense 40 组 + 1e9 值域稀疏 8 组，与旧递归版全量对拍一致）、
`SegmentTree_ACL`（区间仿射+和 / 非交换共轭 ×30 组）、TwoSat（300 组 vs 2^n 暴力 + 50 万链深栈测试）、
ZAlgorithm（400 组 vs O(n²)）、CRT（2000 组 vs 逐点枚举，含负余数/非互质）、
floor_sum（3000 组 vs 逐项下取整 + 大数恒等式 500 组）。全部文件（109 个）`-std=c++14 -O2` 编译通过，
六个文件的尾部示例取消注释后编译运行输出与注释一致。

`max_right/min_left` 说明：与 ACL 相同的"规范块贪心"语义——整块满足即越过、失配块内二分下探，
适合判定条件随区间扩大单调失效的 g；非递归半群版沿叶子路径向上的规范块实现，动态未建子树按 e() 计。

## 对照主流题库补全记录（2026-08 第四轮）

以 [lzyrapx/Algorithmic-Templates](https://github.com/lzyrapx/Algorithmic-Templates)、
[the-tourist/algo](https://github.com/the-tourist/algo)、
[ChenXingLing/OI-Algorithm-Template](https://github.com/ChenXingLing/OI-Algorithm-Template)
三库目录为清单逐项核对，补齐本库缺失的 26 个模板（含新开 Geometry/ 分类），
并保持与库内既有码风一致（`bits/stdc++.h` + 2 空格缩进 + 紧凑语句 + 文件尾注释块 + 可取消注释的示例）。

**补全清单**：数学 9（线性基 / BSGS·exBSGS / Cipolla 二次剩余 / 原根 / Lucas·exLucas /
康托展开 / 杜教筛 / BM+Kitamasa 线性递推 / 辛普森积分）、数据结构 5（李超线段树 / 主席树 /
线段树合并 / 可持久化 01-Trie / 带权+可撤销并查集）、图论 5（SPFA+差分约束 / 欧拉路 /
带花树一般图匹配 / 朱刘最小树形图 / Matrix-Tree）、字符串 2（回文树 / Lyndon 分解+最小表示）、
杂项 2（莫队 / CDQ 三维偏序）、几何 3（二维基础 / 半平面交 / 最小圆覆盖）。

**已覆盖未重复收录**（与三库对照后维持现状）：DLX、回文树已有 SAM 互补形态、Floyd、
prim、K 短路（A*）、匈牙利算法（已有 HopcroftKarp/KM）、Kosaraju（已有 TarjanSCC）、
杜教筛已含 Min25 互补、Bernoulli 数等小工具按库内裁剪原则不单列。

**验证**（全部通过 ASan/UBSan 随机对拍）：
- 数论组：线性基 400 组 vs 子集枚举（含第 k 小 / ≤x 计数）、BSGS/exBSGS 500 组 vs 逐幂枚举、
  Cipolla 9 素数×200 组 vs Euler 准则+暴力、原根 9 素数（阶验证）、Lucas/exLucas 300 组 vs
  `__int128` 精确组合数、康托 200 组 vs 全排列枚举、杜教筛 vs 暴力 φ/μ、BM 200 组 vs
  直接递推到第 700+ 项
- 几何组：凸包/直径/点包含/线段判交/最近点对 300 组 vs 暴力、半平面交 200 组 vs
  Sutherland–Hodgman 逐边裁剪、最小圆覆盖 200 组 vs 三点枚举
- 数据结构组：李超 250 组 vs 线性表、主席树 200 组 vs 排序、线段树合并 200 组 vs 归并、
  带权/可撤销并查集 200 组 vs 暴力、01-Trie 200 组 vs 枚举
- 图论组：SPFA/负环/差分约束 400 组 vs Bellman-Ford + 值域枚举、欧拉路 300 组（随机游走
  造图保证有解）、带花树 300 组 vs mask DP、朱刘 300 组 vs 入边组合枚举、Matrix-Tree 250 组
  vs 生成树枚举
- 字符串/杂项组：PAM 300 组 vs 子串枚举、Lyndon 300 组（因子 Lyndon 性 + 非增性 + 最小表示）、
  莫队 150 组（含带修改）、CDQ 300 组 vs 可比对暴力

**对拍发现并修复的实现错误**（均已在模板中修正）：
- `Geometry_2D` 最近点对：2 元素基例未按 y 排序，破坏归并前提（部分数据漏解）
- `DSU_Weighted_Rollback` 带权合并：权值公式符号写反（`d[rx] = d[y] - d[x] - w` 才正确）
- `SPFA`：`init` 未清 `hd[0]`，差分约束超级源悬挂上一轮邻接表导致死循环
- `EulerPath`：`init` 未复位 `used[]`，多测复用边标记残留
- `GeneralMatching`：开花条件写反（奇环应为外层-外层相邻；白点为偶环跳过），对照
  tourist 实现重写
- `DirectedMST`：缩环标记循环首轮即 break（只标一个点导致死循环），改 do-while
- `PalindromicTree`：`build` 未拷贝输入串（读全零缓冲的 UB）
- `CDQDivide`：仅按 a 稳定排序会漏计"a 相等且后输入者为支配者"的可比对；改按 (a,b,c)
  全序排序后恰为无序可比对计数
- `XORBasis`：`b[]` 缺省未零初始化；`LinearRecurrence`：BM 递推系数漏取负号
- 另修复旧文件 `LCT_Monoid` 示例中的 `'\\n'` 双反斜杠笔误

全库 135 个文件 `g++ -std=c++14 -O2` 编译通过；26 个新模板的尾部示例取消注释后
编译运行输出与注释一致。

## 对照 OI-Wiki 补全记录（2026-08 第五轮）

以 [OI-Wiki](https://oi-wiki.org/) 的算法目录为清单，选取"实现复杂 / 边界易错"且本库缺失的
9 个模板补入：全局最小割（Stoer-Wagner）、最小斯坦纳树、可持久化并查集、可持久化平衡树
（fhq-Treap）、子集卷积、Pell 方程、Nim 积、整体二分、回滚莫队。

**补全清单**：
- 图论 2：Stoer-Wagner 全局最小割、最小斯坦纳树（子集 DP + Dijkstra）
- 数据结构 2：可持久化并查集（线段树存 fa/siz + 按大小合并）、可持久化 fhq-Treap
- 数学 3：子集卷积（popcount 分层 + OR 的 zeta/Möbius）、Pell 方程基本解（连分数）、
  Nim 积（最大费马 2 幂分治，64 位 nimber）
- 杂项 2：整体二分（区间第 k 小离线）、回滚莫队（不删除莫队）

**易错点说明（均已在模板中正确处理）**：
- Stoer-Wagner：cut-of-phase 的 s/t 取"倒数第二/最后加入"的点；合并时边权
  `edge[s][j] += edge[j][t]` 需双向叠加；图不连通提前返回 0
- 斯坦纳树：dp 以 INF 初始化、关键点 `dp[key][1<<i]=0`；必须先子树合并再 Dijkstra 松弛，
  顺序不可颠倒
- 可持久化并查集：**不能路径压缩**（会破坏历史版本共享的结构），只能按大小合并保证
  O(log) 树高；find 沿持久化线段树逐层点查
- 可持久化 Treap：split/merge 沿途"写时复制"，历史根永不被改；erase 仅删一个同值节点
- 子集卷积：必须按 popcount 分层再卷积，直接 FWT 会漏掉"恰好不相交"的限制；每层逆变换后
  取回 `H[popcount(S)][S]`
- Pell 方程：√D 连分数周期以 `a == 2a0` 判定；周期奇偶决定取第 L-1 还是 2L-1 个收敛分数；
  中间乘积需 `__int128`，D 过大导致基本解超出 long long（如 D=277、D=1021）需换高精度
- Nim 积：按 mex 定义或按"最低 2 幂位"拆分会指数级递归（单个 64 位平方可达 ~3e5 次调用）；
  须按最大费马 2 幂 F(k)=2^{2^k} 分治（m⊗m = 3m/2、m⊗z = m·z），递归深度 ≤ 5
- 整体二分：BIT 只统计值域 [L, mid] 的位置并每层回滚；k 要减去左半贡献；值域需离散化
- 回滚莫队：同块内 r 只增不减（禁用奇偶优化）；左指针的临时加入必须**逆序**回滚 first[]；
  询问完全落在块内时暴力 O(√n)

**验证**（9 个新模板全部通过 ASan/UBSan 随机对拍）：
- Stoer-Wagner 3000 组 vs 枚举源汇 Dinic 最小割（含不连通图）
- 斯坦纳树 3000 组 vs 枚举非关键点子集 Prim（先造生成树保证连通）
- 可持久化并查集 300 轮随机合并/询问 + 全版本 find 对拍 vs 逐版本拷贝
- 可持久化 Treap 60 轮 × 2000 步随机 ins/erase/kth/rnk/前驱/后继 vs `vector<multiset>`
- 子集卷积 200 组 vs O(4^n) 枚举子集
- Pell 254 个非平方 D（≤270）：回代 x²-Dy²=1 + 暴力最小性；另交叉验证 D=2/3/5/13/61
- Nim 积：≤31 全表 vs mex 定义 + 5000 组 64 位域公理（交换/结合/分配/单位元）+ 1000 组求逆
- 整体二分 300 组 vs 排序取第 k 小（含负值与重复值）
- 回滚莫队 300 组 vs O(n²) 暴力

全库 144 个文件（含 Trash/）`g++ -std=c++14 -O2` 编译通过；9 个新模板的尾部示例取消注释后
编译运行输出与注释一致。
