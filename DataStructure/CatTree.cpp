#include <bits/stdc++.h>
using namespace std;
template <class T, size_t N> struct cattree {
  T ct[__lg(N) + 2][N];
  void build(int n, const T a[]) {
    for (int h = 0, s = 1; s <= n; ++h, s <<= 1) {
      for (int i = s; i <= n; i += s << 1) {
        ct[h][i - 1] = a[i - 1];
        for (int j = i - 1; j > i - s; --j) ct[h][j - 1] = a[j - 1] + ct[h][j];
        if (i == n) break;
        ct[h][i] = a[i];
        for (int j = i + 1; j < i + s && j < n; ++j) ct[h][j] = ct[h][j - 1] + a[j];
      }
    }
  }
  T query(int l, int r) {
    if (l == r) return ct[0][l];
    int h = __lg(l ^ r);
    return ct[h][l] + ct[h][r];
  }
};

