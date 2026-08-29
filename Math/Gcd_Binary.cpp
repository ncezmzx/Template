#include <bits/stdc++.h>
using namespace std;
#define int long long

int gcd_bin(int a, int b) {
  if (!a) return b;
  if (!b) return a;
  int s = __builtin_ctzll(a | b);
  a >>= __builtin_ctzll(a), b >>= __builtin_ctzll(b);
  while (a != b) {
    if (a > b) a = (a - b) >> __builtin_ctzll(a - b);
    else b = (b - a) >> __builtin_ctzll(b - a);
  }
  return a << s;
}

/*
 * ============================================================
 * Name: binary GCD (__builtin_ctz accelerated)
 * Complexity: O(log max(a,b)) upper bound; near O(1) in practice (constants far
 *             below Euclid's modulo)
 * Usage: replaces std::gcd / Euclidean modulo when gcd is called in bulk
 *        (divisor enumeration, range-gcd blocking, inside Pollard-Rho); bit
 *        operations only, no division, 64-bit friendly: gcd_bin(a, b).
 * Principle: strip the common power of two first; the difference of two odd
 *            numbers is even, so right-shifting removes factors of two and
 *            halves the scale until the numbers are equal; multiply the common
 *            power of two back at the end
 * Notes: a and b are non-negative 64-bit integers; the zero cases are handled
 * ============================================================
 */
