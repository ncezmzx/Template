#include <bits/stdc++.h>
using namespace std;
#define int long long


template <size_t N> struct tree_reroot {
  int n, m;
  int sz[N], key[N], dst[N], f[N];
  vector<int> se[N];

  void dfs2(int x) {
    sz[x] = f[x] = key[x];
    for (int y : se[x]) dfs2(y), f[x] += f[y] + dst[y] * sz[y], sz[x] += sz[y];
  }

  void dfs3(int x) {
    for (int y : se[x]) f[y] = f[x] + dst[y] * (m - 2 * sz[y]), dfs3(y);
  }
};

