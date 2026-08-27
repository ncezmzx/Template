#include <bits/stdc++.h>
using namespace std;

template <uint32_t m> struct modint {
  using u32 = uint32_t;
  using u64 = uint64_t;
  using u128 = __uint128_t;
  static constexpr u32 mod = m;

  static constexpr u32 im = [] {
    u32 x = 1;
    for (int i = 0; i < 5; ++i) x *= 2 - mod * x;
    return u32(0) - x;
  }();
  static constexpr u32 r2 = [] {
    u64 x = (u64(-1) % mod + 1) % mod;
    return u32(x);
  }();

  u32 v;

  static u32 reduce(u64 z) {
    u32 t = u32(z) * im;
    u32 y = u32(((u128)z + (u128)t * mod) >> 32);
    return y >= mod ? y - mod : y;
  }
  static u32 to_mont(u32 x) { return reduce((u64)x * r2); }
  static u32 from_mont(u32 x) { return reduce(x); }

  modint() : v(0) {}
  template <class T> modint(T x) {
    long long t = (long long)(x % (long long)mod);
    v = to_mont(u32(t < 0 ? t + mod : t));
  }
  u32 val() const { return from_mont(v); }
  static u32 modval() { return mod; }

  modint& operator+=(const modint& o) {
    u64 z = (u64)v + o.v;
    if (z >= mod) z -= mod;
    v = u32(z);
    return *this;
  }
  modint& operator-=(const modint& o) {
    u32 x = v, y = o.v;
    v = x >= y ? x - y : x + mod - y;
    return *this;
  }
  modint& operator*=(const modint& o) {
    v = reduce((u64)v * o.v);
    return *this;
  }
  modint& operator/=(const modint& o) { return *this *= o.inv(); }
  modint& operator%=(const modint& o) {
    v = to_mont(u32(val() % o.val()));
    return *this;
  }
  modint& operator++() { return *this += 1; }
  modint& operator--() { return *this -= 1; }
  modint operator++(int) {
    modint t = *this;
    ++*this;
    return t;
  }
  modint operator--(int) {
    modint t = *this;
    --*this;
    return t;
  }
  modint operator-() const {
    modint r = *this;
    r.v = r.v ? mod - r.v : 0;
    return r;
  }

  friend modint operator+(modint a, const modint& b) { return a += b; }
  friend modint operator-(modint a, const modint& b) { return a -= b; }
  friend modint operator*(modint a, const modint& b) { return a *= b; }
  friend modint operator/(modint a, const modint& b) { return a /= b; }
  friend modint operator%(modint a, const modint& b) { return a %= b; }

  bool operator==(const modint& o) const { return v == o.v; }
  bool operator!=(const modint& o) const { return v != o.v; }
  bool operator<(const modint& o) const { return val() < o.val(); }
  bool operator>(const modint& o) const { return o < *this; }
  bool operator<=(const modint& o) const { return !(o < *this); }
  bool operator>=(const modint& o) const { return !(*this < o); }

  modint pow(long long b) const {
    modint r = 1, x = *this;
    for (; b; b >>= 1, x *= x)
      if (b & 1) r *= x;
    return r;
  }
  modint inv() const {
    // MOD 为素数时费马小定理；否则需扩展欧几里得（此处按素数处理）
    return pow(mod - 2);
  }
  modint sqrt() const {
    if (*this == 0) return 0;
    if (pow((mod - 1) >> 1) != 1) return 0;      // 非二次剩余
    if (mod % 4 == 3) return pow((mod + 1) >> 2);
    u32 q = mod - 1, s = 0;
    while (!(q & 1)) q >>= 1, ++s;
    modint z = 2;
    while (z.pow((mod - 1) >> 1) == 1) z += 1;   // 找非二次剩余
    modint c = z.pow(q), x = pow((q + 1) >> 1), t = pow(q);
    for (u32 mm = s; mm > 1;) {
      modint tt = t;
      u32 i = 0;
      while (tt != 1) tt *= tt, ++i;             // 最小 i：t^(2^i) == 1
      modint b = c;
      for (u32 j = 0; j < mm - i - 1; ++j) b *= b;  // b = c^(2^(mm-i-1))
      x *= b, t *= b * b, c = b * b, mm = i;
    }
    return x;
  }

  friend ostream& operator<<(ostream& os, const modint& x) { return os << x.val(); }
  friend istream& operator>>(istream& is, modint& x) {
    long long t;
    is >> t;
    x = modint(t);
    return is;
  }
};

/*
 * ============================================================
 * 名称：完整 modint（蒙哥马利约简，固定模数）
 * 复杂度：四则运算 O(1)（乘法为蒙哥马利约简，无除法/取模指令），pow O(log b)
 * 用途：模素数意义下的完整整数运算：+ - * /（模逆）% 一元负 自增自减
 *       比较 输入输出 幂 逆元 二次剩余开方（Tonelli-Shanks）
 * 原理：内部以蒙哥马利形式存储（值 * R mod m，R = 2^32），乘法用 REDC
 *       约简（__int128 精确计算，避免溢出）；im 为 -m^{-1} mod 2^32
 *       （编译期牛顿迭代），r2 = R^2 mod m
 * 注意：MOD 需为奇素数（inv 用费马小定理，sqrt 用 Tonelli-Shanks）；
 *       除数为 0 时 inv(0) = 0，结果未定义；
 *       operator% 为整数语义（对 val() 取模）；与 Barrett 版
 *       （DynamicModInt.cpp）相比：本版固定模数更快（无运行期约简对象）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * using mint = modint<998244353>;
 * signed main() {
 *   mint a, b;
 *   cin >> a >> b;
 *   cout << a + b << ' ' << a * b << ' ' << a / b << ' ' << a.pow(10) << '\n';
 *   cout << a.inv() * a << '\n';          // 1
 *   cout << mint(2).sqrt() << '\n';       // 模 998244353 下 sqrt(2)
 *   mint x = 5;
 *   cout << ++x << ' ' << x-- << '\n';    // 6 6
 * }
 * ============================================================
 */
