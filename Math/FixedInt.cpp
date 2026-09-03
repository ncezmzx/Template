
template <int L> struct uintN {
  using u64 = uint64_t;
  using u128 = __uint128_t;
  static constexpr int LIMBS = L;
  static constexpr int BITS = 64 * L;
  static_assert(L >= 2, "uintN needs at least two 64-bit limbs");
  array<u64, L> v;

  uintN() { v.fill(0); }
  uintN(const char *s) : uintN(string(s)) {}
  explicit uintN(const string &s) { *this = from_string(s); }

  uintN(__int128 x) {
    v.fill(0);
    v[0] = (u64)x;
    v[1] = (u64)((u128)x >> 64);
    if (x < 0)
      for (int i = 2; i < L; ++i) v[i] = ~0ull;
  }
  uintN(unsigned __int128 x) {
    v.fill(0);
    v[0] = (u64)x;
    v[1] = (u64)((u128)x >> 64);
  }

  template <typename T>
  static void assign_limb(u64 *vv, T x, true_type) {
    if (x < 0) {
      u64 mag = (u64)(0ull - (u64)x);
      vv[0] = ~mag + 1;
      for (int i = 1; i < L; ++i) vv[i] = ~0ull;
    }
    else vv[0] = (u64)x;
  }
  template <typename T>
  static void assign_limb(u64 *vv, T x, false_type) {
    vv[0] = (u64)x;
  }
  template <typename T, typename = enable_if_t<is_integral<T>::value>>
  uintN(T x) {
    v.fill(0);
    assign_limb(v.data(), x, is_signed<T>());
  }

  static uintN from_string(const string &s) {
    bool neg = false;
    int i = 0;
    if (i < (int)s.size() && (s[i] == '-' || s[i] == '+')) neg = s[i++] == '-';
    uintN r;
    static constexpr u64 pw10[10] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
    while (i < (int)s.size()) {
      int take = min(9, (int)s.size() - i);
      u64 val = 0;
      for (int t = 0; t < take; ++t) val = val * 10 + (u64)(s[i + t] - '0');
      i += take;
      r = r * pw10[take] + val;
    }
    if (neg && !r.is_zero()) r = -r;
    return r;
  }

  bool is_zero() const {
    for (int i = 0; i < L; ++i)
      if (v[i]) return false;
    return true;
  }
  bool is_odd() const { return v[0] & 1; }
  static int cmp(const uintN &a, const uintN &b) {
    for (int i = L - 1; i >= 0; --i)
      if (a.v[i] != b.v[i]) return a.v[i] < b.v[i] ? -1 : 1;
    return 0;
  }
  int ctz() const {
    for (int i = 0; i < L; ++i)
      if (v[i]) return i * 64 + __builtin_ctzll(v[i]);
    return BITS;
  }
  int clz() const {
    for (int i = L - 1; i >= 0; --i)
      if (v[i]) return (L - 1 - i) * 64 + __builtin_clzll(v[i]);
    return BITS;
  }
  int bitlen() const { return BITS - clz(); }
  unsigned long long to_ull() const { return v[0]; }
  bool fits_ull() const {
    for (int i = 1; i < L; ++i)
      if (v[i]) return false;
    return true;
  }
  u128 to_u128() const {
    for (int i = 2; i < L; ++i)
      if (v[i]) throw overflow_error("uintN::to_u128: out of range");
    return ((u128)v[1] << 64) | v[0];
  }
  long long to_ll() const {
    if (cmp(*this, uintN((unsigned long long)LLONG_MAX)) > 0) throw overflow_error("uintN::to_ll: out of range");
    return (long long)v[0];
  }
  explicit operator bool() const { return !is_zero(); }
  explicit operator unsigned long long() const { return v[0]; }
  explicit operator long long() const { return to_ll(); }
  explicit operator u128() const { return to_u128(); }

  uintN operator+() const { return *this; }
  uintN operator-() const { return ~*this + 1; }
  uintN operator+(const uintN &o) const {
    uintN r;
    u64 carry = 0;
#pragma GCC unroll 8
    for (int i = 0; i < L; ++i) {
      u64 s = v[i] + o.v[i];
      u64 c2 = s < v[i];
      s += carry;
      c2 |= s < carry;
      r.v[i] = s;
      carry = c2;
    }
    return r;
  }
  uintN operator-(const uintN &o) const {
    uintN r;
    u64 borrow = 0;
#pragma GCC unroll 8
    for (int i = 0; i < L; ++i) {
      u64 d1 = v[i] - o.v[i];
      u64 b1 = v[i] < o.v[i];
      u64 d = d1 - borrow;
      u64 b2 = d1 < borrow;
      r.v[i] = d;
      borrow = b1 | b2;
    }
    return r;
  }
  uintN operator*(const uintN &o) const {
    uintN r;
#pragma GCC unroll 8
    for (int i = 0; i < L; ++i) {
      u64 carry = 0;
#pragma GCC unroll 8
      for (int j = 0; i + j < L; ++j) {
        u128 cur = (u128)v[i] * o.v[j] + r.v[i + j] + carry;
        r.v[i + j] = (u64)cur;
        carry = (u64)(cur >> 64);
      }
    }
    return r;
  }

  static pair<uintN, uintN> divmod(const uintN &x, const uintN &y) {
    int n = L;
    while (n > 0 && y.v[n - 1] == 0) --n;
    int ul = L;
    while (ul > 0 && x.v[ul - 1] == 0) --ul;
    if (ul < n) return {uintN(), x};
    int m = ul - n;

    if (n == 1) {
      uintN q;
      u64 rem = 0, dv = y.v[0];
      for (int i = ul - 1; i >= 0; --i) {
        u128 cur = ((u128)rem << 64) | x.v[i];
        q.v[i] = (u64)(cur / dv);
        rem = (u64)(cur % dv);
      }
      uintN rr;
      rr.v[0] = rem;
      return {q, rr};
    }

    array<u64, L + 1> u{};
    array<u64, L> v{};
    array<u64, L> q{};
    for (int i = 0; i < ul; ++i) u[i] = x.v[i];
    for (int i = 0; i < n; ++i) v[i] = y.v[i];

    u64 d = (u64)(((u128)1 << 64) / ((u128)v[n - 1] + 1));
    {
      u128 carry = 0;
      for (int i = 0; i < m + n; ++i) {
        u128 cur = (u128)u[i] * d + carry;
        u[i] = (u64)cur;
        carry = cur >> 64;
      }
      u[m + n] = (u64)carry;
      carry = 0;
      for (int i = 0; i < n; ++i) {
        u128 cur = (u128)v[i] * d + carry;
        v[i] = (u64)cur;
        carry = cur >> 64;
      }
    }
    for (int j = m; j >= 0; --j) {
      u128 num = ((u128)u[j + n] << 64) | u[j + n - 1];
      u128 qhat = num / v[n - 1];
      u128 rhat = num % v[n - 1];
      while (qhat >= ((u128)1 << 64) || qhat * v[n - 2] > ((rhat << 64) + u[j + n - 2])) {
        --qhat;
        rhat += v[n - 1];
        if (rhat >= ((u128)1 << 64)) break;
      }
      u128 borrow = 0;
#pragma GCC unroll 8
      for (int i = 0; i < n; ++i) {
        u128 p = qhat * v[i] + borrow;
        u64 cur = u[j + i], pl = (u64)p;
        bool under = cur < pl;
        u[j + i] = cur - pl;
        borrow = (p >> 64) + (under ? 1 : 0);
      }
      if (borrow > u[j + n]) {
        --qhat;
        u64 carry = 0;
#pragma GCC unroll 8
        for (int i = 0; i < n; ++i) {
          u128 sum = (u128)u[j + i] + v[i] + carry;
          u[j + i] = (u64)sum;
          carry = (u64)(sum >> 64);
        }
        u[j + n] = (u64)(u[j + n] + carry);
      }
      else {
        u[j + n] = (u64)(u[j + n] - borrow);
      }
      q[j] = (u64)qhat;
    }
    uintN Q, R;
    for (int i = 0; i <= m; ++i) Q.v[i] = q[i];
    u128 rem = 0;
    for (int i = n - 1; i >= 0; --i) {
      u128 cur = ((u128)rem << 64) | u[i];
      R.v[i] = (u64)(cur / d);
      rem = cur % d;
    }
    return {Q, R};
  }
  uintN operator/(const uintN &o) const { return divmod(*this, o).first; }
  uintN operator%(const uintN &o) const { return divmod(*this, o).second; }

  uintN operator&(const uintN &o) const {
    uintN r;
#pragma GCC unroll 8
    for (int i = 0; i < L; ++i) r.v[i] = v[i] & o.v[i];
    return r;
  }
  uintN operator|(const uintN &o) const {
    uintN r;
#pragma GCC unroll 8
    for (int i = 0; i < L; ++i) r.v[i] = v[i] | o.v[i];
    return r;
  }
  uintN operator^(const uintN &o) const {
    uintN r;
#pragma GCC unroll 8
    for (int i = 0; i < L; ++i) r.v[i] = v[i] ^ o.v[i];
    return r;
  }
  uintN operator~() const {
    uintN r;
#pragma GCC unroll 8
    for (int i = 0; i < L; ++i) r.v[i] = ~v[i];
    return r;
  }
  bool operator!() const { return is_zero(); }

  uintN operator<<(int k) const {
    if (k < 0) return *this >> -k;
    if (k == 0) return *this;
    if (k >= BITS) return uintN();
    int ls = k >> 6, bs = k & 63;
    uintN r;
    if (bs == 0) {
#pragma GCC unroll 8
      for (int i = 0; i + ls < L; ++i) r.v[i + ls] = v[i];
    }
    else {
#pragma GCC unroll 8
      for (int i = 0; i < L; ++i) {
        if (i + ls < L) r.v[i + ls] |= v[i] << bs;
        if (i + ls + 1 < L) r.v[i + ls + 1] |= v[i] >> (64 - bs);
      }
    }
    return r;
  }
  uintN operator>>(int k) const {
    if (k < 0) return *this << -k;
    if (k == 0) return *this;
    if (k >= BITS) return uintN();
    int ls = k >> 6, bs = k & 63;
    uintN r;
    if (bs == 0) {
#pragma GCC unroll 8
      for (int i = ls; i < L; ++i) r.v[i - ls] = v[i];
    }
    else {
#pragma GCC unroll 8
      for (int i = 0; i + ls < L; ++i) {
        r.v[i] = v[i + ls] >> bs;
        if (i + ls + 1 < L) r.v[i] |= v[i + ls + 1] << (64 - bs);
      }
    }
    return r;
  }

  bool operator==(const uintN &o) const { return v == o.v; }
  bool operator!=(const uintN &o) const { return !(*this == o); }
  bool operator<(const uintN &o) const { return cmp(*this, o) < 0; }
  bool operator>(const uintN &o) const { return cmp(*this, o) > 0; }
  bool operator<=(const uintN &o) const { return cmp(*this, o) <= 0; }
  bool operator>=(const uintN &o) const { return cmp(*this, o) >= 0; }

  uintN &operator+=(const uintN &o) { return *this = *this + o; }
  uintN &operator-=(const uintN &o) { return *this = *this - o; }
  uintN &operator*=(const uintN &o) { return *this = *this * o; }
  uintN &operator/=(const uintN &o) { return *this = *this / o; }
  uintN &operator%=(const uintN &o) { return *this = *this % o; }
  uintN &operator&=(const uintN &o) { return *this = *this & o; }
  uintN &operator|=(const uintN &o) { return *this = *this | o; }
  uintN &operator^=(const uintN &o) { return *this = *this ^ o; }
  uintN &operator<<=(int k) { return *this = *this << k; }
  uintN &operator>>=(int k) { return *this = *this >> k; }
  uintN &operator++() { return *this += 1; }
  uintN &operator--() { return *this -= 1; }
  uintN operator++(int) {
    uintN t = *this;
    ++*this;
    return t;
  }
  uintN operator--(int) {
    uintN t = *this;
    --*this;
    return t;
  }

  uintN pow(unsigned long long e) const {
    uintN r(1), b = *this;
    while (e) {
      if (e & 1) r = r * b;
      b = b * b;
      e >>= 1;
    }
    return r;
  }
  friend uintN gcd(uintN a, uintN b) {
    while (!b.is_zero()) {
      uintN t = a % b;
      a = b, b = t;
    }
    return a;
  }

  uintN divmod_small(u64 d, u64 &rem) const {
    uintN q;
    u128 r = 0;
#pragma GCC unroll 8
    for (int i = L - 1; i >= 0; --i) {
      u128 cur = (r << 64) | v[i];
      q.v[i] = (u64)(cur / d);
      r = cur % d;
    }
    rem = (u64)r;
    return q;
  }
  string to_string() const {
    if (is_zero()) return "0";
    uintN t = *this;
    static constexpr u64 B = 1000000000ull;
    static constexpr int MAXC = (BITS + 29) / 30 + 2;
    array<string, MAXC> chunk;
    int cnt = 0;
    char buf[16];
    while (!t.is_zero()) {
      u64 rem;
      t = t.divmod_small(B, rem);
      snprintf(buf, sizeof buf, "%09llu", (unsigned long long)rem);
      chunk[cnt++] = buf;
    }
    string res;
    for (int i = cnt - 1; i >= 0; --i) {
      if (i == cnt - 1) {
        size_t k = 0;
        while (k + 1 < chunk[i].size() && chunk[i][k] == '0') ++k;
        res += chunk[i].substr(k);
      }
      else {
        res += chunk[i];
      }
    }
    return res;
  }
  friend ostream &operator<<(ostream &os, const uintN &x) { return os << x.to_string(); }
  friend istream &operator>>(istream &is, uintN &x) {
    string s;
    is >> s;
    x = uintN(s);
    return is;
  }
};

