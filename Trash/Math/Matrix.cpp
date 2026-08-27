#include <bits/stdc++.h>
using namespace std;
/*
 * 名称：矩阵乘法 + 快速幂（通用数值矩阵类）
 * 复杂度：乘法 O(n*m*k)，快速幂 O(n^3 log b)
 * 用途：线性递推（斐波那契等）与图上 DP 的矩阵加速；元素类型 d64 = long double
 * 使用示例：matrix A(2, 2); ...; matrix F = qpow(A, k);
 * 来源：all.cpp 11231-11263（struct matrix + operator* + I + qpow）
 */
#define int long long
using d64 = long double;
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
matrix I(int n) {
  matrix ret(n, n);
  for (int i = 0; i < n; ++i) ret[i][i] = 1;
  return ret;
}
matrix qpow(matrix a, int b) {
  matrix r = I(a.n);
  for (; b; b >>= 1, a = a * a)
    if (b & 1) r = r * a;
  return r;
}
#ifdef DEMO
signed main() {
  matrix A(2, 2);
  A[0][0] = 1; A[0][1] = 1; A[1][0] = 1; A[1][1] = 0;  // 斐波那契转移矩阵
  matrix F = qpow(A, 10);  // 矩阵快速幂
  cout << fixed << setprecision(0) << F[0][1] << '\n';  // fib(10) = 55
  matrix B = A * A;  // 直接相乘
  cout << B[0][0] << ' ' << B[0][1] << ' ' << B[1][0] << ' ' << B[1][1] << '\n';
}
#endif
