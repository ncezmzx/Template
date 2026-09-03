#include <bits/stdc++.h>
using namespace std;
#define int long long


template <size_t N> struct combinatorics {
  static constexpr int md = 998244353;
  int fac[N], inv[N];
  int qpow(int a, int b = md - 2) {
    int r = 1;
    for (; b; b >>= 1, a = a * a % md)
      if (b & 1) r = r * a % md;
    return r;
  }
  void init(int n) {
    fac[0] = 1;
    for (int i = 1; i <= n; ++i) fac[i] = fac[i - 1] * i % md;
    inv[n] = qpow(fac[n]);
    for (int i = n - 1; i >= 0; --i) inv[i] = inv[i + 1] * (i + 1) % md;
  }
  int C(int n, int m) {
    if (n < m) return 0;
    return fac[n] * inv[m] % md * inv[n - m] % md;
  }
};

