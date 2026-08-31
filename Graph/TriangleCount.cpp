#include <bits/stdc++.h>
using namespace std;
#define int long long

// triangle counting in an undirected graph via degree orientation, O(m sqrt m)
template <size_t N, size_t M> struct triangle_count {
  int n, m, deg[N];
  pair<int, int> e[M];
  vector<int> g[N];
  // fill e[1..m] and deg[] first; no multi-edges
  long long count() {
    for (int i = 1; i <= m; ++i) {
      int u = e[i].first, v = e[i].second;
      if (deg[u] > deg[v] || (deg[u] == deg[v] && u > v)) swap(u, v);
      g[u].push_back(v); // orient low degree -> high degree (DAG)
    }
    vector<char> tag(n + 1, 0);
    long long ans = 0;
    for (int u = 1; u <= n; ++u) {
      for (int v : g[u]) tag[v] = 1;
      for (int v : g[u])
        for (int w : g[v])
          if (tag[w]) ++ans; // (u, v, w) counted at its smallest-degree vertex
      for (int v : g[u]) tag[v] = 0;
    }
    return ans;
  }
};

/*
 * ============================================================
 * Name: triangle counting (undirected graph, O(m sqrt m))
 * Complexity: O(m sqrt m) (after reorientation every vertex has out-degree
 *             O(sqrt m))
 * Usage: count length-3 cycles of an undirected graph, `triangle_count<N, M>`:
 *        fill e[1..m] and deg[], then count().
 * Principle: orient each edge from lower degree to higher (ties by id),
 *            obtaining a DAG; every triangle is counted exactly once at its
 *            smallest-degree vertex: mark u's out-neighbors, then for each out-
 *            neighbor v scan v's out-neighbors and count the marked ones
 * Notes: merge multi-edges first (none are assumed here); the tag clearing
 *        costs O(out-degree) per vertex, O(m) total
 * ============================================================
 */
