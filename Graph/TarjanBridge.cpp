#include <bits/stdc++.h>
using namespace std;
#define int long long

// Tarjan bridges / edge-biconnected components (handles multi-edges via edge ids)
template <size_t N>
struct edcc {
  int n, idx, tp, cl, etot;
  int dfn[N], stk[N], low[N], vis[N], col[N];
  vector<pair<int, int>> es[N];  // (neighbor, edge id)
  void add(int x, int y) {
    es[x].emplace_back(y, ++etot);
    es[y].emplace_back(x, etot);
  }
  void tarjan(int x, int lst) {  // lst = edge id to parent
    dfn[x] = low[x] = ++idx, vis[x] = 1, stk[++tp] = x;
    for (auto [y, z] : es[x]) {
      if (lst == z) continue;
      if (!dfn[y]) tarjan(y, z), low[x] = min(low[x], low[y]);
      else if (vis[y]) low[x] = min(low[x], dfn[y]);
    }
    if (dfn[x] == low[x]) {  // x is the top of one e-DCC
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
 * Usage: contract the components left after removing bridges, wrapped as
 *        edcc<N>: add(x, y) inserts edges, build(n), then cl = number of
 *        edge-biconnected components and col[x] = component id; bridge
 *        criterion: edge (x, parent) is a bridge iff dfn[x] == low[x] after
 *        x is finished; the parent-edge id (lst) handles multi-edges
 *        correctly (parallel edges get distinct ids, so none is skipped)
 * Source: all.cpp lines 51899-51912 (wrapped into a struct, recursion unchanged)
 * ============================================================
 * Example (uncomment to compile):
 * static edcc<300009> ec;
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   int n, m;
 *   cin >> n >> m;
 *   for (int i = 1, x, y; i <= m; ++i) {
 *     cin >> x >> y;
 *     ec.add(x, y);
 *   }
 *   ec.build(n);
 *   cout << ec.cl << '\n';                    // number of e-DCCs
 *   for (int i = 1; i <= n; ++i) cout << ec.col[i] << " \n"[i == n];
 * }
 * ============================================================
 */
