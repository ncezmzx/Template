/*
 * 名称：树上换根 DP（rerooting / 二次扫描换根）
 * 复杂度：O(n)（两遍 DFS）
 * 用途：给定一棵树与若干标记点（key[x] = 1），对每个点 x 求
 *       f[x] = sum_{标记点 u} dist(x, u)（所有标记点到 x 的距离和）。
 *       第一遍 dfs2 以任意根（如 1）做子树 DP，第二遍 dfs3 换根推出其余点的答案。
 * 换根公式：从 x 换到子节点 y 时，
 *       f[y] = f[x] + dst[y] * (m - 2 * sz[y])
 *   其中 sz[y] 为 y 子树内标记点个数，m 为标记点总数：
 *   子树内的 sz[y] 个标记点距离减少 dst[y]，其余 m - sz[y] 个增加 dst[y]。
 *   注：原实现运行在虚树（virtual tree）上，se[x] 为虚树的父子边表，
 *       dst[y] 为边权（原树上两点距离）；对普通树同样适用。
 * 使用示例（DEMO main）：输入 n m，m 个标记点编号，随后 n-1 条带权边；
 *       对每个点输出所有标记点到该点的距离和。
 * 来源：all.cpp 7568-7577（dfs2/dfs3 原样抽取；变量声明沿用原块 7488-7491）
 */
#include <bits/stdc++.h>
using namespace std;
#define int long long
constexpr int N = 4e5 + 9;
// 以下声明原样取自 all.cpp 7489-7491（原块为 HLD + 虚树题；本模板只用到 sz/key/dst/f/m/se）
int n, q, c[N], dfn[N], top[N], fa[N], sz[N], son[N], mi[20][N], dep[N], id[N], idx;
int m, p[N], a[N], key[N], cnt, dst[N], f[N];
vector<int> es[N], se[N];

// 第一遍：子树 DP。sz[x] = 子树内标记点个数，f[x] = 子树内标记点到 x 的距离和。
void dfs2(int x) {
  sz[x] = f[x] = key[x];
  for (int y : se[x])
    dfs2(y), f[x] += f[y] + dst[y] * sz[y], sz[x] += sz[y];
}
// 第二遍：换根。f[y] = f[x] + dst[y] * (m - 2 * sz[y])，然后清空虚树边表。
void dfs3(int x) {
  for (int y : se[x])
    f[y] = f[x] + dst[y] * (m - 2 * sz[y]), dfs3(y);
  se[x].clear();
}

#ifdef DEMO
// 使用示例：n 个点、m 个标记点、n-1 条带权边（普通树，非虚树）
// 输入：n m，下一行 m 个标记点编号，随后 n-1 行 u v w
vector<pair<int, int>> g[N];  // 仅 DEMO 用：带权邻接表
signed main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  cin >> n >> m;
  for (int i = 1; i <= m; ++i) {
    int x;
    cin >> x;
    key[x] = 1;
  }
  for (int i = 1, u, v, w; i < n; ++i) {
    cin >> u >> v >> w;
    g[u].push_back({v, w}), g[v].push_back({u, w});
  }
  // 建一棵以 1 为根的父子树（se[x] 存子节点，dst[y] 存边权）
  function<void(int, int)> build = [&](int x, int ff) {
    for (auto [y, w] : g[x])
      if (y != ff) se[x].push_back(y), dst[y] = w, build(y, x);
  };
  build(1, 0);
  dfs2(1);
  dfs3(1);
  for (int i = 1; i <= n; ++i) cout << f[i] << " \n"[i == n];
  return 0;
}
#endif
