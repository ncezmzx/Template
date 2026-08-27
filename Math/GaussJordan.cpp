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

constexpr int N = 505;
int a[N][N];

int gauss(int n) {
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
          // 一次取模 + 条件减替代两次取模（t*a[r][j] < 2^60 安全）
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

/*
 * ============================================================
 * 名称：高斯-约旦消元（模素数域，解线性方程组 / 求秩）
 * 复杂度：O(n^3)
 * 用途：解 n 元线性方程组 Ax = b（a[i][j] 为系数，a[i][n] 为常数项）：
 *       gauss(n) 返回 1（唯一解，解在 a[i][n]）、2（无穷多解）、0（无解）；
 *       全选主元的双精度版本见注释（数值稳定性更好，用于实数方程组）
 * 原理：高斯-约旦：对每一列选一个非零主元行，消去**所有**其他行该列，
 *       得到对角矩阵，解直接读出；比高斯消元少一次回代
 * 注意：模数需为素数（除法用逆元）；无主元列对应自由元（无穷多解情形）；
 *       矩阵求逆：对 [A | I] 做同样消元，左侧变 I 时右侧即 A^{-1}
 * ============================================================
 * 使用示例（编译时取消注释；解 2 元方程组 x+2y=5, 3x+4y=6 → x=-4, y=4.5 mod p）：
 * signed main() {
 *   int n = 2;
 *   a[0][0] = 1, a[0][1] = 2, a[0][2] = 5;
 *   a[1][0] = 3, a[1][1] = 4, a[1][2] = 6;
 *   cout << gauss(n) << '\n';
 *   for (int i = 0; i < n; ++i) cout << a[i][n] << ' ';
 * }
 * // 双精度版（注释）：
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
