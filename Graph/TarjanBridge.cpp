#include <bits/stdc++.h>
using namespace std;
#define int long long

// Tarjan bridges / edge-biconnected components (handles multi-edges via edge ids)
template <size_t N> struct edcc {
  int n, idx, tp, cl, etot;
  int dfn[N], stk[N], low[N], vis[N], col[N];
  vector<pair<int, int>> es[N]; // (neighbor, edge id)
  void add(int x, int y) {
    es[x].emplace_back(y, ++etot);
    es[y].emplace_back(x, etot);
  }
  void tarjan(int x, int lst) { // lst = edge id to parent
    dfn[x] = low[x] = ++idx, vis[x] = 1, stk[++tp] = x;
    for (auto [y, z] : es[x]) {
      if (lst == z) continue;
      if (!dfn[y]) tarjan(y, z), low[x] = min(low[x], low[y]);
      else if (vis[y]) low[x] = min(low[x], dfn[y]);
    }
    if (dfn[x] == low[x]) { // x is the top of one e-DCC
      ++cl;
      while (stk[tp] != x) col[stk[tp--]] = cl;
      col[stk[tp--]] = cl;
    }
  }
  // cl = number of edge-biconnected components, col[x] = component id
  void build(int n_) {
    n = n_, idx = tp = cl = etot = 0;
    for (int i = 1; i <= n; ++i) dfn[i] = vis[i] = 0;
    for (int i = 1; i <= n; ++i)
      if (!dfn[i]) tarjan(i, 0);
  }
};

/*
 * ============================================================
 * Name: Tarjan bridges / edge-biconnected components (e-DCC)
 * Complexity: O(n + m)
 * Usage: components left after removing the bridges, `edcc<N>`: add(x, y)
 *        inserts edges, build(n), then cl = number of edge-biconnected
 *        components and col[x] = component id.
 *        Bridge criterion: the edge (x, parent) is a bridge iff dfn[x] ==
 *        low[x] once x is finished; the parent-edge id (lst) handles multi-
 *        edges correctly.
 * ============================================================
 */
