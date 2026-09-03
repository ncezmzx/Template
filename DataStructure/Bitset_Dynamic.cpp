#include <bits/stdc++.h>
using namespace std;

struct Bitset {
  using u64 = unsigned long long;
  static constexpr u64 W = u64(-1);
  vector<u64> v;
  int n, len;
  u64 mask;
  Bitset() = default;
  Bitset(int l) : n((l >> 6) + 1), len(l), mask((1ULL << (l & 63)) - 1) { v.resize(n); }
  void trim() { v[len >> 6] &= mask; }

  Bitset &operator=(const Bitset &a) {
    v = a.v, len = a.len, n = a.n;
    return *this;
  }
  Bitset &operator&=(const Bitset &a) {
    for (int i = 0; i < n; ++i) v[i] &= a.v[i];
    return *this;
  }
  friend Bitset operator&(Bitset a, const Bitset &b) { return a &= b; }
  Bitset &operator|=(const Bitset &a) {
    for (int i = 0; i < n; ++i) v[i] |= a.v[i];
    return *this;
  }
  friend Bitset operator|(Bitset a, const Bitset &b) { return a |= b; }
  Bitset &operator^=(const Bitset &a) {
    for (int i = 0; i < n; ++i) v[i] ^= a.v[i];
    return *this;
  }
  friend Bitset operator^(Bitset a, const Bitset &b) { return a ^= b; }
  Bitset &operator-=(const Bitset &a) {
    for (int i = 0; i < n; ++i) v[i] &= ~a.v[i];
    return *this;
  }
  friend Bitset operator-(Bitset a, const Bitset &b) { return a -= b; }

  friend bool operator==(const Bitset &a, const Bitset &b) {
    return memcmp(a.v.data(), b.v.data(), sizeof(u64) * a.n) == 0;
  }
  friend bool operator!=(const Bitset &a, const Bitset &b) { return !(a == b); }
  friend bool operator<=(const Bitset &a, const Bitset &b) {
    for (int i = 0; i < a.n; ++i)
      if ((a.v[i] & b.v[i]) != a.v[i]) return false;
    return true;
  }
  friend bool operator>=(const Bitset &a, const Bitset &b) { return b <= a; }
  friend bool operator<(const Bitset &a, const Bitset &b) {
    bool neq = false;
    for (int i = 0; i < a.n; ++i)
      if (neq |= (a.v[i] != b.v[i]), (a.v[i] & b.v[i]) != a.v[i]) return false;
    return neq;
  }
  friend bool operator>(const Bitset &a, const Bitset &b) { return b < a; }

  void set(int i) {
    if (i >= len) return;
    v[i >> 6] |= 1ULL << (i & 63);
  }
  void set(int l, int r) {
    if (l > r || r >= len) return;
    int sl = l >> 6, sr = r >> 6;
    for (; l >> 6 == sl && l <= r; ++l) v[sl] |= 1ULL << (l & 63);
    for (; r >> 6 == sr && l <= r; --r) v[sr] |= 1ULL << (r & 63);
    for (int i = sl + 1; i < sr; ++i) v[i] = W;
  }
  void set() { fill(v.begin(), v.end(), size_t(-1)), trim(); }

  void unset(int i) {
    if (i >= len) return;
    v[i >> 6] &= ~(1ULL << (i & 63));
  }
  void unset(int l, int r) {
    if (l > r || r >= len) return;
    int sl = l >> 6, sr = r >> 6;
    for (; l >> 6 == sl && l <= r; ++l) v[sl] &= ~(1ULL << (l & 63));
    for (; r >> 6 == sr && l <= r; --r) v[sr] &= ~(1ULL << (r & 63));
    for (int i = sl + 1; i < sr; ++i) v[i] = 0;
  }
  void unset() { fill(v.begin(), v.end(), 0); }

  void flip(int i) {
    if (i >= len) return;
    v[i >> 6] ^= 1ULL << (i & 63);
  }
  void flip(int l, int r) {
    if (l > r || r > len) return;
    int sl = l >> 6, sr = r >> 6;
    for (; l >> 6 == sl && l <= r; ++l) v[sl] ^= 1ULL << (l & 63);
    for (; r >> 6 == sr && l <= r; --r) v[sr] ^= 1ULL << (r & 63);
    for (int i = sl + 1; i < sr; ++i) v[i] ^= W;
  }
  void flip() {
    for (int i = 0; i < n; ++i) v[i] ^= size_t(-1);
    trim();
  }

