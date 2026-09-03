#define int long long

template <size_t N, size_t M> struct bounded_flow {
  static constexpr int INF = 0x3f3f3f3f3f3f3f3f;
  int hd[N], nxt[M], to[M], cap[M], tot = 1, cur[N], dep[N], q[N], nv = 2;
  int in[N];
  void init() {
    tot = 1, nv = 2;
    memset(hd, 0, sizeof hd), memset(in, 0, sizeof in);
  }
  void add(int x, int y, int c) {
    nxt[++tot] = hd[x], hd[x] = tot, to[tot] = y, cap[tot] = c;
    nxt[++tot] = hd[y], hd[y] = tot, to[tot] = x, cap[tot] = 0;
    if (x >= nv) nv = x + 1;
    if (y >= nv) nv = y + 1;
  }
  bool bfs(int s, int t) {
    memset(dep, -1, nv * sizeof(int));
    int qh = 0, qt = 0;
    q[qt++] = s, dep[s] = 0;
    while (qh < qt) {
      int x = q[qh++], dx = dep[x] + 1;
      for (int i = hd[x]; i; i = nxt[i])
        if (cap[i] && dep[to[i]] == -1) dep[to[i]] = dx, q[qt++] = to[i];
    }
    return dep[t] != -1;
  }
  int dfs(int x, int t, int w) {
    if (x == t) return w;
    int flow = 0, dx = dep[x] + 1;
    for (int &i = cur[x]; i && w; i = nxt[i]) {
      int v = to[i];
      if (cap[i] && dep[v] == dx) {
        int k = dfs(v, t, min(w, cap[i]));
        flow += k, w -= k, cap[i] -= k, cap[i ^ 1] += k;
      }
    }
    return flow;
  }
  int maxflow(int s, int t) {
    int r = 0;
    while (bfs(s, t)) {
      memcpy(cur, hd, nv * sizeof(int));
      r += dfs(s, t, INF);
    }
    return r;
  }
};

