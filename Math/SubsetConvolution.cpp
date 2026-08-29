#include <bits/stdc++.h>
using namespace std;
#define int long long

// subset convolution: h[S] = sum_{T subset S} f[T] * g[S\T] (mod MOD)
// popcount layering + OR zeta/Mobius transforms; O(n^2 2^n)
constexpr int MOD = 998244353;
inline void addm(int& x, int y) { (x += y) >= MOD && (x -= MOD); }
inline void subm(int& x, int y) { (x -= y) < 0 && (x += MOD); }

// f, g have length 2^n indexed by subset bitmask; returns the length-2^n convolution
vector<int> subset_convolution(int n, const vector<int>& f, const vector<int>& g) {
  int m = 1 << n;
  vector<vector<int>> F(n + 1, vector<int>(m)), G(n + 1, vector<int>(m)), H(n + 1, vector<int>(m));
  for (int s = 0; s < m; ++s) {
    F[__builtin_popcount((unsigned)s)][s] = f[s];
    G[__builtin_popcount((unsigned)s)][s] = g[s];
  }
  for (int k = 0; k <= n; ++k)  // OR zeta transform (subset sums)
    for (int i = 0; i < n; ++i)
      for (int s = 0; s < m; ++s)
        if (s >> i & 1) addm(F[k][s], F[k][s ^ (1 << i)]), addm(G[k][s], G[k][s ^ (1 << i)]);
  for (int k = 0; k <= n; ++k)
    for (int j = 0; j <= k; ++j)
      for (int s = 0; s < m; ++s)
        H[k][s] = (H[k][s] + F[j][s] * G[k - j][s]) % MOD;
  for (int k = 0; k <= n; ++k)  // OR Mobius inversion
    for (int i = 0; i < n; ++i)
      for (int s = 0; s < m; ++s)
        if (s >> i & 1) subm(H[k][s], H[k][s ^ (1 << i)]);
  vector<int> h(m);
  for (int s = 0; s < m; ++s) h[s] = H[__builtin_popcount((unsigned)s)][s];
  return h;
}

/*
 * ============================================================
 * Name: subset convolution
 * Complexity: O(n^2 2^n) (n = log2 of the length; the naive form is O(3^n))
 * Usage: h[S] = sum_{T subset S} f[T] g[S\T] with the |T| + |S\T| = |S|
 *        constraint; common in set power series, set-partition counting and
 *        tree-set DP merges.
 *        subset_convolution(n, f, g) with f / g of length 2^n returns h of
 *        length 2^n.
 * Interface: subset_convolution(n, f, g), f/g of length 2^n, returns h of length 2^n
 * Principle: layer by popcount into F[k][S] keeping only the |S| = k terms; OR-
 *            zeta each layer, convolve the layers (a univariate polynomial
 *            product per S), Mobius-invert, then read back H[popcount(S)][S]
 * Notes: results are taken mod MOD; f / g lengths must be powers of two; MOD
 *        should be prime (only multiplications are reduced)
 * Source: OI-Wiki "Set power series / subset convolution" (https://oi-wiki.org/math/poly/sps/)
 * ============================================================
 */