  bool none() const {
    for (int i = 0; i < n; ++i)
      if (v[i] > 0) return false;
    return true;
  }
  bool any() const { return !none(); }
  bool all() const {
    for (int i = 0; i + 1 < n; ++i)
      if (v[i] != W) return false;
    return v[n - 1] == mask;
  }

  void swap(Bitset &a) { ::swap(n, a.n), ::swap(len, a.len), ::swap(mask, a.mask), v.swap(a.v); }

  int find0(int p) const {
    if (p >= len) return len;
    int sp = p >> 6;
    u64 msk = ~((1ULL << (p & 63)) - 1), cur = (~v[sp]) & msk;
    if (cur) return min(len, sp * 64 + __builtin_ctzll(cur));
    for (int i = sp + 1; i < n; ++i)
      if (v[i] != W) return min(len, i * 64 + __builtin_ctzll(~v[i]));
    return len;
  }
  int find1(int p) const {
    if (p >= len) return len;
    int sp = p >> 6;
    u64 msk = ~((1ULL << (p & 63)) - 1), cur = v[sp] & msk;
    if (cur) return min(len, sp * 64 + __builtin_ctzll(cur));
    for (int i = sp + 1; i < n; ++i)
      if (v[i] != W) return min(len, i * 64 + __builtin_ctzll(v[i]));
    return len;
  }

  bool operator[](const int i) const { return i >= len ? false : v[i >> 6] >> (i & 63) & 1; }

  int count() const {
    int res = 0;
    for (int i = 0; i < n; ++i) res += __builtin_popcountll(v[i]);
    return res;
  }
  int size() const { return len; }

  Bitset &operator<<=(int s) {
    if (s >= len) return unset(), *this;
    int k = s >> 6, t = s & 63;
    if (k > 0) {
      copy_backward(v.begin(), v.end() - k, v.end());
      fill(v.begin(), v.begin() + k, 0);
    }
    if (t) {
      u64 nxt = 0;
      for (int i = k; i < n; ++i) {
        u64 nx = v[i] >> (64 - t);
        v[i] = v[i] << t | nxt, nxt = nx;
      }
    }
    return trim(), *this;
  }

  Bitset &operator>>=(int s) {
    if (s >= len) return unset(), *this;
    int k = s >> 6, t = s & 63;
    if (k > 0) {
      copy(v.begin() + k, v.end(), v.begin());
      fill(v.end() - k, v.end(), 0);
    }
    if (t) {
      u64 nxt = 0;
      for (int i = n - k - 1; i >= 0; --i) {
        u64 nx = v[i] & ((1ULL << t) - 1);
        v[i] = v[i] >> t | (nxt << (64 - t)), nxt = nx;
      }
    }
    return trim(), *this;
  }

  friend Bitset operator<<(Bitset a, int s) { return a <<= s; }
  friend Bitset operator>>(Bitset a, int s) { return a >>= s; }
  friend ostream &operator<<(ostream &os, const Bitset &a) {
    for (int i = 0; i + 1 < a.n; ++i) {
      for (int j = 0; j < 64; ++j) os << (a.v[i] >> j & 1);
    }
    for (int i = 0; i < (a.len & 63); ++i) os << (a.v[a.n - 1] >> i & 1);
    return os;
  }

  vector<Bitset> split(int k) const {
    vector<Bitset> res(len / k + 1, Bitset(k));
    int c = 0, s = 0;
    while (s < len) {
      int le = min(len - s, k), p = 0, sp = s >> 6, st = s & 63, rem = le;
      while (rem > 0) {
        u64 cur = v[sp];
        if (st) cur = cur >> st | ((sp + 1 < n ? v[sp + 1] : 0) << (64 - st));
        if (rem < 64) cur &= (1ULL << rem) - 1;
        res[c].v[p++] = cur, rem -= 64, ++sp;
      }
      res[c++].trim(), s += le;
    }
    return res;
  }
};

