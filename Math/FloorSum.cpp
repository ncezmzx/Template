#include <bits/stdc++.h>
using namespace std;
#define int long long

// floor_sum(n, m, a, b) = sum_{i=0}^{n-1} floor((a*i + b) / m)
// constraints (same as ACL): 0 <= n < 2^32, 1 <= m < 2^32; a, b may be negative
int floor_sum(int n, int m, int a, int b) {
  unsigned long long ans = 0;
  if (a < 0) {  // normalize negative slope: subtract n(n-1)/2 * ceil(-a/m)
    int a2 = a % m;
    if (a2 < 0) a2 += m;
    ans -= 1ULL * n * (n - 1) / 2 * ((a2 - a) / m);
    a = a2;
  }
  if (b < 0) {
    int b2 = b % m;
    if (b2 < 0) b2 += m;
    ans -= 1ULL * n * ((b2 - b) / m);
    b = b2;
  }
  unsigned long long un = n, um = m, ua = a, ub = b;
  while (true) {  // Euclidean-like descent: O(log) rounds, shrinks like gcd
    if (ua >= um) {
      ans += un * (un - 1) / 2 * (ua / um);
      ua %= um;
    }
    if (ub >= um) {
      ans += un * (ub / um);
      ub %= um;
    }
    unsigned long long y_max = ua * un + ub;  // unsigned multiply safe for n,m < 2^32
    if (y_max < um) break;
    un = y_max / um;
    ub = y_max % um;
    swap(ua, um);  // swap axes: symmetric smaller subproblem
  }
  return (int)ans;
}

/*
 * ============================================================
 * Name: Euclidean-style floor_sum (sum floor((a*i+b)/m), aligned with ACL)
 * Complexity: O(log max(a, m)) (each round at least halves the scale, like gcd)
 * Usage: classic lattice counting: integer points under (on) the line y = (a*x
 *        + b)/m, floor sums, and modulo distributions of arithmetic
 *        progressions.
 *        Method list: see Interface below.
 * Interface: floor_sum(n, m, a, b) = sum_{i=0}^{n-1} floor((a*i + b) / m);
 *        a, b may be negative (ACL semantics); 0 <= n < 2^32, 1 <= m < 2^32
 * Principle: first normalize a and b to non-negative, then Euclidean descent:
 *            the integer parts (a/m, b/m) are counted as arithmetic sums and
 *            the remaining affine part swaps axes, reducing to a smaller
 *            instance; unsigned 64-bit multiplies inside (ACL style)
 * Source: ported from AtCoder Library math.hpp floor_sum (same algorithm)
 * Notes: i starts at 0; a and b may be negative (ACL semantics); 0 <= n < 2^32
 *        and 1 <= m < 2^32; the result may be large, use long long
 * ============================================================
 */
