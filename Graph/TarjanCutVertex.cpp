#include <bits/stdc++.h>
using namespace std;
#define int long long

// Tarjan cut vertices (articulation points) in an undirected graph
template <size_t N>
struct cut_vertex {
  int n, idx, rt;
  int dfn[N], low[N], is_cut[N];
  vector<int> es[N];
  void tarjan(int x) {
    low[x] = dfn[x] = ++idx;
    int sz = 0;
    for (int y : es[x]) {
      if (!dfn[y]) {
        ++sz;
        tarjan(y), low[x] = min(low[x], low[y]);
        if (low[y] >= dfn[x] && x != rt) is_cut[x] = 1;
      }
      else low[x] = min(low[x], dfn[y]);
    }
    if (x == rt && sz > 1) is_cut[x] = 1;  // root needs >= 2 child subtrees
  }
  void build(int n_) {
    n = n_, idx = 0;
    for (int i = 1; i <= n; ++i) dfn[i] = is_cut[i] = 0;
    for (int i = 1; i <= n; ++i)
      if (!dfn[i]) rt = i, tarjan(i);
  }
};

/*
 * ============================================================
 * Name: Tarjan cut vertices (articulation points, undirected graph)
 * Complexity: O(n + m)
 * Usage: cut vertices of an undirected graph, `cut_vertex<N>`: es holds the
 *        undirected edges, build(n), then is_cut[x] = 1 marks a cut vertex.
 *        Criterion: a non-root x with a child y satisfying low[y] >= dfn[x];
 *        the root needs >= 2 child subtrees (sz > 1).
 * Source: all.cpp lines 56109-56127 (wrapped into a struct, recursion unchanged)
 * ============================================================
 */
