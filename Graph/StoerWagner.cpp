#define int long long

template <size_t N> struct stoer_wagner {
  int n_;
  int edge[N][N];
  int dist[N], vis[N], bin[N];
  void init(int n) {
    n_ = n;
    for (int i = 1; i <= n; ++i) {
      bin[i] = 0;
      for (int j = 1; j <= n; ++j) edge[i][j] = 0;
    }
  }
  void add(int u, int v, int w) { edge[u][v] += w, edge[v][u] += w; }

  int contract_(int &s, int &t) {
    memset(dist, 0, sizeof dist), memset(vis, 0, sizeof vis);
    int mincut = 0;
    s = t = -1;
    for (int i = 0; i < n_; ++i) {
      int k = -1, mx = -1;
      for (int j = 1; j <= n_; ++j)
        if (!bin[j] && !vis[j] && dist[j] > mx) k = j, mx = dist[j];
      if (k == -1) return mincut;
      s = t, t = k, mincut = mx, vis[k] = 1;
      for (int j = 1; j <= n_; ++j)
        if (!bin[j] && !vis[j]) dist[j] += edge[k][j];
    }
    return mincut;
  }
  int solve() {
    int ans = LLONG_MAX, s = 0, t = 0;
    for (int i = 1; i < n_; ++i) {
      int cur = contract_(s, t);
      bin[t] = 1;
      ans = min(ans, cur);
      if (ans == 0) return 0;
      for (int j = 1; j <= n_; ++j)
        if (!bin[j]) edge[s][j] = (edge[j][s] += edge[j][t]);
    }
    return ans;
  }
};

