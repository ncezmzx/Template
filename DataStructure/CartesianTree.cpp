#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int N = 1e5 + 9;
int n, a[N], lc[N], rc[N], fa[N], stk[N];

void build() {
  int tp = 0;
  for (int i = 1; i <= n; ++i) {
    int last = 0;
    while (tp && a[stk[tp]] > a[i]) last = stk[tp--];
    if (tp) rc[stk[tp]] = i, fa[i] = stk[tp];
    lc[i] = last;
    if (last) fa[last] = i;
    stk[++tp] = i;
  }
}

/*
 * ============================================================
 * 名称：笛卡尔树（Cartesian Tree，小根堆型，单调栈 O(n) 构建）
 * 复杂度：O(n)
 * 用途：给定序列 a[1..n]，构造二叉树满足：
 *       1) 中序遍历 = 原序列（位置作为键，保持相对顺序）
 *       2) 堆性质：父亲的值 < 孩子的值（小根型；改 > 号即大根型）
 *       经典应用：
 *       - RMQ：区间 [l, r] 最小值 = lca(l, r) 的权值（对笛卡尔树做 LCA 预处理）
 *       - 直方图最大矩形 / 最大子矩阵：每个节点为根的子树区间即"以 a[x] 为
 *         最小值的最长区间"，区间长度 = sz[x]（可求），矩形面积 = a[x] * sz[x]
 *       - 与 Treap 的关系：固定键（位置）与值（优先级）的 Treap 即笛卡尔树
 * 原理：单调栈维护"当前最右链"；新元素不断弹出比它大的栈顶，弹出的链成为
 *       新元素的左子树，新元素挂到新的栈顶右侧
 * 注意：值相同的元素需自定义比较（如位置序）避免歧义；
 *       构建后根为 fa 为 0 的节点（栈底）；空节点 lc/rc/fa 均为 0
 * 用法：n、a 就绪后 build()；遍历 1..n 中 fa[x]==0 者即根
 * ============================================================
 * 使用示例（编译时取消注释；求每个位置作为最小值的最长区间）：
 * signed main() {
 *   cin >> n;
 *   for (int i = 1; i <= n; ++i) cin >> a[i];
 *   build();
 *   vector<int> sz(n + 1, 1);
 *   function<void(int)> dfs = [&](int x) {
 *     if (lc[x]) dfs(lc[x]), sz[x] += sz[lc[x]];
 *     if (rc[x]) dfs(rc[x]), sz[x] += sz[rc[x]];
 *   };
 *   for (int x = 1; x <= n; ++x)
 *     if (!fa[x]) { dfs(x); break; }
 *   for (int i = 1; i <= n; ++i) cout << a[i] * sz[i] << ' ';
 * }
 * ============================================================
 */
