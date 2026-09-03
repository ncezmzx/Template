#include <bits/stdc++.h>
using namespace std;
#define int long long


template <size_t N> struct cantor {
  int n_, tr_[N];
  void tr_add(int x, int v) {
    for (; x <= n_; x += x & -x) tr_[x] += v;
  }
  int tr_sum(int x) {
    int r = 0;
    for (; x; x -= x & -x) r += tr_[x];
    return r;
  }

  int expand(const vector<int> &a, int mod) {
    n_ = a.size();
    vector<int> f(n_ + 1, 1 % mod);
    for (int i = 1; i <= n_; ++i) f[i] = f[i - 1] * i % mod;
    memset(tr_, 0, sizeof(int) * (n_ + 1));
    for (int i = 1; i <= n_; ++i) tr_add(i, 1);
    int res = 0;
    for (int i = 0; i < n_; ++i) {
      res = (res + tr_sum(a[i] - 1) % mod * f[n_ - 1 - i]) % mod;
      tr_add(a[i], -1);
    }
    return res;
  }

  vector<int> unexpand(int k, int n) {
    n_ = n;
    vector<int> fac(n, 1);
    for (int i = 1; i < n; ++i) fac[i] = fac[i - 1] * i;
    memset(tr_, 0, sizeof(int) * (n_ + 1));
    for (int i = 1; i <= n_; ++i) tr_add(i, 1);
    vector<int> a;
    for (int i = n - 1; i >= 0; --i) {
      int t = k / fac[i];
      k %= fac[i];
      int lo = 1, pos = 0;
      for (int j = 20; j >= 0; --j)
        if (pos + (1 << j) <= n_ && tr_[pos + (1 << j)] <= t) pos += 1 << j, t -= tr_[pos];
      ++pos;
      tr_add(pos, -1), a.push_back(pos);
    }
    return a;
  }
};

