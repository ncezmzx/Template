#define int long long

template <size_t N> struct edcc {
  int n, idx, tp, cl, etot;
  int dfn[N], stk[N], low[N], vis[N], col[N];
  vector<pair<int, int>> es[N];
  void add(int x, int y) {
    es[x].emplace_back(y, ++etot);
    es[y].emplace_back(x, etot);
  }
  void tarjan(int x, int lst) {
    dfn[x] = low[x] = ++idx, vis[x] = 1, stk[++tp] = x;
    for (auto [y, z] : es[x]) {
      if (lst == z) continue;
      if (!dfn[y]) tarjan(y, z), low[x] = min(low[x], low[y]);
      else if (vis[y]) low[x] = min(low[x], dfn[y]);
    }
    if (dfn[x] == low[x]) {
      ++cl;
      while (stk[tp] != x) col[stk[tp--]] = cl;
      col[stk[tp--]] = cl;
    }
  }

  void build(int n_) {
    n = n_, idx = tp = cl = etot = 0;
    for (int i = 1; i <= n; ++i) dfn[i] = vis[i] = 0;
    for (int i = 1; i <= n; ++i)
      if (!dfn[i]) tarjan(i, 0);
  }
};

