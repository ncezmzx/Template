#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int N = 2e5 + 9;
constexpr int INF = 0x3f3f3f3f3f3f3f3f;
int n, val[N];
vector<int> es[N];

int fa[N], dep[N], sz[N], son[N], top[N], dfn[N], id[N], idx, tail[N];

void dfs1(int x, int ff) {
  fa[x] = ff, dep[x] = dep[ff] + 1, sz[x] = 1;
  for (int y : es[x])
    if (y != ff) {
      dfs1(y, x), sz[x] += sz[y];
      if (sz[y] > sz[son[x]]) son[x] = y;
    }
}

void dfs2(int x, int tp) {
  top[x] = tp, dfn[x] = ++idx, id[idx] = x, tail[tp] = x;
  if (son[x]) dfs2(son[x], tp);
  for (int y : es[x])
    if (y != fa[x] && y != son[x]) dfs2(y, y);
}

int ch[N][2], f[N], Lp[N], Rp[N];
int smx[N], rake[N], ck[N];
int chroot[N];

void pull(int x) {
  smx[x] = max(val[x], max(ch[x][0] ? smx[ch[x][0]] : -INF, ch[x][1] ? smx[ch[x][1]] : -INF));
  ck[x] = max({smx[x], rake[x], ch[x][0] ? ck[ch[x][0]] : -INF, ch[x][1] ? ck[ch[x][1]] : -INF});
  Lp[x] = ch[x][0] ? Lp[ch[x][0]] : dfn[x];
  Rp[x] = ch[x][1] ? Rp[ch[x][1]] : dfn[x];
}

int build_chain(int l, int r) {
  if (l > r) return 0;
  int m = (l + r) >> 1, x = id[m];
  ch[x][0] = build_chain(l, m - 1);
  ch[x][1] = build_chain(m + 1, r);
  if (ch[x][0]) f[ch[x][0]] = x;
  if (ch[x][1]) f[ch[x][1]] = x;
  pull(x);
  return x;
}

void toptree_build() {
  for (int h = 1; h <= n; ++h)
    if (!fa[h] && top[h] == 0) dfs1(h, 0), dfs2(h, h);
  for (int i = 1; i <= n; ++i) rake[i] = -INF;
  vector<int> heads;
  for (int h = 1; h <= n; ++h)
    if (top[h] == h) heads.push_back(h);
  sort(heads.begin(), heads.end(), [&](int a, int b) { return dep[a] > dep[b]; });
  for (int h : heads) {
    for (int x = h; x; x = son[x])
      for (int y : es[x])
        if (y != fa[x] && y != son[x]) rake[x] = max(rake[x], ck[chroot[y]]);
    int len = 0;
    for (int x = h; x; x = son[x]) ++len;
    chroot[h] = build_chain(dfn[h], dfn[h] + len - 1);
  }
}

int ck_query(int x, int l, int r) {
  if (!x || r < Lp[x] || Rp[x] < l) return -INF;
  if (l <= Lp[x] && Rp[x] <= r) return ck[x];
  int res = -INF;
  if (l <= dfn[x] && dfn[x] <= r) res = max(val[x], rake[x]);
  return max(res, max(ck_query(ch[x][0], l, r), ck_query(ch[x][1], l, r)));
}

int pt_query(int x, int l, int r) {
  if (!x || r < Lp[x] || Rp[x] < l) return -INF;
  if (l <= Lp[x] && Rp[x] <= r) return smx[x];
  int res = (l <= dfn[x] && dfn[x] <= r) ? val[x] : -INF;
  return max(res, max(pt_query(ch[x][0], l, r), pt_query(ch[x][1], l, r)));
}

int path_query(int u, int v) {
  int res = -INF;
  while (top[u] != top[v]) {
    if (dep[top[u]] < dep[top[v]]) swap(u, v);
    res = max(res, pt_query(chroot[top[u]], dfn[top[u]], dfn[u]));
    u = fa[top[u]];
  }
  if (dep[u] > dep[v]) swap(u, v);
  res = max(res, pt_query(chroot[top[u]], dfn[u], dfn[v]));
  return res;
}

int subtree_query(int x) {
  int h = top[x];
  return ck_query(chroot[h], dfn[x], dfn[tail[h]]);
}

void point_set(int x, int v) {
  val[x] = v;
  while (true) {
    for (int u = x; u; u = f[u]) pull(u);
    int h = top[x], p = fa[h];
    if (!p) break;
    int nr = -INF;
    for (int y : es[p])
      if (y != fa[p] && y != son[p]) nr = max(nr, ck[chroot[y]]);
    if (nr == rake[p]) break;
    rake[p] = nr;
    x = p;
  }
}

/*
 * ============================================================
 * 名称：静态 Top Tree（重链剖分 + 链内 Splay 的簇分解）
 * 复杂度：预处理 O(n log n)；path_query O(log^2 n)；subtree_query O(log n)；
 *         point_set O(log^2 n + Σ轻边度数)（簇信息沿虚边传播）
 * 用途：静态树上的簇（cluster）聚合查询：
 *       1) path_query(u, v) 路径点权最大值（不含轻链 rake，语义与树剖一致）；
 *       2) subtree_query(x) 整棵子树的最大值——利用"簇"语义：x 的重链段
 *          [dfn[x], dfn[tail[top[x]]]] 上每个节点聚合其所有轻儿子链的簇
 *          （rake），恰好覆盖 x 的整棵子树（O(log n) 而非 O(子树大小)）；
 *       3) point_set 修改点权并沿 splay 与虚边（轻链根 → 父链节点）传播簇
 * 原理：每条重链一棵 Splay（中序 = 链序，子树覆盖连续 dfn 区间）；
 *       每个节点维护：smx（链内区间点权最值）、rake（轻儿子链根簇的聚合）、
 *       ck（簇聚合 = 点 ∪ splay 子树 ∪ rake）；建树按链头深度从深到浅，
 *       保证 rake 依赖的轻链簇已就绪；splay 结构保留旋转能力便于扩展
 * 注意：点权版本；森林需对每个根 dfs1/dfs2；tail[h] 为链 h 的末端节点；
 *       rake 依赖 ck，ck 依赖 rake —— 修改传播见 point_set
 * ============================================================
 * 使用示例（编译时取消注释；路径 max / 子树 max / 单点改）：
 * signed main() {
 *   cin >> n;
 *   for (int i = 1; i <= n; ++i) cin >> val[i];
 *   for (int i = 1, u, v; i < n; ++i) {
 *     cin >> u >> v;
 *     es[u].push_back(v), es[v].push_back(u);
 *   }
 *   toptree_build();
 *   int q;
 *   cin >> q;
 *   while (q--) {
 *     int o, u, v;
 *     cin >> o >> u >> v;
 *     if (o == 1) point_set(u, v);
 *     if (o == 2) cout << path_query(u, v) << '\n';
 *     if (o == 3) cout << subtree_query(u) << '\n';
 *   }
 * }
 * ============================================================
 */