template <int L> struct intN {
  using u64 = uint64_t;
  using u128 = __uint128_t;
  static constexpr int LIMBS = L;
  static constexpr int BITS = 64 * L;
  uintN<L> u;

  intN() : u() {}
  intN(const char *s) : intN(string(s)) {}
  explicit intN(const string &s) {
    if (!s.empty() && s[0] == '-') {
      u = uintN<L>(s.substr(1));
      u = ~u + 1;
    }
    else {
      u = uintN<L>(s);
    }
  }
  template <typename T, typename = enable_if_t<is_integral<T>::value>>
  intN(T x) : u(x) {}
  intN(__int128 x) : u(x) {}
  intN(unsigned __int128 x) : u(x) {}
  static intN from_bits(const uintN<L> &bits) {
    intN r;
    r.u = bits;
    return r;
  }

  bool is_neg() const { return (u.v[L - 1] >> 63) & 1; }
  bool is_zero() const { return u.is_zero(); }
  int sign() const { return is_zero() ? 0 : (is_neg() ? -1 : 1); }
  uintN<L> abs_mag() const { return is_neg() ? (~u + 1) : u; }
  intN abs() const { return from_bits(abs_mag()); }

  long long to_ll() const {
    if (is_neg()) {
      uintN<L> m = abs_mag();
      if (uintN<L>::cmp(m, uintN<L>(1ull << 63)) > 0) throw overflow_error("intN::to_ll: out of range");
      return (long long)(0ull - m.v[0]);
    }
    else {
      if (uintN<L>::cmp(u, uintN<L>((unsigned long long)LLONG_MAX)) > 0)
        throw overflow_error("intN::to_ll: out of range");
      return (long long)u.v[0];
    }
  }
  int to_int() const {
    if (is_neg()) {
      uintN<L> m = abs_mag();
      if (uintN<L>::cmp(m, uintN<L>(1u << 31)) > 0) throw overflow_error("intN::to_int: out of range");
      return (int)(0u - (unsigned)m.v[0]);
    }
    else {
      if (uintN<L>::cmp(u, uintN<L>((unsigned)INT_MAX)) > 0) throw overflow_error("intN::to_int: out of range");
      return (int)u.v[0];
    }
  }
  explicit operator bool() const { return !is_zero(); }
  explicit operator long long() const { return to_ll(); }
  explicit operator int() const { return to_int(); }

  intN operator+() const { return *this; }
  intN operator-() const { return from_bits(~u + 1); }
  intN operator+(const intN &o) const { return from_bits(u + o.u); }
  intN operator-(const intN &o) const { return from_bits(u - o.u); }
  intN operator*(const intN &o) const { return from_bits(u * o.u); }
  pair<intN, intN> divmod(const intN &o) const {
    bool qneg = is_neg() ^ o.is_neg();
    pair<uintN<L>, uintN<L>> qr = uintN<L>::divmod(abs_mag(), o.abs_mag());
    return {from_bits(qneg ? (~qr.first + 1) : qr.first),
            from_bits(is_neg() ? (~qr.second + 1) : qr.second)};
  }
  intN operator/(const intN &o) const { return divmod(o).first; }
  intN operator%(const intN &o) const { return divmod(o).second; }

  intN operator&(const intN &o) const { return from_bits(u & o.u); }
  intN operator|(const intN &o) const { return from_bits(u | o.u); }
  intN operator^(const intN &o) const { return from_bits(u ^ o.u); }
  intN operator~() const { return from_bits(~u); }
  bool operator!() const { return is_zero(); }

  intN operator<<(int k) const {
    if (k < 0) return *this >> -k;
    return from_bits(u << k);
  }
  intN operator>>(int k) const {
    if (k < 0) return *this << -k;
    if (k == 0) return *this;
    if (k >= BITS) return is_neg() ? intN(-1) : intN(0);
    uintN<L> r = u >> k;
    if (is_neg()) r = r | ~((uintN<L>(1) << (BITS - k)) - 1);
    return from_bits(r);
  }

  bool operator<(const intN &o) const {
    bool an = is_neg(), bn = o.is_neg();
    if (an != bn) return an;
    return uintN<L>::cmp(u, o.u) < 0;
  }
  bool operator>(const intN &o) const { return o < *this; }
  bool operator<=(const intN &o) const { return !(o < *this); }
  bool operator>=(const intN &o) const { return !(*this < o); }
  bool operator==(const intN &o) const { return u == o.u; }
  bool operator!=(const intN &o) const { return !(*this == o); }

  intN &operator+=(const intN &o) { return *this = *this + o; }
  intN &operator-=(const intN &o) { return *this = *this - o; }
  intN &operator*=(const intN &o) { return *this = *this * o; }
  intN &operator/=(const intN &o) { return *this = *this / o; }
  intN &operator%=(const intN &o) { return *this = *this % o; }
  intN &operator&=(const intN &o) { return *this = *this & o; }
  intN &operator|=(const intN &o) { return *this = *this | o; }
  intN &operator^=(const intN &o) { return *this = *this ^ o; }
  intN &operator<<=(int k) { return *this = *this << k; }
  intN &operator>>=(int k) { return *this = *this >> k; }
  intN &operator++() { return *this += 1; }
  intN &operator--() { return *this -= 1; }
  intN operator++(int) {
    intN t = *this;
    ++*this;
    return t;
  }
  intN operator--(int) {
    intN t = *this;
    --*this;
    return t;
  }

  intN pow(unsigned long long e) const {
    intN r(1), b = *this;
    while (e) {
      if (e & 1) r = r * b;
      b = b * b;
      e >>= 1;
    }
    return r;
  }
  friend intN gcd(intN a, intN b) { return intN::from_bits(gcd(a.abs_mag(), b.abs_mag())); }

  string to_string() const {
    if (is_neg()) return "-" + abs_mag().to_string();
    return u.to_string();
  }
  friend ostream &operator<<(ostream &os, const intN &x) { return os << x.to_string(); }
  friend istream &operator>>(istream &is, intN &x) {
    string s;
    is >> s;
    x = intN(s);
    return is;
  }
};

using uint256 = uintN<4>;
using int256 = intN<4>;
using uint512 = uintN<8>;
using int512 = intN<8>;

