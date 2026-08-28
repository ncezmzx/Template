#include <bits/stdc++.h>
using namespace std;

struct dynbitset {
  using u64 = unsigned long long;
  static constexpr int W = 64;
  int n;
  vector<u64> a;

  dynbitset() : n(0) {}
  explicit dynbitset(int n_) { init(n_); }
  dynbitset(int n_, unsigned long long v) {
    init(n_);
    a[0] = v;
    norm();
  }

  void init(int n_) {
    n = max(0, n_);
    a.assign((n + W - 1) / W, 0);
  }

  void resize(int n_) {
    n_ = max(0, n_);
    int w = (n_ + W - 1) / W;
    a.resize(w);
    n = n_;
    norm();
  }

  int size() const { return n; }
  void clear() { fill(a.begin(), a.end(), 0); }

  void norm() {
    if (n == 0) return fill(a.begin(), a.end(), 0), void();
    int w = (n + W - 1) / W;
    if (n & 63) a[w - 1] &= (1ull << (n & 63)) - 1;
    for (int i = w; i < (int)a.size(); ++i) a[i] = 0;
  }

  bool get(int p) const { return (a[p >> 6] >> (p & 63)) & 1; }
  void set1(int p) { a[p >> 6] |= 1ull << (p & 63); }
  void set0(int p) { a[p >> 6] &= ~(1ull << (p & 63)); }
  void flip1(int p) { a[p >> 6] ^= 1ull << (p & 63); }
  bool test(int p) const { return get(p); }
  bool operator[](int p) const { return get(p); }

  dynbitset& set() { fill(a.begin(), a.end(), ~0ull); norm(); return *this; }
  dynbitset& set(int p) { set1(p); return *this; }
  dynbitset& reset() { fill(a.begin(), a.end(), 0); return *this; }
  dynbitset& reset(int p) { set0(p); return *this; }
  dynbitset& flip() {
    for (auto& x : a) x = ~x;
    norm();
    return *this;
  }
  dynbitset& flip(int p) { flip1(p); return *this; }

  bool any() const {
    for (auto x : a)
      if (x) return true;
    return false;
  }
  bool none() const { return !any(); }
  bool all() const {
    if (n == 0) return true;
    int w = (n + W - 1) / W;
    for (int i = 0; i + 1 < w; ++i)
      if (a[i] != ~0ull) return false;
    u64 tail = a[w - 1];
    if (n & 63) tail |= ~((1ull << (n & 63)) - 1);
    return tail == ~0ull;
  }
  int count() const {
    int r = 0;
    for (auto x : a) r += __builtin_popcountll(x);
    return r;
  }

  int find_first() const {
    for (int i = 0; i < (int)a.size(); ++i)
      if (a[i]) return (i << 6) + __builtin_ctzll(a[i]);
    return n;
  }
  int find_next(int p) const {
    if (p < 0) return find_first();
    int w = p >> 6, b = p & 63;
    for (int i = w; i < (int)a.size(); ++i) {
      u64 x = a[i];
      if (i == w) x &= b == 63 ? 0 : ~((1ull << (b + 1)) - 1);
      if (x) return (i << 6) + __builtin_ctzll(x);
    }
    return n;
  }

  dynbitset& operator&=(const dynbitset& o) {
    int m = min((int)a.size(), (int)o.a.size());   // two loops remove per-element bound checks
    for (int i = 0; i < m; ++i) a[i] &= o.a[i];
    for (int i = m; i < (int)a.size(); ++i) a[i] = 0;
    // AND cannot create out-of-range bits and high words are already zero; no norm() needed
    return *this;
  }
  dynbitset& operator|=(const dynbitset& o) {
    if ((int)o.a.size() > (int)a.size()) a.resize(o.a.size());
    for (int i = 0; i < (int)o.a.size(); ++i) a[i] |= o.a[i];
    norm();
    return *this;
  }
  dynbitset& operator^=(const dynbitset& o) {
    if ((int)o.a.size() > (int)a.size()) a.resize(o.a.size());
    for (int i = 0; i < (int)o.a.size(); ++i) a[i] ^= o.a[i];
    norm();
    return *this;
  }
  dynbitset operator~() const {
    dynbitset r = *this;
    return r.flip();
  }
  friend dynbitset operator&(dynbitset a, const dynbitset& b) { return a &= b; }
  friend dynbitset operator|(dynbitset a, const dynbitset& b) { return a |= b; }
  friend dynbitset operator^(dynbitset a, const dynbitset& b) { return a ^= b; }

