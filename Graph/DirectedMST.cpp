#define int long long

constexpr int NV = 109;
struct edge {
  int u, v, w;
};

long long zhu_liu(int n, int m, int root, vector<edge> &e) {
  static int id_[NV], vis_[NV], pre[NV], ine[NV];
  long long res = 0;
  for (;;) {
    fill(ine, ine + n, LLONG_MAX);
    for (int i = 0; i < m; ++i)
      if (e[i].u != e[i].v && e[i].w < ine[e[i].v]) ine[e[i].v] = e[i].w, pre[e[i].v] = e[i].u;
    for (int i = 0; i < n; ++i)
      if (i != root && ine[i] == LLONG_MAX) return -1;
    int cnt_ = 0;
    fill(id_, id_ + n, -1), fill(vis_, vis_ + n, -1);
    for (int i = 0; i < n; ++i) {
      if (i == root) continue;
      res += ine[i];
      int v = i;
      while (vis_[v] != i && id_[v] == -1 && v != root) vis_[v] = i, v = pre[v];
      if (v != root && id_[v] == -1) {
        int u = v;
        do {
          id_[u] = cnt_;
          u = pre[u];
        } while (u != v);
        ++cnt_;
      }
    }
    if (!cnt_) return res;
    for (int i = 0; i < n; ++i)
      if (id_[i] == -1) id_[i] = cnt_++;
    for (int i = 0; i < m; ++i) {
      int v = e[i].v;
      e[i].u = id_[e[i].u], e[i].v = id_[e[i].v];
      if (e[i].u != e[i].v) e[i].w -= ine[v];
    }
    n = cnt_, root = id_[root];
  }
}

