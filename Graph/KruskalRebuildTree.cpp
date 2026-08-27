#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int N = 2e5 + 9, M = 5e5 + 9;
struct edge {
  int u, v, w;
  bool operator<(const edge& r) const { return w < r.w; }
} e[M];
int n, m, fa[N], tot, val[N];
vector<int> T[N];

int get(int x) { return x == fa[x] ? x : fa[x] = get(fa[x]); }

void kruskal_rebuild() {
  sort(e + 1, e + m + 1);
  tot = n;
  for (int i = 1; i <= 2 * n; ++i) fa[i] = i;
  for (int i = 1; i <= m; ++i) {
    int u = get(e[i].u), v = get(e[i].v);
    if (u == v) continue;
    ++tot;
    fa[u] = fa[v] = tot;
    T[tot].push_back(u), T[tot].push_back(v);
    val[tot] = e[i].w;
    if (tot == 2 * n - 1) break;
  }
}

int f[N][20], dep[N];
void dfs_rebuild(int x, int ff) {
  f[x][0] = ff, dep[x] = dep[ff] + 1;
  for (int i = 1; f[x][i - 1]; ++i) f[x][i] = f[f[x][i - 1]][i - 1];  // 越顶即止（默认 0）
  for (int y : T[x])
    if (y != ff) dfs_rebuild(y, x);
}

int lca(int x, int y) {
  if (dep[x] < dep[y]) swap(x, y);
  for (int i = 19; ~i; --i)
    if (dep[f[x][i]] >= dep[y]) x = f[x][i];
  if (x == y) return x;
  for (int i = 19; ~i; --i)
    if (f[x][i] != f[y][i]) x = f[x][i], y = f[y][i];
  return f[x][0];
}

/*
 * ============================================================
 * 名称：Kruskal 重构树
 * 复杂度：建树 O(m log m)（排序）+ O(m α)；预处理 O(n log n)；查询 O(log n)
 * 用途：最小生成树（按边权升序建）的"瓶颈"性质：
 *       1) u、v 之间所有路径中，最大边权最小的一条路径，其最大边权
 *          = 重构树上 lca(u, v) 的权值 val[lca]
 *       2) 从 u 出发只走边权 <= w 的边能到达的点集 = 重构树上 val <= w 的
 *          最高祖先的整棵子树（配合倍增 + 子树大小做"边权限制连通性"题）
 *       3) 按边权降序建（改排序方向）则对应最小瓶颈 / 最大生成树语义
 * 原理：Kruskal 合并两个连通块时新建虚点（权值 = 当前边权）作为两者父亲，
 *       得到一棵 2n-1 个节点的二叉堆树：叶子为原图点，虚点权值单调
 * 注意：图可能不连通 → 建出的重构森林，需对每个根分别 dfs_rebuild；
 *       原图点权值 val[1..n] = 0（不参与比较）
 * 用法：kruskal_rebuild(); 对每个根（fa[i] == i）dfs_rebuild(i, 0);
 *       查询瓶颈值 = val[lca(u, v)]
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   cin >> n >> m;
 *   for (int i = 1, u, v, w; i <= m; ++i) cin >> e[i].u >> e[i].v >> e[i].w;
 *   kruskal_rebuild();
 *   for (int i = 1; i <= tot; ++i)
 *     if (fa[i] == i) dfs_rebuild(i, 0);
 *   int q;
 *   cin >> q;
 *   while (q--) {
 *     int u, v;
 *     cin >> u >> v;
 *     cout << val[lca(u, v)] << '\n';
 *   }
 * }
 * ============================================================
 */
