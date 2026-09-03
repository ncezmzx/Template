#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int md = 998244353;
int qpow(int a, int b) {
  int r = 1;
  for (; b; b >>= 1, a = a * a % md)
    if (b & 1) r = r * a % md;
  return r;
}


template <size_t N> struct gauss_jordan {
  int a[N][N];

  int solve(int n) {
    int r = 0;
    for (int c = 0; c < n; ++c) {
      int p = r;
      while (p < n && !a[p][c]) ++p;
      if (p == n) continue;
      for (int j = c; j <= n; ++j) swap(a[p][j], a[r][j]);
      int inv = qpow(a[r][c], md - 2);
      for (int j = c; j <= n; ++j) a[r][j] = a[r][j] * inv % md;
      for (int i = 0; i < n; ++i)
        if (i != r && a[i][c]) {
          int t = a[i][c];
          for (int j = c; j <= n; ++j) {

            int v = a[i][j] - (int)(t * a[r][j] % md);
            a[i][j] = v < 0 ? v + md : v;
          }
        }
      ++r;
    }
    for (int i = r; i < n; ++i)
      if (a[i][n]) return 0;
    return r == n ? 1 : 2;
  }
};

