#include <bits/stdc++.h>
using namespace std;
#define int long long

// extended Euclid, three-argument form: returns {x, y, d} with a*x + b*y = c
// (c must be a multiple of gcd(a, b)); d = gcd(a, b)
tuple<int, int, int> exgcd(int a, int b, int c) {
  if (!b) return {c / a, 0, a};
  auto [y, x, d] = exgcd(b, a % b, c);
  return {x, y - a / b * x, d};
}
// clean wrapper: returns {x, y, g} with a*x + b*y = g = gcd(a, b)
tuple<int, int, int> exgcd(int a, int b) {
  return exgcd(a, b, __gcd(a, b));
}

/*
 * ============================================================
 * Name: extended Euclid (exgcd)
 * Complexity: O(log min(a, b))
 * Usage: solve a*x + b*y = c (c a multiple of gcd(a,b)), modular inverses,
 *        congruences; kept as free functions:
 *        exgcd(a, b) -> {x, y, g}; exgcd(a, b, c) -> {x, y, d}
 * Source: all.cpp 42447-42451 (author's three-argument version; the clean
 *         two-argument wrapper is built on the same implementation)
 * ============================================================
 * Example (uncomment to compile):
 * signed main() {
 *   auto [x, y, g] = exgcd(30, 12);
 *   cout << x << ' ' << y << ' ' << g << '\n';  // 1 -2 6, 30*1 + 12*(-2) = 6
 *   auto [x2, y2, d2] = exgcd(30, 12, 6);  // three-argument form: solve a*x + b*y = c
 *   cout << x2 << ' ' << y2 << ' ' << d2 << '\n';  // 1 -2 6
 * }
 * ============================================================
 */
