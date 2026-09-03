#include <bits/stdc++.h>
using namespace std;
constexpr int md = 998244353;
template <class T> using must_int = enable_if_t<is_integral<T>::value> *;
struct Montgomery {
  typedef unsigned u32;
  typedef unsigned long long u64;
  u32 umod, niv, R, R2;
  Montgomery() {}
  Montgomery(u32 m) : umod(m), niv(2 + m), R(-m % m), R2(-(u64)m % m) {
    for (int i = 0; i < 4; i++) niv *= 2 + m * niv;
  }
  u32 reduce(u64 x) {
    x = (x + (u64)((u32)x * niv) * umod) >> 32;
    return x >= umod ? x - umod : x;
  }
};
Montgomery mont(md);
template <int id> struct modint {
  static constexpr int mod = id;
  static constexpr unsigned umod = id;
  unsigned v;
  modint() : v(0) {}
  modint(unsigned x) : v(mont.reduce(1ull * x * mont.R2)) {}
  template <class T, must_int<T> = nullptr> modint(T x) : modint((unsigned)(x %= mod, x < 0 ? x + mod : x)) {}
  modint operator+() const { return *this; }
  modint operator-() const { return modint() - *this; }
  friend int raw(const modint &self) { return mont.reduce(self.v); }
  friend ostream &operator<<(ostream &os, const modint &self) { return os << raw(self); }
  modint &operator+=(const modint &rhs) {
    v += rhs.v;
    if (v >= umod) v -= umod;
    return *this;
  }
  modint &operator-=(const modint &rhs) {
    v -= rhs.v;
    if (v >= umod) v += umod;
    return *this;
  }
  modint &operator*=(const modint &rhs) {
    v = mont.reduce(1ull * v * rhs.v);
    return *this;
  }
  modint &operator/=(const modint &rhs) {
    assert(rhs.v);
    return *this *= qpow(rhs, mod - 2);
  }
  template <class T, must_int<T> = nullptr> friend modint qpow(modint a, T b) {
    modint r = 1;
    for (; b; b >>= 1, a *= a)
      if (b & 1) r *= a;
    return r;
  }
  friend modint operator+(modint lhs, const modint &rhs) { return lhs += rhs; }
  friend modint operator-(modint lhs, const modint &rhs) { return lhs -= rhs; }
  friend modint operator*(modint lhs, const modint &rhs) { return lhs *= rhs; }
  friend modint operator/(modint lhs, const modint &rhs) { return lhs /= rhs; }
  bool operator==(const modint &rhs) const { return v == rhs.v; }
  bool operator!=(const modint &rhs) const { return v != rhs.v; }
};
using mint = modint<md>;

