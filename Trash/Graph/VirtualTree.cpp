#include <bits/stdc++.h>
using namespace std;
#define int long long


template <size_t N> struct virtual_tree {
  int n, idx;
  int dep[N], fa[N][20], dfn[N];
  vector<int> es[N], se[N];
  void dfs(int x, int ff) {
    fa[x][0] = ff, dep[x] = dep[ff] + 1, dfn[x] = ++idx;
    for (int i = 1; i < 20; ++i) fa[x][i] = fa[fa[x][i - 1]][i - 1];
    for (int y : es[x])
      if (y ^ ff) dfs(y, x);
  }
  int lca(int x, int y) {
    for (int i = 19; ~i; --i) {
      if (dep[fa[x][i]] >= dep[y]) x = fa[x][i];
      if (dep[fa[y][i]] >= dep[x]) y = fa[y][i];
    }
    if (x == y) return x;
    for (int i = 19; ~i; --i)
      if (fa[x][i] != fa[y][i]) x = fa[x][i], y = fa[y][i];
    return fa[x][0];
  }


  vector<int> build(vector<int> v) {
    sort(v.begin(), v.end(), [&](int x, int y) -> bool { return dfn[x] < dfn[y]; });
    int m = (int)v.size();
    for (int i = 0; i + 1 < m; ++i) v.push_back(lca(v[i], v[i + 1]));
    v.push_back(1);
    sort(v.begin(), v.end(), [&](int x, int y) -> bool { return dfn[x] < dfn[y]; });
    v.erase(unique(v.begin(), v.end()), v.end());
    for (int i = 0; i + 1 < (int)v.size(); ++i) se[lca(v[i], v[i + 1])].push_back(v[i + 1]);
    return v;
  }
};

