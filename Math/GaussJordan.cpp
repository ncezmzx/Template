#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int md = 998244353;
int qpow(int a, int b) {
  int r = 1;
  for (; b; b >>= 1, a = a * a % md)
    if (b & 1) r = r * a % md;
  return r;
}

// Gauss-Jordan elimination over the prime field Z_md
template <size_t N>
struct gauss_jordan {
  int a[N][N];  // a[i][j] coefficients, a[i][n] constants
  // returns 1 = unique solution (x[i] in a[i][n]), 2 = infinitely many, 0 = none
  int solve(int n) {
    int r = 0;
    for (int c = 0; c < n; ++c) {
      int p = r;
      while (p < n && !a[p][c]) ++p;
      if (p == n) continue;
      for (int j = c; j <= n; ++j) swap(a[p][j], a[r][j]);
      int inv = qpow(a[r][c], md - 2);
      for (int j = c; j <= n; ++j) a[r][j] = a[r][j] * inv % md;
      for (int i = 0; i < n; ++i)
        if (i != r && a[i][c]) {
          int t = a[i][c];
          for (int j = c; j <= n; ++j) {
            // one mod + conditional subtract instead of two mods (t*a[r][j] < 2^60)
            int v = a[i][j] - (int)(t * a[r][j] % md);
            a[i][j] = v < 0 ? v + md : v;
          }
        }
      ++r;
    }
    for (int i = r; i < n; ++i)
      if (a[i][n]) return 0;
    return r == n ? 1 : 2;
  }
};

/*
 * ============================================================
 * ============================================================
 * Name: Gauss-Jordan elimination (prime field; solve linear systems / rank)
 * Complexity: O(n^3)
 * Usage: solve n-variable linear systems Ax = b, wrapped as gauss_jordan<N>:
 *        a[i][j] coefficients, a[i][n] constants; solve(n) returns 1
 *        (unique solution, in a[i][n]), 2 (infinitely many), 0 (none); a
 *        fully-pivoting double-precision version is included in comments
 *        (better numerical stability for real systems)
 * Principle: Gauss-Jordan: for each column pick a non-zero pivot row and
 *        eliminate that column from ALL other rows, reaching a diagonal
 *        matrix whose entries are the solution; no back-substitution needed
 * Notes: the modulus must be prime (division via inverses); pivot-less
 *        columns correspond to free variables (infinite-solution case);
 *        matrix inverse: run the same elimination on [A | I]; when the left
 *        side becomes I, the right side is A^{-1}
 * ============================================================
 * Example (uncomment to compile; solves x+2y=5, 3x+4y=6 -> x=-4, y=4.5 mod p):

 * static gauss_jordan<505> gj;
 * signed main() {
 *   int n = 2;
 *   gj.a[0][0] = 1, gj.a[0][1] = 2, gj.a[0][2] = 5;
 *   gj.a[1][0] = 3, gj.a[1][1] = 4, gj.a[1][2] = 6;
 *   cout << gj.solve(n) << '\n';
 *   for (int i = 0; i < n; ++i) cout << gj.a[i][n] << ' ';
 * }
 * // double-precision version (commented out):
 * // const long double eps = 1e-9; long double b[N][N];
 * // int gauss_double(int n) {
 * //   for (int c = 0, r = 0; c < n; ++c) {
 * //     int p = r;
 * //     for (int i = r + 1; i < n; ++i)
 * //       if (fabs(b[i][c]) > fabs(b[p][c])) p = i;
 * //     if (fabs(b[p][c]) < eps) continue;
 * //     for (int j = c; j <= n; ++j) swap(b[p][j], b[r][j]);
 * //     for (int i = 0; i < n; ++i)
 * //       if (i != r && fabs(b[i][c]) > eps) {
 * //         long double t = b[i][c] / b[r][c];
 * //         for (int j = c; j <= n; ++j) b[i][j] -= t * b[r][j];
 * //       }
 * //     ++r;
 * //   }
 * //   for (int i = r; i < n; ++i) if (fabs(b[i][n]) > eps) return 0;
 * //   return r == n ? 1 : 2;
 * // }
 * ============================================================
 */
