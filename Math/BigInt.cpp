#include <bits/stdc++.h>
using namespace std;

struct bigint {
  using u32 = uint32_t;
  using u64 = uint64_t;
  bool neg;
  vector<u32> a;

  bigint() : neg(false), a(1, 0) {}
  bigint(long long x) {
    neg = x < 0;
    unsigned long long t = neg ? ~(unsigned long long)x + 1ull : (unsigned long long)x;
    a.push_back(u32(t));
    if (t >> 32) a.push_back(u32(t >> 32));
    trim();
  }

  void mul_add_small(int k, u64 v) {
    static constexpr u64 pw10[10] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
    u64 m = pw10[k], carry = v;
    for (int i = 0; i < (int)a.size(); ++i) {
      u64 cur = (u64)a[i] * m + carry;
      a[i] = u32(cur), carry = cur >> 32;
    }
    while (carry) a.push_back(u32(carry)), carry >>= 32;
    trim();
  }
  explicit bigint(const string &s) {
    int i = 0;
    bool n = false;
    if (i < (int)s.size() && (s[i] == '-' || s[i] == '+')) n = s[i++] == '-';
    a.assign(1, 0);
    neg = false;

    while (i < (int)s.size()) {
      int take = min(9, (int)s.size() - i);
      u64 v = 0;
      for (int t = 0; t < take; ++t) v = v * 10 + (u64)(s[i + t] - '0');
      i += take;
      mul_add_small(take, v);
    }
    if (n && !is_zero()) neg = true;
  }

  static void trim(vector<u32> &v) {
    while (v.size() > 1 && v.back() == 0) v.pop_back();
  }
  void trim() { ::bigint::trim(a); }
  bool is_zero() const { return a.size() == 1 && a[0] == 0; }
  int sign() const { return is_zero() ? 0 : (neg ? -1 : 1); }
  bigint abs() const {
    bigint r = *this;
    r.neg = false;
    return r;
  }

  int bitlen() const {
    if (is_zero()) return 0;
    return (int)(a.size() - 1) * 32 + 32 - __builtin_clz(a.back());
  }

  long long to_ll() const {
    if (*this > (bigint)LLONG_MAX) throw overflow_error("bigint::to_ll: value out of range");
    if (*this < (bigint)LLONG_MIN) throw overflow_error("bigint::to_ll: value out of range");
    unsigned long long v = 0;
    for (int i = (int)a.size() - 1; i >= 0; --i) v = (v << 32) | a[i];
    return neg ? (long long)(0ull - v) : (long long)v;
  }
  explicit operator bool() const { return !is_zero(); }
  explicit operator long long() const { return to_ll(); }
  explicit operator int() const { return (int)to_ll(); }


  static int cmp_vec(const vector<u32> &x, const vector<u32> &y) {
    if (x.size() != y.size()) return x.size() < y.size() ? -1 : 1;
    for (int i = (int)x.size() - 1; i >= 0; --i)
      if (x[i] != y[i]) return x[i] < y[i] ? -1 : 1;
    return 0;
  }
  static int cmp_abs(const bigint &x, const bigint &y) { return cmp_vec(x.a, y.a); }

  static vector<u32> add_abs(const vector<u32> &x, const vector<u32> &y) {
    vector<u32> r(max(x.size(), y.size()) + 1, 0);
    u64 carry = 0;
    for (int i = 0; i < (int)r.size(); ++i) {
      u64 cur = carry + (i < (int)x.size() ? x[i] : 0) + (i < (int)y.size() ? y[i] : 0);
      r[i] = u32(cur);
      carry = cur >> 32;
    }
    trim(r);
    return r;
  }
  static vector<u32> sub_abs(const vector<u32> &x, const vector<u32> &y) {
    vector<u32> r(x);
    u64 borrow = 0;
    for (int i = 0; i < (int)r.size(); ++i) {
      u64 cur = (u64)r[i] - borrow - (i < (int)y.size() ? y[i] : 0);
      borrow = (cur >> 63) & 1;
      r[i] = u32(cur);
    }
    trim(r);
    return r;
  }
  static vector<u32> mul_abs(const vector<u32> &x, const vector<u32> &y) {
    if (x.size() == 1 && x[0] == 0) return vector<u32>(1, 0);
    if (y.size() == 1 && y[0] == 0) return vector<u32>(1, 0);
    vector<u32> r(x.size() + y.size(), 0);
    for (int i = 0; i < (int)x.size(); ++i) {
      u64 carry = 0;
      for (int j = 0; j < (int)y.size(); ++j) {
        u64 cur = (u64)x[i] * y[j] + r[i + j] + carry;
        r[i + j] = u32(cur);
        carry = cur >> 32;
      }
      r[i + y.size()] = u32(carry);
    }
    trim(r);
    return r;
  }

