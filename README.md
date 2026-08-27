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

全库共 81 个模板（DataStructure 46 / Graph 15 / String 5 / Math 15 / Misc 7）+ 本 README。

## 索引

### DataStructure/（32 个）

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

### Graph/（15 个）

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
| `PointDivide.cpp` | 点分治 / 动态点分树（重心 + 双 BIT 容斥；已补 LCA 稀疏表构建） |
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

### Math/（13 个）

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

### Misc/（7 个）

| 文件 | 内容 |
|---|---|
| `Bitset.cpp` | bitset 技巧（26 位通配符字符串匹配） |
| `HashTable_PBDS.cpp` | 哈希表实现一：gp_hash_table + splitmix64（抗卡） |
| `HashTable_Chain.cpp` | 哈希表实现二：手写链式引用计数表（槽位池回收） |
| `HashMap_OpenAddress.cpp` | 哈希表实现三：开放寻址线性探测 + backward-shift 删除（实测 1M 插查约为 unordered_map 的 3 倍速） |
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
