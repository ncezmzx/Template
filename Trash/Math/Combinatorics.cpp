#include <bits/stdc++.h>
using namespace std;
#define int long long

// binomial coefficients modulo a prime (factorials + inverse factorials)
template <size_t N> struct combinatorics {
  static constexpr int md = 998244353;
  int fac[N], inv[N]; // inv = inverse factorials: inv[i] = 1 / i!
  int qpow(int a, int b = md - 2) {
    int r = 1;
    for (; b; b >>= 1, a = a * a % md)
      if (b & 1) r = r * a % md;
    return r;
  }
  void init(int n) { // O(n) preprocessing of fac / inv up to n
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
// ---- O(n) linear inverse table variant (all.cpp 915-916), when every 1..n inverse is needed ----
// inv[0] = inv[1] = 1;
// for (int i = 2; i <= n; ++i) inv[i] = (md - md / i * inv[md % i] % md) % md;

/*
 * ============================================================
 * Name: binomial coefficients (fac / inverse factorials / C(n, m))
 * Complexity: preprocessing O(n) (one qpow at O(log md)); C(n, m) O(1)
 * Usage: binomial coefficients modulo a prime, wrapped as combinatorics<N>:
 *        init(n) preprocesses fac/inv, then C(n, m); combine with
 *        inclusion-exclusion or DP as needed
 * Source: all.cpp 10821-10833 (qpow + C(n,m); fac/inv preprocessing from
 *         10842-10844), wrapped with init()
 * ============================================================
 * Example (uncomment to compile):
 * static combinatorics<100009> cb;
 * signed main() {
 *   cb.init(10);
 *   cout << cb.C(10, 3) << ' ' << cb.C(5, 5) << ' ' << cb.C(3, 5) << '\n';  // 120 1 0
 * }
 * ============================================================
 */