  static pair<vector<u32>, vector<u32>> divmod_abs(const vector<u32> &U, const vector<u32> &V) {
    if (cmp_vec(U, V) < 0) return {vector<u32>(1, 0), U};
    vector<u32> u = U, v = V;
    int n = (int)v.size();
    if (n == 1) {
      vector<u32> q(u.size(), 0);
      u64 rem = 0;
      for (int i = (int)u.size() - 1; i >= 0; --i) {
        u64 cur = (rem << 32) | u[i];
        q[i] = u32(cur / v[0]);
        rem = cur % v[0];
      }
      trim(q);
      return {q, vector<u32>(1, u32(rem))};
    }
    int m = (int)u.size() - n;
    u64 d = ((u64)1 << 32) / ((u64)v.back() + 1);
    auto mul_small = [](const vector<u32> &x, u64 d) {
      vector<u32> r(x.size() + 1, 0);
      u64 carry = 0;
      for (int i = 0; i < (int)x.size(); ++i) {
        u64 cur = (u64)x[i] * d + carry;
        r[i] = u32(cur);
        carry = cur >> 32;
      }
      r.back() = u32(carry);
      trim(r);
      return r;
    };
    u = mul_small(u, d), v = mul_small(v, d);
    if ((int)u.size() == m + n) u.push_back(0);
    vector<u32> q(m + 1, 0);
    for (int j = m; j >= 0; --j) {
      u64 num = ((u64)u[j + n] << 32) | u[j + n - 1];
      u64 qhat = num / v[n - 1], rhat = num % v[n - 1];
      while (qhat >= (1ull << 32) || qhat * v[n - 2] > ((rhat << 32) + u[j + n - 2])) {
        --qhat, rhat += v[n - 1];
        if (rhat >= (1ull << 32)) break;
      }
      u64 borrow = 0;
      for (int i = 0; i < n; ++i) {
        u64 p = qhat * v[i] + borrow;
        u32 cur = u[j + i];
        u32 pl = u32(p);
        bool under = cur < pl;
        u[j + i] = cur - pl;
        borrow = (p >> 32) + (under ? 1 : 0);
      }
      if (borrow > u[j + n]) {
        --qhat;
        u64 carry = 0;
        for (int i = 0; i < n; ++i) {
          u64 sum = (u64)u[j + i] + v[i] + carry;
          u[j + i] = u32(sum);
          carry = sum >> 32;
        }
        u[j + n] = u32(u[j + n] + carry);
      }
      else {
        u[j + n] = u32(u[j + n] - borrow);
      }
      q[j] = u32(qhat);
    }
    trim(q);
    vector<u32> r(n, 0);
    u64 rem = 0;
    for (int i = n - 1; i >= 0; --i) {
      u64 cur = (rem << 32) | u[i];
      r[i] = u32(cur / d);
      rem = cur % d;
    }
    trim(r);
    return {q, r};
  }


  static vector<u32> to_twos(const bigint &x, int n) {
    vector<u32> r(n, 0);
    for (int i = 0; i < (int)x.a.size() && i < n; ++i) r[i] = x.a[i];
    if (x.neg) {
      for (int i = 0; i < n; ++i) r[i] = ~r[i];
      u64 carry = 1;
      for (int i = 0; i < n && carry; ++i) {
        u64 cur = (u64)r[i] + carry;
        r[i] = u32(cur);
        carry = cur >> 32;
      }
    }
    return r;
  }