  dynbitset& operator<<=(int k) {
    if (k <= 0) return *this;
    if (k >= n) return reset(), *this;
    int ws = k >> 6, bs = k & 63;
    for (int i = (int)a.size() - 1; i >= 0; --i) {
      u64 cur = i - ws >= 0 ? a[i - ws] : 0;
      u64 hi = i - ws - 1 >= 0 ? a[i - ws - 1] : 0;
      a[i] = bs ? (cur << bs) | (hi >> (W - bs)) : cur;
    }
    for (int i = 0; i < ws; ++i) a[i] = 0;
    norm();
    return *this;
  }
  dynbitset& operator>>=(int k) {
    if (k <= 0) return *this;
    if (k >= n) return reset(), *this;
    int ws = k >> 6, bs = k & 63;
    for (int i = 0; i < (int)a.size(); ++i) {
      u64 lo = i + ws < (int)a.size() ? a[i + ws] : 0;
      u64 hi = i + ws + 1 < (int)a.size() ? a[i + ws + 1] : 0;
      a[i] = bs ? (lo >> bs) | (hi << (W - bs)) : lo;
    }
    for (int i = max(0, (int)a.size() - ws); i < (int)a.size(); ++i) a[i] = 0;
    norm();
    return *this;
  }
  friend dynbitset operator<<(dynbitset a, int k) { return a <<= k; }
  friend dynbitset operator>>(dynbitset a, int k) { return a >>= k; }

  bool operator==(const dynbitset& o) const {
    int m = max((int)a.size(), (int)o.a.size());
    for (int i = 0; i < m; ++i) {
      u64 x = i < (int)a.size() ? a[i] : 0, y = i < (int)o.a.size() ? o.a[i] : 0;
      if (x != y) return false;
    }
    return true;
  }
  bool operator!=(const dynbitset& o) const { return !(*this == o); }

  string to_string() const {
    string s(n, '0');
    for (int i = 0; i < n; ++i) s[i] = char('0' + get(n - 1 - i));
    return s;
  }
  static dynbitset from_string(const string& s) {
    dynbitset r((int)s.size());
    for (int i = 0; i < (int)s.size(); ++i)
      if (s[i] == '1') r.set1((int)s.size() - 1 - i);
    return r;
  }
};

/*
 * ============================================================
 * Name: hand-written dynamic bitset (all std::bitset operations + runtime resizing)
 * Complexity: 64-bit word granularity; and/or/xor/shift/popcount are O(n/64); single bits O(1)
 * Usage: bitset acceleration (reachability, subset convolution, string
 *        matching, DP state compression) when the length must be dynamic or
 *        larger than a compile-time constant allows (std::bitset needs one)
 * Notes: stored as 64-bit words, low bits first; find_first/find_next use
 *        ctz, ideal for enumerating all set bits (total O(#words + #ones));
 *        shifts preserve the length (overflow dropped, high bits zero);
 *        resize changes the logical length and clears; mixed-length & | ^
 *        use the longer length (missing bits read as 0); to_string prints
 *        most significant first
 * ============================================================
 * Example (uncomment to compile):
 * signed main() {
 *   dynbitset a = dynbitset::from_string("10101"), b(10);
 *   b.set(2), b.set(9);
 *   cout << a.to_string() << '\n';                    // 00101
 *   cout << (a & b).to_string() << '\n';              // 00100
 *   a <<= 3;
 *   cout << a.to_string() << '\n';                    // 01000
 *   cout << a.count() << ' ' << a.find_first() << ' ' << a.find_next(2) << '\n'; // 1 3 10
 *   a.resize(100), a.set(99);
 *   cout << a.size() << ' ' << a.test(99) << '\n';    // 100 1
 *   cout << (a.all() ? "all" : "notall") << '\n';
 * }
 * ============================================================
 */
