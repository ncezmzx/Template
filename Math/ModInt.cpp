#include <bits/stdc++.h>
using namespace std;
constexpr int md = 998244353;
template <class T>
using must_int = enable_if_t<is_integral<T>::value>*;
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
template <int id>
struct modint {
  static constexpr int mod = id;
  static constexpr unsigned umod = id;
  unsigned v;
  modint() : v(0) {}
  modint(unsigned x) : v(mont.reduce(1ull * x * mont.R2)) {}
  template <class T, must_int<T> = nullptr>
  modint(T x) : modint((unsigned)(x %= mod, x < 0 ? x + mod : x)) {}
  modint operator+() const { return *this; }
  modint operator-() const { return modint() - *this; }
  friend int raw(const modint &self) { return mont.reduce(self.v); }
  friend ostream &operator<<(ostream &os, const modint &self) {
    return os << raw(self);
  }
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
  template <class T, must_int<T> = nullptr>
  friend modint qpow(modint a, T b) {
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
/*
 * ============================================================
 * 名称：快速模数封装（Montgomery modint，固定模数）
 * 复杂度：四则运算 O(1)（每次乘法一次 Montgomery reduce），qpow O(log b)
 * 用途：模质数意义下的四则运算通用封装，替代普通 % 运算，速度更快；除法自动走逆元（要求模数为质数）。
 * 来源：all.cpp 行 5615-5678（原样保留；注释已统一移至文件尾部）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   mint a = 123456789, b = 987654321;
 *   cout << a + b << ' ' << a - b << ' ' << a * b << '\n';
 *   cout << qpow(a, 10) << '\n';
 *   cout << a / b << '\n';  // 除法走逆元（模质数）
 * }
 * ============================================================
 */
