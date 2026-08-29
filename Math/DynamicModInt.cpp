#include <bits/stdc++.h>
using namespace std;
constexpr int md = 1e9 + 9;
namespace {
using u32 = unsigned;
using i64 = long long;
using u64 = unsigned long long;
struct Barrett {
  u32 _m;
  u64 im;
  Barrett(u32 m) : _m(m), im(u64(-1) / m + 1) {}
  u32 umod() { return _m; }
  u32 mul(u32 a, u32 b) const {
    u64 z = (u64) a * b;
    u64 x = (u64)(((__uint128_t) (z) * im) >> 64) * _m;
    return (u32) (z - x + (z < x ? _m : 0));
  }
};
struct dynamic_modint {
  using mint = dynamic_modint;
  unsigned int _v;
  static Barrett bt;
  dynamic_modint() : _v(0) {}
  template < class T > dynamic_modint(T v) {
    T x = v % mod();
    if (x < 0) x += mod();
    _v = (u32) x;
  }
  u32 val() const { return _v; }
  static int mod() { return (int) bt.umod(); }
  static void set_mod(int m) {
    assert(m >= 1);
    bt = Barrett(m);
  }
  mint& operator+=(const mint &rhs) {
    _v += rhs._v;
    if (_v >= mod()) _v -= mod();
    return *this;
  }
  mint& operator-=(const mint &rhs) {
    _v += mod() - rhs._v;
    if (_v >= mod()) _v -= mod();
    return *this;
  }
  mint& operator*=(const mint &rhs) {
    _v = bt.mul(_v, rhs._v);
    return *this;
  }
  friend mint operator-(mint lhs, const mint &rhs) { return lhs -= rhs; }
  friend mint operator*(mint lhs, const mint &rhs) { return lhs *= rhs; }
  friend mint operator+(mint lhs, const mint &rhs) { return lhs += rhs; }
  friend mint raw(const int th) { return dynamic_modint(th); }
  friend ostream& operator<<(ostream &os, const mint &lhs) {
    os << lhs.val();
    return os;
  }
};
Barrett dynamic_modint :: bt(md);
using mint = dynamic_modint;
mint qpow(mint a, int b = md - 2) {
  mint r = 1;
  for (; b; b >>= 1, a = a * a)
    if (b & 1) r = r * a;
  return r;
}
}
/*
 * ============================================================
 * Name: dynamic modulus (Barrett reduction modint, runtime modulus)
 * Complexity: arithmetic O(1) (Barrett-reduced multiply); qpow O(log b)
 * Usage: modular arithmetic when the modulus is only known at runtime (e.g.
 *        read from input); set_mod switches the modulus on the fly.
 * Source: all.cpp lines 9559-9623 (kept verbatim, comments translated)
 * Notes: the templated constructor requires integer arguments: do not write
 *        mint(1e9 + 9), because 1e9+9 is a double and fails to compile
 * ============================================================
 */
