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

// factor n (trial division; fine up to n <= 1e12), returns distinct prime factors
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

// smallest primitive root (p prime; g is one iff g^{(p-1)/q} != 1 for every prime q | p-1)
int min_primitive_root(int p) {
  if (p == 2) return 1;
  vector<int> fs = factorize(p - 1);
  for (int g = 2;; ++g)
    if (all_of(fs.begin(), fs.end(), [&](int q) { return pw(g, (p - 1) / q, p) != 1; })) return g;
}

// all primitive roots are g^k with gcd(k, p-1) = 1; there are phi(p-1) of them
vector<int> primitive_roots(int p) {
  vector<int> res;
  if (p == 2) return {1};
  int g = min_primitive_root(p);
  for (int k = 1, cur = g; k < p - 1; ++k, cur = cur * g % p)
    if (__gcd(k, p - 1) == 1) res.push_back(cur);
  sort(res.begin(), res.end());
  return res;
}

/*
 * ============================================================
 * Name: primitive roots (smallest / all)
 * Complexity: trial-division factorization of p-1 is O(sqrt p); enumeration
 *             O(sqrt p + omega(p) log p)
 * Usage: min_primitive_root(p) is the smallest primitive root g of the prime p;
 *        primitive_roots(p) lists all phi(p-1) of them; typical uses are the
 *        NTT modulus generator g and discrete logarithms (indices).
 * Principle: g is a primitive root mod p iff its order is p-1, i.e. iff
 *            g^{(p-1)/q} != 1 (mod p) for every prime factor q of p-1; the
 *            smallest primitive root is usually tiny (< ~300)
 * Notes: p must be prime (p = 2 returns 1); p-1 is factored by trial division,
 *        which is fine up to p <= 1e12
 * ============================================================
 */