  static bigint from_twos(const vector<u32> &v) {
    bool neg = (v.back() >> 31) & 1;
    vector<u32> mag = v;
    if (neg) {
      for (auto &w : mag) w = ~w;
      u64 carry = 1;
      for (int i = 0; i < (int)mag.size() && carry; ++i) {
        u64 cur = (u64)mag[i] + carry;
        mag[i] = u32(cur);
        carry = cur >> 32;
      }
    }
    trim(mag);
    return bigint(neg, mag);
  }

  bigint operator-() const {
    bigint r = *this;
    if (!r.is_zero()) r.neg = !r.neg;
    return r;
  }
  bigint operator+() const { return *this; }
  bigint operator+(const bigint &o) const {
    if (neg == o.neg) return {neg, add_abs(a, o.a)};
    int c = cmp_abs(*this, o);
    if (c == 0) return bigint();
    if (c > 0) return {neg, sub_abs(a, o.a)};
    return {o.neg, sub_abs(o.a, a)};
  }
  bigint operator-(const bigint &o) const { return *this + (-o); }
  bigint operator*(const bigint &o) const { return {static_cast<bool>(neg ^ o.neg), mul_abs(a, o.a)}; }
  pair<bigint, bigint> divmod(const bigint &o) const {
    auto [q, r] = divmod_abs(a, o.a);
    bigint Q{static_cast<bool>(neg ^ o.neg), q}, R{neg, r};
    if (Q.is_zero()) Q.neg = false;
    if (R.is_zero()) R.neg = false;
    return {Q, R};
  }
  bigint operator/(const bigint &o) const { return divmod(o).first; }
  bigint operator%(const bigint &o) const { return divmod(o).second; }
  bigint &operator+=(const bigint &o) { return *this = *this + o; }
  bigint &operator-=(const bigint &o) { return *this = *this - o; }
  bigint &operator*=(const bigint &o) { return *this = *this * o; }
  bigint &operator/=(const bigint &o) { return *this = *this / o; }
  bigint &operator%=(const bigint &o) { return *this = *this % o; }
  bigint &operator&=(const bigint &o) { return *this = *this & o; }
  bigint &operator|=(const bigint &o) { return *this = *this | o; }
  bigint &operator^=(const bigint &o) { return *this = *this ^ o; }
  bigint &operator<<=(long long k) { return *this = *this << k; }
  bigint &operator>>=(long long k) { return *this = *this >> k; }
  bigint &operator<<=(const bigint &k) { return *this = *this << k; }
  bigint &operator>>=(const bigint &k) { return *this = *this >> k; }
  bigint &operator++() { return *this += 1; }
  bigint &operator--() { return *this -= 1; }
  bigint operator++(int) {
    bigint t = *this;
    ++*this;
    return t;
  }
  bigint operator--(int) {
    bigint t = *this;
    --*this;
    return t;
  }


  bigint operator&(const bigint &o) const {
    int n = max(bitlen(), o.bitlen()) / 32 + 2;
    vector<u32> a = to_twos(*this, n), b = to_twos(o, n);
    for (int i = 0; i < n; ++i) a[i] &= b[i];
    return from_twos(a);
  }
  bigint operator|(const bigint &o) const {
    int n = max(bitlen(), o.bitlen()) / 32 + 2;
    vector<u32> a = to_twos(*this, n), b = to_twos(o, n);
    for (int i = 0; i < n; ++i) a[i] |= b[i];
    return from_twos(a);
  }
  bigint operator^(const bigint &o) const {
    int n = max(bitlen(), o.bitlen()) / 32 + 2;
    vector<u32> a = to_twos(*this, n), b = to_twos(o, n);
    for (int i = 0; i < n; ++i) a[i] ^= b[i];
    return from_twos(a);
  }
  bigint operator~() const {
    vector<u32> a = to_twos(*this, bitlen() / 32 + 2);
    for (auto &w : a) w = ~w;
    return from_twos(a);
  }
  bool operator!() const { return is_zero(); }

