#define int long long

template <size_t N, size_t M> struct spfa {
  int n_, ecnt, hd[N], nxt[M], to[M], cnt_[N];
  long long wt[M], dis[N];
  bool inq[N];
  void init(int n) {
    n_ = n, ecnt = 1;
    fill(hd, hd + n + 1, 0);
  }
  void add(int u, int v, long long w) {
    ++ecnt, to[ecnt] = v, wt[ecnt] = w, nxt[ecnt] = hd[u], hd[u] = ecnt;
  }

  bool run(int s) {
    fill(dis + 1, dis + n_ + 1, LLONG_MAX);
    fill(cnt_ + 1, cnt_ + n_ + 1, 0);
    fill(inq + 1, inq + n_ + 1, false);
    deque<int> q;
    dis[s] = 0, inq[s] = true, q.push_back(s);
    while (!q.empty()) {
      int u = q.front();
      q.pop_front(), inq[u] = false;
      for (int e = hd[u]; e; e = nxt[e]) {
        int v = to[e];
        if (dis[u] + wt[e] < dis[v]) {
          dis[v] = dis[u] + wt[e];
          if (!inq[v]) {
            if (++cnt_[v] >= n_) return false;
            inq[v] = true;
            if (!q.empty() && dis[v] < dis[q.front()]) q.push_front(v);
            else q.push_back(v);
          }
        }
      }
    }
    return true;
  }

  bool diff_constraints(const vector<array<long long, 3>> &cs, int n, vector<long long> &x) {
    init(n + 1);
    for (const auto &t : cs) add(t[1], t[0], t[2]);
    for (int i = 1; i <= n; ++i) add(0, i, 0);
    if (!run(0)) return false;
    x.assign(n + 1, 0);
    for (int i = 1; i <= n; ++i) x[i] = dis[i];
    return true;
  }
};

