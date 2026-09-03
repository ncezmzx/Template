
constexpr int N = 509;
struct Blossom {
  int n, par[N], match_[N], pre[N], col[N], q_[N], hd, tl;
  vector<int> g[N];
  void init(int n_) {
    n = n_;
    for (int i = 0; i <= n; ++i) g[i].clear();
    fill(match_ + 1, match_ + n + 1, 0);
  }
  void add(int u, int v) { g[u].push_back(v), g[v].push_back(u); }
  int find_(int x) { return par[x] == x ? x : par[x] = find_(par[x]); }
  int lca(int x, int y) {
    static int tim = 0, vis[N] = {0};
    ++tim;
    for (;; swap(x, y))
      if (x) {
        x = find_(x);
        if (vis[x] == tim) return x;
        vis[x] = tim;
        x = pre[match_[x]];
      }
  }
  void blossom(int x, int y, int l) {
    while (find_(x) != l) {
      pre[x] = y;
      int w = match_[x];
      if (col[w] == 2) col[w] = 1, q_[tl++] = w;
      if (find_(x) == x) par[x] = l;
      if (find_(w) == w) par[w] = l;
      y = w;
      x = pre[w];
    }
  }
  bool augment(int s) {
    for (int i = 1; i <= n; ++i) par[i] = i, col[i] = 0;
    hd = tl = 0, q_[tl++] = s, col[s] = 1;
    while (hd < tl) {
      int u = q_[hd++];
      for (int v : g[u]) {
        if (find_(u) == find_(v) || col[v] == 2) continue;
        if (col[v] == 1) {
          int l = lca(u, v);
          blossom(u, v, l), blossom(v, u, l);
        }
        else if (!match_[v]) {
          pre[v] = u;
          for (int x = v; x;) {
            int y = pre[x], z = match_[y];
            match_[x] = y, match_[y] = x;
            x = z;
          }
          return true;
        }
        else if (!col[v]) {
          pre[v] = u, col[v] = 2;
          if (!col[match_[v]]) col[match_[v]] = 1, q_[tl++] = match_[v];
        }
      }
    }
    return false;
  }
  int solve() {
    int res = 0;
    for (int i = 1; i <= n; ++i)
      if (!match_[i] && augment(i)) ++res;
    return res;
  }
};

