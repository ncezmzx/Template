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


vector<int> factorize(int n) {
  vector<int> fs;
  for (int i = 2; i * i <= n; ++i)
    if (n % i == 0) {
      fs.push_back(i);
      while (n % i == 0) n /= i;
    }
  if (n > 1) fs.push_back(n);
  return fs;
}


int min_primitive_root(int p) {
  if (p == 2) return 1;
  vector<int> fs = factorize(p - 1);
  for (int g = 2;; ++g)
    if (all_of(fs.begin(), fs.end(), [&](int q) { return pw(g, (p - 1) / q, p) != 1; })) return g;
}


vector<int> primitive_roots(int p) {
  vector<int> res;
  if (p == 2) return {1};
  int g = min_primitive_root(p);
  for (int k = 1, cur = g; k < p - 1; ++k, cur = cur * g % p)
    if (__gcd(k, p - 1) == 1) res.push_back(cur);
  sort(res.begin(), res.end());
  return res;
}

