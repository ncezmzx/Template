#include <bits/stdc++.h>
using namespace std;

// random-seeded rolling / concatenable hash (mod 2^61-1)
mt19937_64 rng(random_device{}());
using u64 = unsigned long long;
using i128 = __int128;
const u64 msk = rng();
struct hash_t {
  static u64 shift(u64 x) {
    x ^= msk;
    x ^= (x << 5ull);
    x ^= (x >> 11ull);
    x ^= (x << 54ull);
    return x ^ msk;
  }
  static constexpr u64 hmod = (1LL << 61) - 1;
  u64 h;
  constexpr hash_t() : h{0} {}
  template <class T> constexpr hash_t(T H) : h{H % hmod} {}
  hash_t &operator+=(hash_t rhs) {
    h += rhs.h, h >= hmod && (h -= hmod);
    return *this;
  }
  hash_t &operator-=(hash_t rhs) {
    h += hmod - rhs.h, h >= hmod && (h -= hmod);
    return *this;
  }
  hash_t &operator*=(hash_t rhs) {
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
template <size_t N> struct rolling_hash {
  hash_t hsh[N], pw[N];        // prefix hashes and powers
  void init(const string &s) { // s is 1-indexed (prepend a placeholder)
    int m = (int)s.size() - 1;
    pw[0] = 1;
    for (int i = 1; i <= m; ++i) pw[i] = pw[i - 1] * seed;
    for (int j = 1; j <= m; ++j) hsh[j] = hsh[j - 1] * seed + s[j];
  }
  hash_t query(int l, int r) const { // substring hash, 1-indexed inclusive
    return hsh[r] - hsh[l - 1] * pw[r - l + 1];
  }
};

/*
 * ============================================================
 * Name: random-seeded rolling hash / concatenable hash (mod 2^61-1)
 * Complexity: O(n) preprocessing, O(1) substring query / concatenation
 * Usage: substring hash comparison and hash concatenation on trees/graphs
 *        (Hash(x*y) = Hash(x)*pw[|y|] + Hash(y)), wrapped as rolling_hash<N>:
 *        init(' ' + str), query(l, r); hash_t is linear, so given Hash(a) and
 *        Hash(b) with |b| = len, the concatenation a*b hashes to
 *        Hash(a)*pw[len] + Hash(b)
 * Source: all.cpp lines 51593-51638 (hash_t: templated constructor, chars
 *         construct directly); all.cpp lines 51640-51649 (pw / hsh / query
 *         prefix hashes, now inside rolling_hash<N>); the concatenation trick
 *         from all.cpp 51674/51683
 * ============================================================
 * Example (uncomment to compile):
 * static rolling_hash<100009> rh;
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   string s;
 *   cin >> s;
 *   s = ' ' + s;                       // 1-indexed
 *   rh.init(s);
 *   int m = (int)s.size() - 1;
 *   // substring hashes (1-indexed inclusive)
 *   auto h1 = rh.query(1, 3), h2 = rh.query(4, m);
 *   cout << (h1 == h2) << '\n';        // are the two substrings equal
 *   // concatenation: Hash(s[1..3] * s[4..m]) = h1 * pw[m - 3] + h2 == query(1, m)
 *   hash_t cat = h1 * rh.pw[m - 3] + h2;
 *   cout << (cat == rh.query(1, m)) << '\n';
 *   return 0;
 * }
 * ============================================================
 */
