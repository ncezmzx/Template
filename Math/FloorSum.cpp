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
 * ============================================================
 * Name: Euclidean-style floor_sum (sum floor((a*i+b)/m), aligned with ACL)
 * Complexity: O(log max(a, m)) (each round at least halves the scale, like gcd)
 * Usage: classic lattice counting: integer points under (on) the line
 *        y = (a*x+b)/m; floor-sums, modulo distributions of arithmetic
 *        progressions, and friends
 * Interface: floor_sum(n, m, a, b) = sum_{i=0}^{n-1} floor((a*i + b) / m);
 *        a, b may be negative (ACL semantics); 0 <= n < 2^32, 1 <= m < 2^32
 * Principle: first normalize a, b to non-negative (whole blocks subtracted
 *        for the negative parts); then Euclidean descent: integer parts
 *        (a/m, b/m) counted as arithmetic sums; the remaining affine part
 *        swaps axes, reducing to a smaller instance (new n = (a*n+b)/m),
 *        converging like gcd; unsigned 64-bit multiplies inside (ACL style,
 *        safe for n, m < 2^32)
 * Source: ported from AtCoder Library math.hpp floor_sum (same algorithm)
 * Notes: i starts at 0; the result may be large (long long); m >= 1
 * ============================================================
 * Example (uncomment to compile):

 * signed main() {
 *   cout << floor_sum(4, 2, 3, 1) << '\n';
 *   // i=0..3: floor(1/2)+floor(4/2)+floor(7/2)+floor(10/2) = 0+2+3+5 = 10
 *   cout << floor_sum(10, 7, -3, 5) << '\n';
 *   // i=0..9: 0+0-1-1-1-2-2-3-3-4 = -17 (floor, negative terms included)
 *   cout << floor_sum(1, 1, 0, 0) << '\n';   // 0
 *   cout << floor_sum(0, 3, 9, 2) << '\n';   // 0 (empty sum)
 * }
 */
