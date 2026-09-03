#include <bits/stdc++.h>
using namespace std;
#define int long long


template <size_t N>
struct hopcroft_karp {
  int nl, nr, vstamp = 0;
  int dis[N], match[N], vis[N], q[N];
  vector<int> g[N];
  bool bfs() {
    bool f = false;
    int qh = 0, qt = 0;
    for (int i = 1; i <= nl; ++i)
      if (!match[i]) dis[i] = 0, q[qt++] = i;
      else dis[i] = -1;
    dis[0] = -1;
    while (qh < qt) {
      int u = q[qh++], du = dis[u] + 1;
      for (int v : g[u]) {
        if (dis[match[v]] == -1) {
          dis[match[v]] = du;
          if (match[v]) q[qt++] = match[v];
          else f = true;
        }
      }
    }
    return f;
  }
  bool dfs(int u) {
    int du = dis[u] + 1;
    for (int v : g[u]) {
      if (match[v] && dis[match[v]] != du) continue;
      if (vis[v] == vstamp) continue;
      vis[v] = vstamp;
      if (!match[v] || dfs(match[v])) {
        match[u] = v, match[v] = u;
        return true;
      }
    }
    dis[u] = -1;
    return false;
  }
  int solve(int nl_, int nr_) {
    nl = nl_, nr = nr_;
    for (int i = 1; i <= nl + nr; ++i) match[i] = 0;
    int ans = 0;
    while (bfs()) {
      ++vstamp;
      for (int i = 1; i <= nl; ++i)
        if (!match[i] && dfs(i)) ++ans;
    }
    return ans;
  }
};

