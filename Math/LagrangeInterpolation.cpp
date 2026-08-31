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

int lagrange(const vector<int> &x, const vector<int> &y, int k) {
  int n = (int)x.size(), res = 0;
  for (int i = 0; i < n; ++i) {
    int num = 1, den = 1;
    for (int j = 0; j < n; ++j)
      if (j != i) {
        num = num * ((k - x[j] + md) % md) % md;
        den = den * ((x[i] - x[j] + md) % md) % md;
      }
    res = (res + y[i] * num % md * qpow(den, md - 2)) % md;
  }
  return res;
}

// O(n) version for nodes x = 1..n (prefix/suffix products + factorial inverses)
int lagrange_1n(const vector<int> &y, int k) {
  int n = (int)y.size() - 1;
  if (k >= 1 && k <= n) return y[k];
  if (n == 0) return 0;
  vector<int> fac(n + 1), ifac(n + 1);
  fac[0] = ifac[0] = 1;
  for (int i = 1; i <= n; ++i) fac[i] = fac[i - 1] * i % md;
  ifac[n] = qpow(fac[n], md - 2);
  for (int i = n - 1; i >= 1; --i) ifac[i] = ifac[i + 1] * (i + 1) % md;
  vector<int> pre(n + 1), suf(n + 2);
  pre[0] = 1;
  for (int i = 1; i <= n; ++i) pre[i] = pre[i - 1] * ((k - i + md) % md) % md;
  suf[n + 1] = 1;
  for (int i = n; i >= 1; --i) suf[i] = suf[i + 1] * ((k - i + md) % md) % md;
  int res = 0;
  for (int i = 1; i <= n; ++i) {
    int a = pre[i - 1] * suf[i + 1] % md;
    int b = ifac[i - 1] * ifac[n - i] % md;
    if ((n - i) & 1) b = (md - b) % md;
    res = (res + y[i] * a % md * b) % md;
  }
  return res;
}

/*
 * ============================================================
 * Name: Lagrange interpolation
 * Complexity: general form O(n^2); O(n) for consecutive abscissas x = 1..n
 * Usage: evaluate the unique polynomial of degree <= n through n+1 points (x_i,
 *        y_i), distinct x_i, at any point k:
 *        lagrange(x, y, k) for general abscissas; lagrange_1n(y, k) is the O(n)
 *        version for abscissas 1..n (k may far exceed n, even be a modular
 *        negative).
 * Principle: Lagrange basis polynomials L_i(k) = prod_{j!=i} (k - x_j) / (x_i -
 *            x_j); the answer is sum y_i * L_i(k)
 * Notes: denominators must be non-zero (prime modulus plus distinct x_i); in
 *        lagrange_1n, y is indexed 0..n against abscissas 1..n, so y[0] is
 *        unused; pass k as long long, it is reduced internally
 * ============================================================
 */
