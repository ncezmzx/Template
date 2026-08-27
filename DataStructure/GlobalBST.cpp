#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int N = 2e5 + 9;
constexpr int INF = 0x3f3f3f3f3f3f3f3f;
int n, m, val[N];
vector<int> es[N];

int fa[N], dep[N], sz[N], son[N], top[N], dfn[N], id[N], idx, light[N];

void dfs1(int x, int ff) {
  fa[x] = ff, dep[x] = dep[ff] + 1, sz[x] = 1;
  for (int y : es[x])
    if (y != ff) {
      dfs1(y, x), sz[x] += sz[y];
      if (sz[y] > sz[son[x]]) {
        if (son[x]) light[x] += sz[son[x]];
        son[x] = y;
      } else light[x] += sz[y];
    }
}

void dfs2(int x, int tp) {
  top[x] = tp, dfn[x] = ++idx, id[idx] = x;
  if (son[x]) dfs2(son[x], tp);
  for (int y : es[x])
    if (y != fa[x] && y != son[x]) dfs2(y, y);
}

int ls[N], rs[N], f[N], mx[N], tag[N], w[N], Lp[N], Rp[N], root[N];

void up(int x) {
  Lp[x] = ls[x] ? Lp[ls[x]] : dfn[x];
  Rp[x] = rs[x] ? Rp[rs[x]] : dfn[x];
  int lv = ls[x] ? mx[ls[x]] : -INF;
  int rv = rs[x] ? mx[rs[x]] : -INF;
  mx[x] = max(val[x], max(lv, rv));
}

void apply(int x, int d) {
  if (x) val[x] += d, mx[x] += d, tag[x] += d;
}

void pushdown(int x) {
  if (tag[x]) apply(ls[x], tag[x]), apply(rs[x], tag[x]), tag[x] = 0;
}

int build_chain(int l, int r) {
  if (l > r) return 0;
  int tot = 0;
  for (int i = l; i <= r; ++i) tot += w[id[i]];
  int acc = 0, mid = l;
  for (int i = l; i <= r; ++i) {
    acc += w[id[i]];
    if (acc * 2 >= tot) {
      mid = i;
      break;
    }
  }
  int x = id[mid];
  ls[x] = build_chain(l, mid - 1);
  rs[x] = build_chain(mid + 1, r);
  if (ls[x]) f[ls[x]] = x;
  if (rs[x]) f[rs[x]] = x;
  up(x);
  return x;
}

void gbst_build() {
  for (int h = 1; h <= n; ++h)
    if (!fa[h] && top[h] == 0) dfs1(h, 0), dfs2(h, h);
  for (int h = 1; h <= n; ++h)
    if (top[h] == h) {
      int len = 0;
      for (int x = h; x; x = son[x]) ++len;
      for (int i = 0; i < len; ++i) w[id[dfn[h] + i]] = light[id[dfn[h] + i]] + 1;
      root[h] = build_chain(dfn[h], dfn[h] + len - 1);
    }
}

void range_add(int x, int l, int r, int d) {
  if (!x || r < Lp[x] || Rp[x] < l) return;
  if (l <= Lp[x] && Rp[x] <= r) return apply(x, d), void();
  pushdown(x);
  if (l <= dfn[x] && dfn[x] <= r) val[x] += d;
  range_add(ls[x], l, r, d), range_add(rs[x], l, r, d);
  up(x);
}

int range_max(int x, int l, int r) {
  if (!x || r < Lp[x] || Rp[x] < l) return -INF;
  if (l <= Lp[x] && Rp[x] <= r) return mx[x];
  pushdown(x);
  int res = -INF;
  if (l <= dfn[x] && dfn[x] <= r) res = val[x];
  return max(res, max(range_max(ls[x], l, r), range_max(rs[x], l, r)));
}

void path_add(int u, int v, int d) {
  while (top[u] != top[v]) {
    if (dep[top[u]] < dep[top[v]]) swap(u, v);
    range_add(root[top[u]], dfn[top[u]], dfn[u], d);
    u = fa[top[u]];
  }
  if (dep[u] > dep[v]) swap(u, v);
  range_add(root[top[u]], dfn[u], dfn[v], d);
}

int path_max(int u, int v) {
  int res = -INF;
  while (top[u] != top[v]) {
    if (dep[top[u]] < dep[top[v]]) swap(u, v);
    res = max(res, range_max(root[top[u]], dfn[top[u]], dfn[u]));
    u = fa[top[u]];
  }
  if (dep[u] > dep[v]) swap(u, v);
  res = max(res, range_max(root[top[u]], dfn[u], dfn[v]));
  return res;
}

void point_set(int x, int v) {
  int tmp[N], tp2 = 0;
  for (int y = x; y; y = f[y]) tmp[++tp2] = y;
  while (tp2) pushdown(tmp[tp2--]);
  val[x] = v;
  for (int y = x; y; y = f[y]) up(y);
}

/*
 * ============================================================
 * 名称：全局平衡二叉树（Global BST，静态树路径操作）
 * 复杂度：预处理 O(n log n)；路径加/路径最大值/单点修改 O(log n) 均摊
 * 用途：静态树的路径修改与查询（本模板：路径加 + 路径最大值 + 单点改），
 *       相比"树链剖分 + 线段树"的 O(log^2 n)，链内操作用带权平衡 BST
 *       实现 O(log n)；改 up/apply 的聚合可扩展和、异或等信息
 * 原理：树链剖分后，每条重链按"轻子树大小和 + 1"为权建带权中位点的平衡
 *       BST（中序遍历 = 链序，子树覆盖链上连续 dfn 区间，存 Lp/Rp）；
 *       链内区间操作按子树区间覆盖递归（O(log n)）；路径操作 = 树剖跳链
 *       + 链内 BST 区间操作；带权中位点保证每条链 BST 高度 O(log n)，
 *       且轻链跳跃总次数受势能约束
 * 注意：森林需对每个根 dfs1/dfs2（gbst_build 已处理）；点权版本（边权可
 *       下放到儿子点）；up 中 Lp/Rp 依赖"中序 = dfn 序"的性质
 * ============================================================
 * 使用示例（编译时取消注释；路径加 + 路径最大值 + 单点改）：
 * signed main() {
 *   cin >> n >> m;
 *   for (int i = 1; i <= n; ++i) cin >> val[i];
 *   for (int i = 1, u, v; i < n; ++i) {
 *     cin >> u >> v;
 *     es[u].push_back(v), es[v].push_back(u);
 *   }
 *   gbst_build();
 *   while (m--) {
 *     int o, u, v;
 *     cin >> o >> u >> v;
 *     if (o == 1) {
 *       int d;
 *       cin >> d;
 *       path_add(u, v, d);
 *     }
 *     if (o == 2) cout << path_max(u, v) << '\n';
 *     if (o == 3) point_set(u, v);
 *   }
 * }
 * ============================================================
 */