  bool operator&&(const bigint &o) const { return !is_zero() && !o.is_zero(); }
  bool operator||(const bigint &o) const { return !is_zero() || !o.is_zero(); }


  bigint operator<<(long long k) const {
    if (k < 0) return *this >> -k;
    if (k == 0 || is_zero()) return *this;
    u64 sh = (u64)k;
    int ls = (int)(sh / 32), bs = (int)(sh % 32);
    vector<u32> r(a.size() + ls + 1, 0);
    for (int i = 0; i < (int)a.size(); ++i) {
      u64 cur = (u64)a[i] << bs;
      r[i + ls] |= u32(cur);
      r[i + ls + 1] |= u32(cur >> 32);
    }
    trim(r);
    return bigint(neg, r);
  }
  bigint operator>>(long long k) const {
    if (k < 0) return *this << -k;
    if (k == 0 || is_zero()) return *this;
    u64 sh = (u64)k;
    bigint m = abs();
    if ((u64)m.bitlen() <= sh) return bigint(neg ? -1 : 0);
    if (neg) m += (bigint(1) << k) - 1;
    int ls = (int)(sh / 32), bs = (int)(sh % 32);
    int n = (int)m.a.size();
    vector<u32> r(n - ls, 0);
    for (int i = 0; i < (int)r.size(); ++i) {
      u64 cur = m.a[i + ls];
      if (bs && i + ls + 1 < n) cur |= (u64)m.a[i + ls + 1] << 32;
      r[i] = u32(cur >> bs);
    }
    trim(r);
    return bigint(neg, r);
  }
  bigint operator<<(const bigint &k) const { return *this << k.to_ll(); }
  bigint operator>>(const bigint &k) const { return *this >> k.to_ll(); }

  bool operator<(const bigint &o) const {
    if (neg != o.neg) return neg;
    int c = cmp_abs(*this, o);
    return neg ? c > 0 : c < 0;
  }
  bool operator>(const bigint &o) const { return o < *this; }
  bool operator<=(const bigint &o) const { return !(o < *this); }
  bool operator>=(const bigint &o) const { return !(*this < o); }
  bool operator==(const bigint &o) const { return neg == o.neg && a == o.a; }
  bool operator!=(const bigint &o) const { return !(*this == o); }

  bigint pow(long long b) const {
    bigint r = 1, x = *this;
    while (b) {
      if (b & 1) r = r * x;
      x = x * x, b >>= 1;
    }
    return r;
  }
  friend bigint gcd(bigint a, bigint b) {
    a = a.abs(), b = b.abs();
    while (!b.is_zero()) {
      auto qr = a.divmod(b);
      a = b, b = qr.second;
    }
    return a;
  }

  string to_string() const {
    if (is_zero()) return "0";
    vector<u32> t = a;
    vector<string> chunk;
    while (!(t.size() == 1 && t[0] == 0)) {
      u64 rem = 0;
      for (int i = (int)t.size() - 1; i >= 0; --i) {
        u64 cur = (rem << 32) | t[i];
        t[i] = u32(cur / 1000000000u);
        rem = cur % 1000000000u;
      }
      trim(t);
      char buf[16];
      snprintf(buf, sizeof buf, "%09u", (u32)rem);
      chunk.push_back(buf);
    }
    string res = neg ? "-" : "";
    string head = chunk.back();
    int k = 0;
    while (k + 1 < (int)head.size() && head[k] == '0') ++k;
    res += head.substr(k);
    for (int i = (int)chunk.size() - 2; i >= 0; --i) res += chunk[i];
    return res;
  }

  friend ostream &operator<<(ostream &os, const bigint &x) { return os << x.to_string(); }
  friend istream &operator>>(istream &is, bigint &x) {
    string s;
    is >> s;
    x = bigint(s);
    return is;
  }

  private:
  bigint(bool n, vector<u32> v) : neg(n), a(std::move(v)) { trim(); }
};

