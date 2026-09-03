#include <bits/stdc++.h>
using namespace std;
#define int long long


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

  void build(int n_) {
    n = n_, cl = idx = tp = 0;
    for (int i = 1; i <= n; ++i) dfn[i] = vst[i] = 0;
    for (int i = 1; i <= n; ++i)
      if (!dfn[i]) tarjan(i);
  }
};

