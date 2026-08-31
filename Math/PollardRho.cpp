#include <bits/stdc++.h>
using namespace std;
#define int long long

int mul_mod(int x, int y, int md) { return (__int128)x * y % md; }

int qpow(int a, int b, int md) {
  int r = 1;
  for (; b; b >>= 1, a = mul_mod(a, a, md))
    if (b & 1) r = mul_mod(r, a, md);
  return r;
}

bool is_prime(int n) {
  if (n == 2) return true;
  if (n <= 1 || (n & 1 ^ 1)) return false;
  int u = n - 1, t = 0;
  while (u & 1 ^ 1) u >>= 1, ++t;
  auto chk = [&](int a) -> bool {
    int k = qpow(a % n, u, n);
    if (k <= 1) return true;
    for (int i = 0; i < t; ++i) {
      if (k == n - 1) return true;
      k = mul_mod(k, k, n);
    }
    return false;
  };
  static constexpr int bas[7] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
  for (int i : bas) {
    if (n % i == 0) return n == i;
    if (!chk(i)) return false;
  }
  return true;
}

int pollard_rho(int n) {
  if (n % 2 == 0) return 2;
  if (n % 3 == 0) return 3;
  static mt19937_64 rng(random_device{}());
  while (true) {
    int c = rng() % (n - 1) + 1;
    int x = rng() % (n - 1) + 1, y = x, d = 1;
    auto f = [&](int v) { return (mul_mod(v, v, n) + c) % n; };
    while (d == 1) {
      int q = 1;
      for (int i = 0; i < 128 && d == 1; ++i) {
        x = f(x), y = f(f(y));
        if (x == y) {
          d = __gcd(q, n);
          if (d == 1) d = n;
          break;
        }
        q = mul_mod(q, x > y ? x - y : y - x, n);
        if (!(i & 31)) d = __gcd(q, n);
      }
      if (d == 1) d = __gcd(q, n);
    }
    if (d != n) return d;
  }
}

void factorize(int n, vector<int> &fac) {
  if (n == 1) return;
  if (is_prime(n)) return fac.push_back(n), void();
  int d = pollard_rho(n);
  factorize(d, fac), factorize(n / d, fac);
}

/*
 * ============================================================
 * Name: Pollard-Rho factorization (Floyd cycle detection, pairs with Miller-Rabin)
 * Complexity: expected O(n^{1/4}) to find a non-trivial factor; full
 *             factorization O(n^{1/4} log n)
 * Usage: factor 64-bit composites (e.g. ~1e18); check primality first (is_prime
 *        is embedded), then split with Pollard-Rho.
 *        After factorize(n, fac), fac holds every prime factor of n, with
 *        multiplicity and unordered.
 * Principle: the pseudo-random map f(x) = (x^2 + c) mod n eventually cycles;
 *            Floyd cycle detection takes differences and gcds them with n to
 *            hit a non-trivial factor; on failure (d == n) retry with a new
 *            random seed
 * Notes: #define int long long makes int 64-bit; mul_mod uses __int128 against
 *        overflow; relies on __gcd (a GNU extension, C++14-compatible)
 * ============================================================
 */
