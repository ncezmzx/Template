#define int long long

template <size_t N> struct dijkstra {
  static constexpr int INF = 0x3f3f3f3f3f3f3f3f;
  int dst[N];
  vector<pair<int, int>> es[N];
  void run(int s) {
    memset(dst, 0x3f, sizeof dst);
    priority_queue<pair<int, int>> q;
    q.push({dst[s] = 0, s});
    while (!q.empty()) {
      auto [d, x] = q.top();
      d = -d, q.pop();
      if (d != dst[x]) continue;
      for (auto [y, z] : es[x])
        if (dst[y] > dst[x] + z) {
          dst[y] = dst[x] + z;
          q.emplace(-dst[y], y);
        }
    }
  }
};

