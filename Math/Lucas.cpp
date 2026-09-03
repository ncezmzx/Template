#include <bits/stdc++.h>
using namespace std;
#define int long long

int pw(int x, int n, int p) {
  int r = 1 % p;
  x %= p;
  while (n) {
    if (n & 1) r = r * x % p;
    x = x * x % p, n >>= 1;
  }
  return r;
}
int exgcd(int a, int b, int &x, int &y) {
  if (!b) return x = 1, y = 0, a;
  int g = exgcd(b, a % b, y, x);
  y -= a / b * x;
  return g;
}
int inv_mod(int a, int m) {
  int x, y;
  exgcd(a, m, x, y);
  return (x % m + m) % m;
}


struct lucas {
  int p_;
  vector<int> fac_, ifac_;
  void init(int p) {
    p_ = p;
    fac_.assign(p, 1), ifac_.assign(p, 1);
    for (int i = 1; i < p; ++i) fac_[i] = fac_[i - 1] * i % p;
    ifac_[p - 1] = pw(fac_[p - 1], p - 2, p);
    for (int i = p - 1; i; --i) ifac_[i - 1] = ifac_[i] * i % p;
  }
  int c_small(int n, int m) {
    if (m < 0 || m > n) return 0;
    return fac_[n] * ifac_[m] % p_ * ifac_[n - m] % p_;
  }
  int solve(int n, int m) {
    if (m < 0 || m > n) return 0;
    if (m == 0) return 1;
    return c_small(n % p_, m % p_) * solve(n / p_, m / p_) % p_;
  }
};


int fact_pe(int n, int p, int pe) {
  if (!n) return 1;
  int res = 1;
  for (int i = 1; i <= pe; ++i)
    if (i % p) res = res * i % pe;
  res = pw(res, n / pe, pe);
  for (int i = 1; i <= n % pe; ++i)
    if (i % p) res = res * i % pe;
  return res * fact_pe(n / p, p, pe) % pe;
}
int C_mod_pe(int n, int m, int p, int pe, int e) {
  if (m < 0 || m > n) return 0;
  int k = 0;
  int a = n, b = m, c = n - m;
  while (a) a /= p, b /= p, c /= p, k += a - b - c;
  if (k >= e) return 0;
  int r = fact_pe(n, p, pe) * inv_mod(fact_pe(m, p, pe), pe) % pe;
  r = r * inv_mod(fact_pe(n - m, p, pe), pe) % pe;
  return r * pw(p, k, pe) % pe;
}
int exlucas(int n, int m, int P) {
  if (m < 0 || m > n) return 0;
  int r = 0, mod = 1;
  int PP = P;
  for (int p = 2; p * p <= PP; ++p)
    if (PP % p == 0) {
      int e = 0, pe = 1;
      while (PP % p == 0) PP /= p, ++e, pe *= p;
      int cr = C_mod_pe(n, m, p, pe, e), t = (cr - r) % pe;
      if (t < 0) t += pe;
      r += mod * (t * inv_mod(mod % pe, pe) % pe), mod *= pe;
    }
  if (PP > 1) {
    int cr = C_mod_pe(n, m, PP, PP, 1), t = (cr - r) % PP;
    if (t < 0) t += PP;
    r += mod * (t * inv_mod(mod % PP, PP) % PP), mod *= PP;
  }
  return r;
}

