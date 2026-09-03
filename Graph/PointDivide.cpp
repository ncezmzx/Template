#define int long long

template <size_t N> struct centroid_tree {
  int n, idx, rt, cnt, lca_built = -1;
  int a[N], mi[20][N], dfn[N], fa[N], ban[N], sz[N], dep[N];
  vector<int> es[N], c[2][N];
  void bit_add(int u, int x, int y, int o) {
    for (++x; x < (int)c[o][u].size(); x += x & -x) c[o][u][x] += y;
  }
  int bit_sum(int u, int x, int o, int r = 0) {
    for (++x, x = min(x, (int)c[o][u].size() - 1); x > 0; x -= x & -x) r += c[o][u][x];
    return r;
  }
  void dfs(int x, int ff) {
    mi[0][dfn[x] = ++idx] = ff, dep[x] = dep[ff] + 1;
    for (int y : es[x])
      if (y ^ ff) dfs(y, x);
  }
  int get(int x, int y) { return dfn[x] < dfn[y] ? x : y; }
  void build_mi() {
    for (int k = 1; k < 20; ++k)
      for (int i = 1; i + (1 << k) - 1 <= idx; ++i) mi[k][i] = get(mi[k - 1][i], mi[k - 1][i + (1 << (k - 1))]);
    lca_built = idx;
  }
  int lca(int x, int y) {
    if (x == y) return x;
    if (lca_built != idx) build_mi();
    if ((x = dfn[x]) > (y = dfn[y])) swap(x, y);
    int d = __lg(y - ++x + 1);
    return get(mi[d][x], mi[d][y - (1 << d) + 1]);
  }
  int dist(int x, int y) { return dep[x] + dep[y] - 2 * dep[lca(x, y)]; }
  void dfs3(int x, int ff) {
    sz[x] = 1;
    for (int y : es[x])
      if (y ^ ff && !ban[y]) dfs3(y, x), sz[x] += sz[y];
  }
  void dfs2(int x, int ff) {
    int mx = cnt - sz[x];
    for (int y : es[x])
      if (y ^ ff && !ban[y]) dfs2(y, x), mx = max(mx, sz[y]);
    if (mx * 2 <= cnt) rt = x;
  }
  void build(int x, int ff) {
    dfs3(x, 0), cnt = sz[x], dfs2(x, 0);
    ban[rt] = 1, fa[rt] = ff, c[0][rt].resize(cnt + 2), c[1][rt].resize(cnt + 2);
    int u = rt;
    for (int y : es[u])
      if (y ^ ff && !ban[y]) build(y, u);
  }
  int query(int x, int y) {
    int res = 0;
    for (int pr = 0, i = x; i; pr = i, i = fa[i]) {
      int d = dist(i, x);
      res += bit_sum(i, y - d, 0);
      if (pr) res -= bit_sum(pr, y - d, 1);
    }
    return res;
  }
  void update(int x, int y) {
    for (int i = x; i; i = fa[i]) bit_add(i, dist(x, i), y, 0);
    for (int i = x; fa[i]; i = fa[i]) bit_add(i, dist(x, fa[i]), y, 1);
  }
};

