#include <bits/stdc++.h>
using namespace std;
#define int long long

// Tarjan strongly connected components + condensation info
template <size_t N> struct scc {
  int n, cl, idx, tp;
  int dfn[N], stk[N], low[N], col[N], vst[N];
  vector<int> es[N];
  void tarjan(int x) {
    low[x] = dfn[x] = ++idx, stk[++tp] = x, vst[x] = true;
    for (int y : es[x]) {
      if (!dfn[y]) tarjan(y), low[x] = min(low[x], low[y]);
      else if (vst[y]) low[x] = min(low[x], dfn[y]);
    }
    if (low[x] == dfn[x]) {
      ++cl;
      for (int u = 0; u != x; vst[u] = false) col[u = stk[tp--]] = cl;
    }
  }
  // runs Tarjan from every unvisited vertex; cl = component count, col[x] = id
  void build(int n_) {
    n = n_, cl = idx = tp = 0;
    for (int i = 1; i <= n; ++i) dfn[i] = vst[i] = 0;
    for (int i = 1; i <= n; ++i)
      if (!dfn[i]) tarjan(i);
  }
};

/*
 * ============================================================
 * Name: Tarjan strongly connected components (SCC) + condensation
 * Complexity: O(n + m)
 * Usage: SCCs of a directed graph, `scc<N>`: es holds the edges, build(n), then
 *        cl = number of SCCs and col[x] = component id of x.
 *        To condense into a DAG, add col[x] -> col[y] for every original edge
 *        (x, y) with col[x] != col[y].
 * Source: all.cpp lines 14111-14129 (wrapped into a struct, recursion unchanged)
 * ============================================================
 */
