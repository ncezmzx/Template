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

int lagrange(const vector<int>& x, const vector<int>& y, int k) {
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
int lagrange_1n(const vector<int>& y, int k) {
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
 * ============================================================
 * Name: Lagrange interpolation
 * Complexity: general form O(n^2); O(n) for consecutive nodes x = 1..n
 * Usage: given n+1 points (x_i, y_i) (distinct x_i), evaluate the unique
 *        polynomial of degree <= n at any point k:
 *        1) lagrange(x, y, k): general abscissas (need not be an arithmetic
 *           progression modulo the prime);
 *        2) lagrange_1n(y, k): O(n) version for abscissas 1..n (prefix/
 *           suffix products + factorial inverses), typical for "evaluate a
 *           degree-n polynomial at k" (k may far exceed n, even a modular
 *           negative)
 * Principle: Lagrange basis polynomials L_i(k) = prod_{j!=i} (k - x_j) /
 *        (x_i - x_j); answer = sum y_i * L_i(k)
 * Notes: denominators must be non-zero (prime modulus + distinct x_i); in
 *        lagrange_1n, y is indexed 0..n against abscissas 1..n (y[0]
 *        unused); pass k as long long, reduced internally
 * ============================================================
 * Example (uncomment to compile):

 * signed main() {
 *   // the quadratic through (0,1),(1,2),(2,4) is f(k)=k^2+1
 *   cout << lagrange({0, 1, 2}, {1, 2, 4}, 5) << '\n';       // 26
 *   // 1^2+2^2+...+n^2 = n(n+1)(2n+1)/6 is cubic in n:
 *   // f(1)=1, f(2)=5, f(3)=14, f(4)=30 -> evaluate f(1e9)
 *   vector<int> y = {0, 1, 5, 14, 30};
 *   cout << lagrange_1n(y, 1000000000) << '\n';
 * }
 * ============================================================
 */
