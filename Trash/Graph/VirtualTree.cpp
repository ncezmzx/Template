#include <bits/stdc++.h>
using namespace std;

/*
 * 名称：虚树（Virtual Tree）
 * 复杂度：对 k 个关键点，排序 + 求相邻 lca 后 O(k log k) 建树
 * 用途：多次询问时，每次只保留关键点及其两两 lca 构成虚树（se[] 邻接表），
 *       在虚树上做树形 DP，避免每次 O(n) 遍历整棵树
 * 建树规则（原样）：按 dfn 排序；相邻点取 lca 加入；加入根节点 1；去重后
 *       se[lca(v[i], v[i+1])].push_back(v[i+1])，即每个点在虚树上的父亲是它与前驱的 lca。
 * 使用示例：
 *   dfs(1, 0);                              // 预处理 dfn/dep/倍增数组
 *   vector<int> v = build_virtual_tree(nodes);   // nodes 为本次询问的关键点
 *   ... 在 se[] 虚树上 DP（可给关键点打 imp 标记）...
 *   for (int i : v) se[i].clear();          // 用完清理
 * 注意：原代码固定把根 1 加入虚树，根不是 1 时请自行修改。
 * 来源：all.cpp 行 36532-36570（dfs/lca）、36618-36634（虚树构建，原样保留）
 */
#define int long long
constexpr int N = 1e5 + 9;
int n, dep[N], fa[N][20], dfn[N], idx;
vector<int> es[N], se[N];          // es: 原树；se: 虚树
void dfs(int x, int ff) {
  fa[x][0] = ff, dep[x] = dep[ff] + 1, dfn[x] = ++idx;
  for (int i = 1; i < 20; ++i) fa[x][i] = fa[fa[x][i - 1]][i - 1];
  for (int y : es[x])
    if (y ^ ff) dfs(y, x);
}
int lca(int x, int y) {            // 倍增 LCA
  for (int i = 19; ~i; --i) {
    if (dep[fa[x][i]] >= dep[y]) x = fa[x][i];
    if (dep[fa[y][i]] >= dep[x]) y = fa[y][i];
  }
  if (x == y) return x;
  for (int i = 19; ~i; --i)
    if (fa[x][i] != fa[y][i]) x = fa[x][i], y = fa[y][i];
  return fa[x][0];
}
vector<int> build_virtual_tree(vector<int> v) {
  sort(v.begin(), v.end(), [&](int x, int y) -> bool { return dfn[x] < dfn[y]; });
  for (int i = 0; i + 1 < (int)v.size(); ++i) v.push_back(lca(v[i], v[i + 1]));  // 相邻点 lca
  v.push_back(1);                                                              // 根节点
  sort(v.begin(), v.end(), [&](int x, int y) -> bool { return dfn[x] < dfn[y]; });
  v.erase(unique(v.begin(), v.end()), v.end());
  for (int i = 0; i + 1 < (int)v.size(); ++i) se[lca(v[i], v[i + 1])].push_back(v[i + 1]);  // 连虚树边
  return v;    // 返回虚树包含的所有节点（含 lca 与根），用于事后清理
}

#ifdef DEMO
signed main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  cin >> n;
  for (int i = 1, x, y; i < n; ++i) {
    cin >> x >> y;
    es[x].push_back(y), es[y].push_back(x);
  }
  dfs(1, 0);
  int q;
  cin >> q;
  while (q--) {
    int k;
    cin >> k;
    vector<int> v(k);
    for (int i = 0; i < k; ++i) cin >> v[i];
    v = build_virtual_tree(v);
    // 在 se[] 虚树上做树形 DP ...
    for (int i : v) se[i].clear();   // 清理虚树，避免影响下一次询问
  }
}
#endif
