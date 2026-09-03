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

