#include <bits/stdc++.h>
using namespace std;

// Vertex-biconnected components / block-cut tree, with LCA binary lifting
template <size_t N> struct block_cut_tree {
  int n, sq, idx, tp;
  int dfn[N], low[N], stk[N];
  int dep[N << 1], fa[N << 1][20];
  vector<int> g[N], e[N << 1]; // g = original graph, e = block-cut tree
  void tarjan(int u) {
    dfn[u] = low[u] = ++idx;
    stk[++tp] = u;
    for (int v : g[u]) {
      if (!dfn[v]) {
        tarjan(v);
        low[u] = min(low[u], low[v]);
        if (low[v] >= dfn[u]) { // u is a cut vertex (or root): pop one BCC
          e[u].push_back(++sq);
          e[sq].push_back(u);
          for (int x = 0; x != v;) {
            e[x = stk[tp--]].push_back(sq);
            e[sq].push_back(x);
          }
        }
      }
      else low[u] = min(low[u], dfn[v]);
    }
  }
  // square nodes are numbered n+1..; call once per connected component
  void build(int n_) {
    n = n_, sq = n, idx = tp = 0;
    for (int i = 1; i <= n; ++i) dfn[i] = 0;
    for (int i = 1; i <= n; ++i)
      if (!dfn[i]) tarjan(i);
  }
  void dfs(int u, int f) { // root the tree + build lifting table
    dep[u] = dep[f] + 1;
    fa[u][0] = f;
    for (int i = 1; fa[u][i - 1]; i++) fa[u][i] = fa[fa[u][i - 1]][i - 1]; // stop at 0
    for (int v : e[u]) {
      if (v == f) continue;
      dfs(v, u);
    }
  }
  int lca(int x, int y) {
    if (dep[x] < dep[y]) swap(x, y);
    for (int i = 19; i >= 0; i--) {
      if (dep[fa[x][i]] >= dep[y]) {
        x = fa[x][i];
      }
    }
    if (x == y) return x;
    for (int i = 19; i >= 0; i--) {
      if (fa[x][i] != fa[y][i]) {
        x = fa[x][i], y = fa[y][i];
      }
    }
    return fa[x][0];
  }
};

/*
 * ============================================================
 * Name: Tarjan vertex-biconnected components (v-BCC) / block-cut tree
 * Complexity: O(n + m)
 * Usage: vertex-biconnected components + block-cut tree, `block_cut_tree<N>`: g
 *        holds the original graph; build(n) contracts the BCCs
 *        (square nodes numbered from n+1 up, total sq, adjacency e sized 2N);
 *        then dfs(root, 0) per component roots the tree and builds the lifting
 *        table for lca(x, y).
 *        Cut vertices are the round nodes with degree >= 2 in the block-cut
 *        tree.
 * Source: all.cpp lines 55913-55931 (tarjan verbatim; sq = n from 55972;
 *         dfs/lca 55932-55955 came with the same block); wrapped into a struct
 * ============================================================
 */
