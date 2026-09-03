#include <bits/stdc++.h>
using namespace std;
#define int long long


template <size_t N, size_t M> struct triangle_count {
  int n, m, deg[N];
  pair<int, int> e[M];
  vector<int> g[N];

  long long count() {
    for (int i = 1; i <= m; ++i) {
      int u = e[i].first, v = e[i].second;
      if (deg[u] > deg[v] || (deg[u] == deg[v] && u > v)) swap(u, v);
      g[u].push_back(v);
    }
    vector<char> tag(n + 1, 0);
    long long ans = 0;
    for (int u = 1; u <= n; ++u) {
      for (int v : g[u]) tag[v] = 1;
      for (int v : g[u])
        for (int w : g[v])
          if (tag[w]) ++ans;
      for (int v : g[u]) tag[v] = 0;
    }
    return ans;
  }
};

