#include <bits/stdc++.h>
using namespace std;

// Berlekamp-Massey: shortest recurrence s_n = c1*s_{n-1} + ... + cd*s_{n-d}
// for sequence s over a prime field; needs |s| >= 2d terms
// k-th term (0-indexed); s must contain >= 2*order terms

#define int long long

int pw_(int x, int n, int mod) {
  int r = 1 % mod;
  x %= mod;
  if (x < 0) x += mod;
  while (n) {
    if (n & 1) r = r * x % mod;
    x = x * x % mod, n >>= 1;
  }
  return r;
}

vector<int> BM(const vector<int> &s, int mod) {
  int n = s.size(), L = 0, m = 0;
  vector<int> C(n + 1, 0), B(n + 1, 0), T;
  C[0] = B[0] = 1;
  int b = 1;
  for (int i = 0; i < n; ++i) {
    m++;
    int d = s[i] % mod;
    for (int j = 1; j <= L; ++j) d = (d + C[j] * s[i - j]) % mod;
    if (!d) continue;
    T = C;
    int coef = d * pw_(b, mod - 2, mod) % mod;
    for (int j = m; j <= n; ++j) C[j] = (C[j] - coef * B[j - m] % mod + mod) % mod;
    if (2 * L <= i) L = i + 1 - L, B = T, b = d, m = 0;
  }
  C.resize(L + 1);
  C.erase(C.begin());
  for (auto &x : C) x = (mod - x) % mod;
  return C;
}

vector<int> polymul(const vector<int> &a, const vector<int> &b, const vector<int> &rec, int mod) {
  int d = rec.size();
  vector<int> c(a.size() + b.size() - 1, 0);
  for (int i = 0; i < (int)a.size(); ++i)
    for (int j = 0; j < (int)b.size(); ++j) c[i + j] = (c[i + j] + a[i] * b[j]) % mod;
  for (int k = (int)c.size() - 1; k >= d; --k)
    for (int i = 1; i <= d; ++i) c[k - i] = (c[k - i] + c[k] * rec[i - 1]) % mod;
  c.resize(d);
  return c;
}

int kth_term(const vector<int> &s, int k, int mod) {
  vector<int> rec = BM(s, mod);
  int d = rec.size();
  if (d == 0) return 0;
  if (k < (int)s.size()) return s[k] % mod;
  vector<int> res(1, 1), base(2, 0);
  base[1] = 1;
  for (int e = k; e; e >>= 1) {
    if (e & 1) res = polymul(res, base, rec, mod);
    base = polymul(base, base, rec, mod);
  }
  int ans = 0;
  for (int i = 0; i < d && i < (int)s.size(); ++i) ans = (ans + res[i] * s[i]) % mod;
  return ans;
}

/*
 * ============================================================
 * Name: Berlekamp-Massey + Kitamasa (k-th term of a linear recurrence)
 * Complexity: BM O(n^2); kth_term O(d^2 log k) (d = recurrence order)
 * Usage: given the first terms of a sequence s (modulo a prime), BM finds the
 *        shortest linear recurrence automatically, then the k-th term costs
 *        O(d^2 log k) for k up to 1e18;
 *        a typical alternative to matrix fast exponentiation (smaller constants
 *        when the data is recurrent).
 * Principle: BM incrementally maintains the characteristic polynomial C and the
 *            last discrepancy B, correcting C by B/b on a discrepancy and
 *            updating B when the length more than doubles; the k-th term is x^k
 *            mod f(x) with f = x^d - sum ci*x^{d-i}, and the answer is sum (x^k
 *            mod f)_i * s_i
 * Notes: mod must be prime (BM uses inverses); reduce s entries to [0, mod)
 *        first; s needs at least 2d terms for correctness; rec[i-1] is the
 *        coefficient of x^{d-i}, i.e. c_i
 * ============================================================
 */
