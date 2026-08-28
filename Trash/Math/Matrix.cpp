#include <bits/stdc++.h>
using namespace std;
#define int long long
using d64 = long double;

// generic numeric matrix with multiplication and fast exponentiation
struct matrix {
  vector<d64> vec;
  int n, m;
  auto operator[](const int i) { return vec.begin() + i * m; }
  auto operator[](const int i) const { return vec.begin() + i * m; }
  matrix() = default;
  matrix(int _n, int _m) : n(_n), m(_m), vec(_n * _m, 0) {}
};
matrix operator*(const matrix &lhs, const matrix &rhs) {
  assert(lhs.m == rhs.n);
  matrix ret(lhs.n, rhs.m);
  for (int i = 0; i < lhs.n; ++i) {
    for (int j = 0; j < rhs.m; ++j) {
      for (int k = 0; k < lhs.m; ++k) {
        ret[i][j] += lhs[i][k] * rhs[k][j];
      }
    }
  }
  return ret;
}
matrix I(int n) {  // identity matrix
  matrix ret(n, n);
  for (int i = 0; i < n; ++i) ret[i][i] = 1;
  return ret;
}
matrix qpow(matrix a, int b) {  // matrix fast power
  matrix r = I(a.n);
  for (; b; b >>= 1, a = a * a)
    if (b & 1) r = r * a;
  return r;
}

/*
 * ============================================================
 * Name: matrix multiplication + fast exponentiation (generic numeric matrix)
 * Complexity: multiplication O(n*m*k), fast power O(n^3 log b)
 * Usage: linear recurrences (Fibonacci etc.) and matrix-accelerated graph DP;
 *        element type d64 = long double; struct matrix + operator* + I + qpow
 * Source: all.cpp 11231-11263 (struct matrix + operator* + I + qpow,
 *         comments translated)
 * ============================================================
 * Example (uncomment to compile):
 * signed main() {
 *   matrix A(2, 2);
 *   A[0][0] = 1; A[0][1] = 1; A[1][0] = 1; A[1][1] = 0;  // Fibonacci transition
 *   matrix F = qpow(A, 10);  // fast power
 *   cout << fixed << setprecision(0) << F[0][1] << '\n';  // fib(10) = 55
 *   matrix B = A * A;  // plain multiply
 *   cout << B[0][0] << ' ' << B[0][1] << ' ' << B[1][0] << ' ' << B[1][1] << '\n';
 * }
 * ============================================================
 */
