#include <bits/stdc++.h>
using namespace std;
#define int long long

// Cantor expansion: rank (0-based, in [0, n!)) of a permutation, via BIT
template <size_t N>
struct cantor {
  int n_, tr_[N];
  void tr_add(int x, int v) {
    for (; x <= n_; x += x & -x) tr_[x] += v;
  }
  int tr_sum(int x) {
    int r = 0;
    for (; x; x -= x & -x) r += tr_[x];
    return r;
  }
  // rank of permutation a (values 1..n, distinct) modulo mod
  int expand(const vector<int>& a, int mod) {
    n_ = a.size();
    vector<int> f(n_ + 1, 1 % mod);  // f[j] = j! mod
    for (int i = 1; i <= n_; ++i) f[i] = f[i - 1] * i % mod;
    memset(tr_, 0, sizeof(int) * (n_ + 1));
    for (int i = 1; i <= n_; ++i) tr_add(i, 1);  // set of unused values
    int res = 0;
    for (int i = 0; i < n_; ++i) {
      res = (res + tr_sum(a[i] - 1) % mod * f[n_ - 1 - i]) % mod;
      tr_add(a[i], -1);
    }
    return res;
  }
  // inverse: rank k (0 <= k < n!) back to the permutation; n <= 20
  vector<int> unexpand(int k, int n) {
    n_ = n;
    vector<int> fac(n, 1);
    for (int i = 1; i < n; ++i) fac[i] = fac[i - 1] * i;  // n! < 2^63 for n <= 20
    memset(tr_, 0, sizeof(int) * (n_ + 1));
    for (int i = 1; i <= n_; ++i) tr_add(i, 1);
    vector<int> a;
    for (int i = n - 1; i >= 0; --i) {
      int t = k / fac[i];  // unused values smaller than current position
      k %= fac[i];
      int lo = 1, pos = 0;  // BIT binary lifting for the (t+1)-th smallest
      for (int j = 20; j >= 0; --j)
        if (pos + (1 << j) <= n_ && tr_[pos + (1 << j)] <= t) pos += 1 << j, t -= tr_[pos];
      ++pos;
      tr_add(pos, -1), a.push_back(pos);
    }
    return a;
  }
};

/*
 * ============================================================
 * Name: Cantor expansion / inverse Cantor expansion
 * Complexity: both directions O(n log n)
 * Usage: permutation <-> lexicographic rank (0-based), `cantor<N>`: expand(a,
 *        mod) is the rank of permutation a modulo mod;
 *        unexpand(k, n) rebuilds the permutation of rank k (0 <= k < n!).
 * Principle: rank = sum_i (# unused values smaller than a_i before position i)
 *            * (n-1-i)!; a BIT maintains the unused set, and the inverse
 *            expansion binary-lifts to the (t+1)-th smallest unused value
 * Notes: a must be a permutation of 1..n (or distinct values compared by
 *        order); unexpand needs n <= 20 (n! < 2^63); after reducing ranks
 *        modulo something, the inverse needs the original k
 * ============================================================
 */
