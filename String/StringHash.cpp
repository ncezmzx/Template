#include <bits/stdc++.h>
using namespace std;

constexpr int N = 5e4 + 9;
mt19937_64 rng(random_device{}());
using u64 = unsigned long long;
using i128 = __int128;
const u64 msk = rng();
struct hash_t {
  static u64 shift(u64 x) { // xorshift scrambling, randomized by msk
    x ^= msk;
    x ^= (x << 5ull);
    x ^= (x >> 11ull);
    x ^= (x << 54ull);
    return x ^ msk;
  }
  static constexpr u64 hmod = (1LL << 61) - 1;
  u64 h;
  constexpr hash_t() : h{0} {}
  constexpr hash_t(u64 H) : h{H % hmod} {}
  hash_t &operator+=(hash_t rhs) {
    h += rhs.h, h >= hmod && (h -= hmod);
    return *this;
  }
  hash_t &operator-=(hash_t rhs) {
    h += hmod - rhs.h, h >= hmod && (h -= hmod);
    return *this;
  }
  hash_t &operator*=(hash_t rhs) { // multiply modulo 2^61-1 (Mersenne fold)
    i128 rt = (i128)h * rhs.h;
    h = u64(rt & hmod) + u64(rt >> 61);
    if (h >= hmod) h -= hmod;
    return *this;
  }
  friend hash_t operator+(hash_t lhs, const hash_t &rhs) { return lhs += rhs; }
  friend hash_t operator-(hash_t lhs, const hash_t &rhs) { return lhs -= rhs; }
  friend hash_t operator*(hash_t lhs, const hash_t &rhs) { return lhs *= rhs; }
  bool operator==(const hash_t &rhs) const { return h == rhs.h; }
  bool operator<(const hash_t &rhs) const { return h < rhs.h; }
};
const hash_t seed = uniform_int_distribution<u64>(hash_t::hmod >> 2, hash_t::hmod >> 1)(rng);
// polynomial rolling hash; string must be 1-indexed (prepend a dummy char)
struct strhash {
  int n;
  hash_t h[N], pw[N];
  void init(int m, string &a) {
    n = m, pw[0] = 1;
    for (int i = 1; i <= m; ++i) pw[i] = pw[i - 1] * seed;
    for (int i = 1; i <= m; ++i) h[i] = h[i - 1] * seed + hash_t::shift(a[i]);
  }
  hash_t query(int l, int r) const { // hash of a[l..r]
    return h[r] - h[l - 1] * pw[r - l + 1];
  }
};
/*
 * ============================================================
 * Name: string hashing (mod 2^61-1, random seed, xorshift scrambling)
 * Complexity: O(n) preprocessing, O(1) substring query
 * Usage: fast substring-hash comparison (equality / sorting), randomized
 *        against hacks; `strhash` (the power table pw lives inside the struct
 *        and the seed is a file-level random constant):
 *        init(n, s) preprocesses, query(l, r) hashes a substring.
 * Source: all.cpp lines 53386-53446 (wrapped into a struct, logic unchanged)
 * Notes: strings must be 1-indexed (s = " " + s)
 * ============================================================
 */
