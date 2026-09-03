#include <bits/stdc++.h>
using namespace std;


template <size_t N> struct wdsu {
  int p[N];
  long long d[N];
  void init(int n) {
    for (int i = 0; i <= n; ++i) p[i] = i, d[i] = 0;
  }
  int find(int x) {
    if (p[x] == x) return x;
    int r = find(p[x]);
    d[x] += d[p[x]], p[x] = r;
    return r;
  }


  bool merge(int x, int y, long long w) {
    int rx = find(x), ry = find(y);
    if (rx == ry) return d[y] - d[x] == w;
    p[rx] = ry, d[rx] = d[y] - d[x] - w;
    return true;
  }
  long long rel(int x, int y) { return d[y] - d[x]; }
};

template <size_t N> struct rdsu {
  int p[N], rk[N];
  vector<pair<int, int>> hist;
  void init(int n) {
    for (int i = 0; i <= n; ++i) p[i] = i, rk[i] = 0;
    hist.clear();
  }
  int find(int x) {
    while (p[x] != x) x = p[x];
    return x;
  }
  bool merge(int a, int b) {
    int ra = find(a), rb = find(b);
    if (ra == rb) return hist.push_back({-1, 0}), false;
    if (rk[ra] < rk[rb]) swap(ra, rb);
    p[rb] = ra, hist.push_back({rb, rk[ra] == rk[rb]});
    rk[ra] += (rk[ra] == rk[rb]);
    return true;
  }
  void rollback(size_t target) {
    while (hist.size() > target) {
      auto pr = hist.back();
      hist.pop_back();
      if (pr.first < 0) continue;
      if (pr.second) rk[p[pr.first]]--;
      p[pr.first] = pr.first;
    }
  }
};

