#include <bits/stdc++.h>
using namespace std;


template <size_t N> struct lca_binary_lifting {
  int dep[N], fa[N][20];
  vector<int> e[N];
  void dfs(int u, int f) {
    dep[u] = dep[f] + 1;
    fa[u][0] = f;
    for (int i = 1; i < 20; i++) fa[u][i] = fa[fa[u][i - 1]][i - 1];
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

