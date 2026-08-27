#include <bits/stdc++.h>
using namespace std;

/*
 * 名称：树链剖分（HLD）+ 线段树维护路径和
 * 复杂度：预处理 O(n)，单次路径查询/单点修改 O(log^2 n)
 * 用途：静态树上的路径和、子树和、单点修改等问题
 * 说明：线段树按题意简化为"单点改 / 区间和"；原题 11088-11110 是区间取模线段树，
 *       按题目需要可自行替换。HLD 部分（dfs1/dfs2 与路径查询 query(x,y)）原样保留。
 * 使用示例：
 *   dfs1(1, 0), dfs2(1, 1), build(1, 1, n);
 *   query(x, y);                     // x->y 路径和
 *   update(1, 1, n, dfn[x], y);      // 点 x 权值改为 y
 * 来源：all.cpp 行 11059-11121（HLD：11065-11076、路径查询：11111-11121 原样保留）
 */
#define int long long
constexpr int N = 1e5 + 9;
int n, m, a[N], dfn[N], sz[N], idx, id[N], top[N], fa[N], son[N], dep[N];
vector<int> es[N];
int dfs1(int x, int ff) {            // 求 fa/dep/子树大小 sz/重儿子 son
  fa[x] = ff, dep[x] = dep[ff] + 1;
  for (int y : es[x])
    if (y ^ ff) sz[x] += dfs1(y, x), sz[y] > sz[son[x]] && (son[x] = y);
  return ++sz[x];
}
void dfs2(int x, int tp) {           // 剖分：top 记录链顶，dfn 序（先重儿子）
  top[x] = tp, id[dfn[x] = ++idx] = x;
  if (son[x]) dfs2(son[x], tp);
  for (int y : es[x])
    if ((y ^ son[x]) && (y ^ fa[x])) dfs2(y, y);
}
int sgt[N << 2];
void up(int u) {
  sgt[u] = sgt[u << 1] + sgt[u << 1 | 1];
}
void build(int u, int l, int r) {
  if (l == r) return sgt[u] = a[id[l]], void();   // id[dfn] 还原点权
  int m = (l + r) >> 1;
  build(u << 1, l, m), build(u << 1 | 1, m + 1, r);
  up(u);
}
void update(int u, int l, int r, int x, int y) {  // 单点修改：dfn 为 x 的点改为 y
  if (l == r) return sgt[u] = y, void();
  int m = (l + r) >> 1;
  if (m >= x) update(u << 1, l, m, x, y);
  else update(u << 1 | 1, m + 1, r, x, y);
  up(u);
}
int query(int u, int l, int r, int x, int y) {    // 区间和（dfn 区间）
  if (x <= l && r <= y) return sgt[u];
  int m = (l + r) >> 1, res = 0;
  if (m >= x) res += query(u << 1, l, m, x, y);
  if (m < y) res += query(u << 1 | 1, m + 1, r, x, y);
  return res;
}
int query(int x, int y) {            // 路径 x->y 的和（HLD，原样保留）
  int res = 0;
  while (top[x] != top[y]) {
    if (dep[top[x]] < dep[top[y]]) swap(x, y);
    res += query(1, 1, n, dfn[top[x]], dfn[x]);
    x = fa[top[x]];
  }
  if (dfn[x] < dfn[y]) swap(x, y);
  return query(1, 1, n, dfn[y], dfn[x]) + res;
}

#ifdef DEMO
signed main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  cin >> n >> m;
  for (int i = 1; i <= n; ++i) cin >> a[i];
  for (int i = 1, x, y; i < n; ++i) {
    cin >> x >> y;
    es[x].push_back(y), es[y].push_back(x);
  }
  dfs1(1, 0), dfs2(1, 1), build(1, 1, n);
  while (m--) {
    int o, x, y;
    cin >> o >> x >> y;
    if (o == 1) update(1, 1, n, dfn[x], y);   // 单点修改
    else cout << query(x, y) << '\n';          // 路径和
  }
  cout.flush();   // 演示用：确保缓冲输出落盘（OJ 正常退出会自动 flush）
}
#endif
