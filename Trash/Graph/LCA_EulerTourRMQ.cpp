#define int long long

template <size_t N> struct lca_euler_rmq {
  int n, idx;
  int mi[20][N], dfn[N], dep[N];
  vector<int> es[N];
  void dfs(int x, int ff) {
    mi[0][dfn[x] = ++idx] = ff, dep[x] = dep[ff] + 1;
    for (int y : es[x])
      if (y ^ ff) dfs(y, x);
  }
  int get(int x, int y) { return dfn[x] < dfn[y] ? x : y; }
  void build(int n_) {
    n = n_;
    for (int i = 1; i < 20; ++i)
      for (int j = 1; j + (1 << i) - 1 <= n; ++j) mi[i][j] = get(mi[i - 1][j], mi[i - 1][j + (1 << (i - 1))]);
  }
  int lca(int x, int y) {
    if (x == y) return x;
    if ((x = dfn[x]) > (y = dfn[y])) swap(x, y);
    int d = __lg(y - ++x + 1);
    return get(mi[d][x], mi[d][y - (1 << d) + 1]);
  }
  int dist(int x, int y) { return dep[x] + dep[y] - 2 * dep[lca(x, y)]; }
};

