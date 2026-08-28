#include <bits/stdc++.h>
using namespace std;
#define int long long

// extended gcd: a*x + b*y = gcd(a, b)
int exgcd(int a, int b, int& x, int& y) {
  if (!b) return x = 1, y = 0, a;
  int g = exgcd(b, a % b, y, x);
  y -= a / b * x;
  return g;
}
// returns (g, x): g = gcd(a, b), a*x == g (mod b), 0 <= x < b (b != 0)
pair<int, int> inv_gcd(int a, int b) {
  a %= b;
  if (a < 0) a += b;
  int x, y, g = exgcd(a, b, x, y);
  if (g != 1) x = (x % (b / g) + (b / g)) % (b / g);  // smallest non-negative solution
  else x = (x % b + b) % b;
  return {g, x};
}
int safe_mod(int a, int m) { a %= m; return a < 0 ? a + m : a; }

// CRT merging possibly non-coprime moduli (aligned with ACL crt):
// solves x = r[i] (mod m[i]); returns {r, m} with 0 <= r < m = lcm;
// returns {0, 0} when unsolvable
pair<int, int> crt(vector<int> r, vector<int> m) {
  int r0 = 0, m0 = 1;  // merged so far: x = r0 (mod m0)
  for (int i = 0; i < (int)r.size(); i++) {
    int r1 = safe_mod(r[i], m[i]), m1 = m[i];
    if (m0 < m1) swap(r0, r1), swap(m0, m1);
    if (m0 % m1 == 0) {
      if (r0 % m1 != r1) return {0, 0};
      continue;
    }
    // merge: x = r0 + t*m0 = r1 (mod m1) -> t*m0 = r1-r0 (mod m1)
    int g, im;
    tie(g, im) = inv_gcd(m0, m1);
    if ((r1 - r0) % g) return {0, 0};
    int u1 = m1 / g;
    int t = (r1 - r0) / g % u1 * im % u1;  // 0 <= t < u1
    r0 += t * m0;                          // |r0| < m0 + m0*(u1-1) < lcm
    m0 *= u1;                              // m0 = lcm(m0, m1)
    if (r0 < 0) r0 += m0;
  }
  return {r0, m0};
}

/*
 * ============================================================
 * ============================================================
 * Name: Chinese Remainder Theorem (merges non-coprime moduli, aligned with ACL crt)
 * Complexity: one merge O(log) (exgcd), total O(k log M) for k congruences
 * Usage: solve the system x = r[i] (mod m[i]) (moduli need not be coprime):
 *        returns {r, m}, the unique solution x = r (mod lcm(m)), 0 <= r < lcm;
 *        {0, 0} when unsolvable; i.e. a vectorized exCRT
 * Principle: merge (r0, m0) with (r1, m1) one by one: with g = gcd(m0, m1),
 *        (r1-r0) must be divisible by g, otherwise unsolvable; else
 *        t = (r1-r0)/g * inv(m0/g) (mod m1/g), new solution r0 += t*m0,
 *        new modulus lcm = m0*m1/g (nothing overflows long long)
 * Interface: crt(r[], m[]) (equal-length arrays); exgcd / inv_gcd are reusable
 * Source: ported from AtCoder Library math.hpp crt (same algorithm)
 * Notes: requires 1 <= m[i]; r[i] may be negative (safe_mod internally);
 *        the lcm must fit in long long (same constraint as ACL)
 * ============================================================
 * Example (uncomment to compile):

 * signed main() {
 *   // classic: x%3==2, x%5==3, x%7==2 -> x = 23 (mod 105)
 *   auto res = crt({2, 3, 2}, {3, 5, 7});
 *   cout << res.first << ' ' << res.second << '\n';   // 23 105
 *   // non-coprime, solvable: x = 3 (mod 4), x = 5 (mod 6) -> x = 11 (mod 12)
 *   auto sol = crt({3, 5}, {4, 6});
 *   cout << sol.first << ' ' << sol.second << '\n';   // 11 12
 *   // non-coprime, unsolvable: x = 1 (mod 4) vs x = 2 (mod 6), parity conflict
 *   auto bad = crt({1, 2}, {4, 6});
 *   cout << bad.first << ' ' << bad.second << '\n';   // 0 0
 *   // non-coprime, solvable: x = 2 (mod 4), x = 4 (mod 10) -> x = 14 (mod 20)
 *   auto ok = crt({2, 4}, {4, 10});
 *   cout << ok.first << ' ' << ok.second << '\n';     // 14 20
 * }
 */
