#include <bits/stdc++.h>
using namespace std;
#define int long long

int pw(int x, int n, int p) {
  int r = 1 % p;
  x %= p;
  while (n) {
    if (n & 1) r = r * x % p;
    x = x * x % p, n >>= 1;
  }
  return r;
}
int exgcd(int a, int b, int& x, int& y) {
  if (!b) return x = 1, y = 0, a;
  int g = exgcd(b, a % b, y, x);
  y -= a / b * x;
  return g;
}
int inv_mod(int a, int m) {  // requires gcd(a, m) = 1
  int x, y;
  exgcd(a, m, x, y);
  return (x % m + m) % m;
}

// ---------- Lucas: C(n, m) mod p (prime p <= ~1e6; call init(p) once) ----------
struct lucas {
  int p_;
  vector<int> fac_, ifac_;
  void init(int p) {
    p_ = p;
    fac_.assign(p, 1), ifac_.assign(p, 1);
    for (int i = 1; i < p; ++i) fac_[i] = fac_[i - 1] * i % p;
    ifac_[p - 1] = pw(fac_[p - 1], p - 2, p);
    for (int i = p - 1; i; --i) ifac_[i - 1] = ifac_[i] * i % p;
  }
  int c_small(int n, int m) {  // 0 <= n, m < p
    if (m < 0 || m > n) return 0;
    return fac_[n] * ifac_[m] % p_ * ifac_[n - m] % p_;
  }
  int solve(int n, int m) {  // p prime; n, m up to 1e18
    if (m < 0 || m > n) return 0;
    if (m == 0) return 1;
    return c_small(n % p_, m % p_) * solve(n / p_, m / p_) % p_;
  }
};

// ---------- exLucas: C(n, m) mod P for arbitrary positive P ----------
int fact_pe(int n, int p, int pe) {  // n! with all factors p removed, mod pe
  if (!n) return 1;
  int res = 1;
  for (int i = 1; i <= pe; ++i)  // product of non-multiples of p in one full block [1, pe]
    if (i % p) res = res * i % pe;
  res = pw(res, n / pe, pe);
  for (int i = 1; i <= n % pe; ++i)  // remaining tail
    if (i % p) res = res * i % pe;
  return res * fact_pe(n / p, p, pe) % pe;  // recurse on the p-multiple part divided by p
}
int C_mod_pe(int n, int m, int p, int pe, int e) {  // C(n, m) mod p^e
  if (m < 0 || m > n) return 0;
  int k = 0;  // exponent of p in C(n, m) (Kummer / Legendre)
  int a = n, b = m, c = n - m;
  while (a) a /= p, b /= p, c /= p, k += a - b - c;
  if (k >= e) return 0;
  int r = fact_pe(n, p, pe) * inv_mod(fact_pe(m, p, pe), pe) % pe;
  r = r * inv_mod(fact_pe(n - m, p, pe), pe) % pe;
  return r * pw(p, k, pe) % pe;
}
int exlucas(int n, int m, int P) {  // C(n, m) mod P; factor P into prime powers, merge by CRT
  if (m < 0 || m > n) return 0;
  int r = 0, mod = 1;
  int PP = P;
  for (int p = 2; p * p <= PP; ++p)
    if (PP % p == 0) {
      int e = 0, pe = 1;
      while (PP % p == 0) PP /= p, ++e, pe *= p;
      int cr = C_mod_pe(n, m, p, pe, e), t = (cr - r) % pe;  // CRT merge (coprime)
      if (t < 0) t += pe;
      r += mod * (t * inv_mod(mod % pe, pe) % pe), mod *= pe;
    }
  if (PP > 1) {  // remaining large prime factor
    int cr = C_mod_pe(n, m, PP, PP, 1), t = (cr - r) % PP;
    if (t < 0) t += PP;
    r += mod * (t * inv_mod(mod % PP, PP) % PP), mod *= PP;
  }
  return r;
}

/*
 * ============================================================
 * ============================================================
 * Name: Lucas / exLucas (large binomial coefficients modulo m)
 * Complexity: Lucas O(log_p n) (table build O(p)); exLucas O(sqrt P + sum p^e * log_p n)
 * Usage: struct lucas: lc.init(p) builds the factorial table (p up to ~1e6),
 *        then lc.solve(n, m) = C(n, m) mod p for prime p (n, m up to 1e18);
 *        exlucas(n, m, P): arbitrary positive P (prime-power factorization +
 *        CRT merge)
 * Principle: Lucas' theorem C(n,m) = C(n/p, m/p) * C(n%p, m%p) (mod p),
 *        expanding digit by digit in base p; exLucas handles each prime
 *        power p^e by "stripping p factors from factorials" (fact_pe uses
 *        the periodic block [1, p^e] of non-multiples of p + recursion) to
 *        get the p-free part of C, then multiplies back p^k (k = Kummer carry
 *        count); results across prime powers merge by CRT
 * Notes: keep exLucas' P <= 1e9 (the internal mod * pe products must fit in
 *        long long); duplicates Math/CRT.cpp's exgcd/inv_mod (self-contained here)
 * ============================================================
 * Example (uncomment to compile):

 * signed main() {
 *   lucas lc;
 *   lc.init(7);
 *   cout << lc.solve(10, 3) << '\n';        // 1（C(10,3)=120 = 17×7+1）
 *   cout << lc.solve(25, 12) << '\n';       // 0（25=34₇, 12=15₇，C(4,5)=0 → 0）
 *   cout << exlucas(10, 3, 12) << '\n';     // 0（C(10,3)=120 ≡ 0 mod 12）
 *   cout << exlucas(10, 2, 12) << '\n';     // 9（C(10,2)=45 = 3×12+9）
 * }
 */
