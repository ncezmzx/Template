#define int long long

template <size_t N> struct hld {
  int n, idx;
  int a[N], dfn[N], sz[N], id[N], top[N], fa[N], son[N], dep[N];
  vector<int> es[N];
  int sgt[N << 2];
  int dfs1(int x, int ff) {
    fa[x] = ff, dep[x] = dep[ff] + 1;
    for (int y : es[x])
      if (y ^ ff) sz[x] += dfs1(y, x), sz[y] > sz[son[x]] && (son[x] = y);
    return ++sz[x];
  }
  void dfs2(int x, int tp) {
    top[x] = tp, id[dfn[x] = ++idx] = x;
    if (son[x]) dfs2(son[x], tp);
    for (int y : es[x])
      if ((y ^ son[x]) && (y ^ fa[x])) dfs2(y, y);
  }
  void up(int u) { sgt[u] = sgt[u << 1] + sgt[u << 1 | 1]; }
  void build(int u, int l, int r) {
    if (l == r) return sgt[u] = a[id[l]], void();
    int m = (l + r) >> 1;
    build(u << 1, l, m), build(u << 1 | 1, m + 1, r);
    up(u);
  }
  void update(int u, int l, int r, int x, int y) {
    if (l == r) return sgt[u] = y, void();
    int m = (l + r) >> 1;
    if (m >= x) update(u << 1, l, m, x, y);
    else update(u << 1 | 1, m + 1, r, x, y);
    up(u);
  }
  int query(int u, int l, int r, int x, int y) {
    if (x <= l && r <= y) return sgt[u];
    int m = (l + r) >> 1, res = 0;
    if (m >= x) res += query(u << 1, l, m, x, y);
    if (m < y) res += query(u << 1 | 1, m + 1, r, x, y);
    return res;
  }
  int query(int x, int y) {
    int res = 0;
    while (top[x] != top[y]) {
      if (dep[top[x]] < dep[top[y]]) swap(x, y);
      res += query(1, 1, n, dfn[top[x]], dfn[x]);
      x = fa[top[x]];
    }
    if (dfn[x] < dfn[y]) swap(x, y);
    return query(1, 1, n, dfn[y], dfn[x]) + res;
  }
};

