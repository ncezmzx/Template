#include <bits/stdc++.h>
using namespace std;
#define int long long


template <size_t N> struct cut_vertex {
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
    if (x == rt && sz > 1) is_cut[x] = 1;
  }
  void build(int n_) {
    n = n_, idx = 0;
    for (int i = 1; i <= n; ++i) dfn[i] = is_cut[i] = 0;
    for (int i = 1; i <= n; ++i)
      if (!dfn[i]) rt = i, tarjan(i);
  }
};

