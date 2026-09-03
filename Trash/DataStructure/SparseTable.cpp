#include <bits/stdc++.h>
using namespace std;


template <size_t N> struct sparse_table {
  int n;
  int a[N], d[N][20], lg[N];
  void init(int n_) {
    n = n_;
    lg[0] = -1;
    for (int i = 1; i <= n; ++i) d[i][0] = a[i], lg[i] = lg[i >> 1] + 1;
    for (int j = 1; (1 << j) <= n; ++j)
      for (int i = 1; i + (1 << (j - 1)) <= n; ++i) d[i][j] = min(d[i][j - 1], d[i + (1 << (j - 1))][j - 1]);
  }
  int query(int l, int r) {
    int k = lg[r - l + 1];
    return min(d[l][k], d[r - (1 << k) + 1][k]);
  }
};

