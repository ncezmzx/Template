#include <bits/stdc++.h>
using namespace std;


template <size_t N> struct block_cut_tree {
  int n, sq, idx, tp;
  int dfn[N], low[N], stk[N];
  int dep[N << 1], fa[N << 1][20];
  vector<int> g[N], e[N << 1];
  void tarjan(int u) {
    dfn[u] = low[u] = ++idx;
    stk[++tp] = u;
    for (int v : g[u]) {
      if (!dfn[v]) {
        tarjan(v);
        low[u] = min(low[u], low[v]);
        if (low[v] >= dfn[u]) {
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

  void build(int n_) {
    n = n_, sq = n, idx = tp = 0;
    for (int i = 1; i <= n; ++i) dfn[i] = 0;
    for (int i = 1; i <= n; ++i)
      if (!dfn[i]) tarjan(i);
  }
  void dfs(int u, int f) {
    dep[u] = dep[f] + 1;
    fa[u][0] = f;
    for (int i = 1; fa[u][i - 1]; i++) fa[u][i] = fa[fa[u][i - 1]][i - 1];
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

