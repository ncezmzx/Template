#include <bits/stdc++.h>
using namespace std;
#define int long long


template <size_t N> struct dominator_tree {
  int n, idx;
  int dfn[N], id[N], fa[N];
  int sdom[N], idom[N], mn[N], f[N];
  vector<int> g[N], rg[N], bucket[N];
  void add(int x, int y) { g[x].push_back(y), rg[y].push_back(x); }
  int find(int x) {
    if (f[x] == x) return x;
    int y = find(f[x]);
    if (sdom[y] < sdom[mn[x]]) mn[x] = y;
    f[x] = f[f[x]];
    return mn[x];
  }
  void dfs(int x) {
    dfn[x] = ++idx, id[idx] = x;
    for (int y : g[x])
      if (!dfn[y]) fa[y] = x, dfs(y);
  }
  void build(int n_, int s) {
    n = n_, idx = 0;
    for (int i = 1; i <= n; ++i) dfn[i] = idom[i] = 0, bucket[i].clear();
    dfs(s);
    for (int i = 1; i <= n; ++i) sdom[i] = dfn[i], mn[i] = f[i] = i;
    for (int i = idx; i >= 2; --i) {
      int x = id[i];
      for (int y : rg[x]) {
        if (!dfn[y]) continue;
        int w = find(y);
        if (sdom[w] < sdom[x]) sdom[x] = sdom[w];
      }
      bucket[id[sdom[x]]].push_back(x);
      f[x] = fa[x];
      for (int y : bucket[fa[x]]) {
        int w = find(y);
        idom[y] = sdom[w] == sdom[y] ? fa[x] : w;
      }
      bucket[fa[x]].clear();
    }
    for (int i = 2; i <= idx; ++i) {
      int x = id[i];
      if (idom[x] != id[sdom[x]]) idom[x] = idom[idom[x]];
    }
    idom[s] = 0;
  }
};

