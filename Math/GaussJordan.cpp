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
template <size_t N> struct gauss_jordan {
  int a[N][N]; // a[i][j] coefficients, a[i][n] constants
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
 * Name: Gauss-Jordan elimination (prime field; solve linear systems / rank)
 * Complexity: O(n^3)
 * Usage: solve n-variable linear systems Ax = b, `gauss_jordan<N>`: a[i][j]
 *        holds the coefficients and a[i][n] the constants; solve(n) returns 1
 *        (unique solution, left in a[i][n]),
 *        2 (infinitely many) or 0 (none). A fully-pivoting double-precision
 *        version is included in the comments for real systems.
 * Principle: Gauss-Jordan: for each column pick a non-zero pivot row and
 *            eliminate that column from ALL other rows, reaching a diagonal
 *            matrix whose entries are the solution, so no back-substitution is
 *            needed
 * Notes: the modulus must be prime (division goes through inverses); pivot-less
 *        columns are free variables (the infinite-solution case); for the
 *        matrix inverse run the same elimination on [A | I], the right side
 *        becomes A^{-1}
 * ============================================================
 */
