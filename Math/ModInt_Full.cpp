#include <bits/stdc++.h>
using namespace std;

template <uint32_t m> struct modint {
  using u32 = uint32_t;
  using u64 = uint64_t;
  using u128 = __uint128_t;
  static constexpr u32 mod = m;

  // C++14 has no constexpr lambdas; use constexpr member functions instead
  static constexpr u32 calc_im() {
    u32 x = 1;
    for (int i = 0; i < 5; ++i) x *= 2 - mod * x;
    return u32(0) - x;
  }
  static constexpr u32 calc_r2() {
    u64 x = (u64(-1) % mod + 1) % mod;
    return u32(x);
  }
  static constexpr u32 im = calc_im();
  static constexpr u32 r2 = calc_r2();

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
    // Fermat for prime MOD (extended gcd would be needed otherwise)
    return pow(mod - 2);
  }
  modint sqrt() const {
    if (*this == 0) return 0;
    if (pow((mod - 1) >> 1) != 1) return 0;      // not a quadratic residue
    if (mod % 4 == 3) return pow((mod + 1) >> 2);
    u32 q = mod - 1, s = 0;
    while (!(q & 1)) q >>= 1, ++s;
    modint z = 2;
    while (z.pow((mod - 1) >> 1) == 1) z += 1;   // find a non-residue z
    modint c = z.pow(q), x = pow((q + 1) >> 1), t = pow(q);
    for (u32 mm = s; mm > 1;) {
      modint tt = t;
      u32 i = 0;
      while (tt != 1) tt *= tt, ++i;             // least i with t^(2^i) == 1
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
 * ============================================================
 * Name: full modint (Montgomery reduction, fixed modulus)
 * Complexity: arithmetic O(1) (multiplication is Montgomery reduction, no
 *             division/modulo instructions); pow O(log b)
 * Usage: complete integer arithmetic modulo a prime: + - * / (modular
 *        inverse) % unary minus, increment/decrement, comparisons, stream
 *        IO, powers, inverses, modular square roots (Tonelli-Shanks)
 * Principle: values stored in Montgomery form (value * R mod m,
 *        R = 2^32); multiplication uses REDC (__int128 exact, no overflow);
 *        im = -m^{-1} mod 2^32 (compile-time Newton iteration),
 *        r2 = R^2 mod m
 * Notes: MOD must be an odd prime (inv via Fermat, sqrt via Tonelli-Shanks);
 *        inv(0) = 0, dividing by zero is undefined; operator% has integer
 *        semantics (modulo val()); compared with the Barrett version
 *        (DynamicModInt.cpp): this fixed-modulus version is faster (no
 *        runtime reduction object)
 * ============================================================
 * Example (uncomment to compile):

 * using mint = modint<998244353>;
 * signed main() {
 *   mint a, b;
 *   cin >> a >> b;
 *   cout << a + b << ' ' << a * b << ' ' << a / b << ' ' << a.pow(10) << '\n';
 *   cout << a.inv() * a << '\n';          // 1
 *   cout << mint(2).sqrt() << '\n';       // sqrt(2) modulo 998244353
 *   mint x = 5;
 *   cout << ++x << ' ' << x-- << '\n';    // 6 6
 * }
 * ============================================================
 */
