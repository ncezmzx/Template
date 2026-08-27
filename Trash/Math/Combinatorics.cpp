#include <bits/stdc++.h>
using namespace std;
/*
 * 名称：组合数（fac / 逆元阶乘 inv(=ifac) / C(n,m)）
 * 复杂度：预处理 O(n)（qpow 一次 O(log md)），单次查询 C(n,m) O(1)
 * 用途：模质数下组合数 / 排列数，配合容斥、DP 使用
 * 使用示例：先按 main 中的两行预处理 fac/inv，再 cout << C(n, m);
 * 来源：all.cpp 10821-10833（qpow + C(n,m)；fac/inv 预处理来自 10842-10844）
 */
#define int long long
constexpr int N = 1e5 + 9, md = 998244353;
int fac[N], inv[N];  // inv 为逆元阶乘（ifac），inv[i] = 1 / i!
int qpow(int a, int b = md - 2) {
  int r = 1;
  for (; b; b >>= 1, a = a * a % md)
    if (b & 1) r = r * a % md;
  return r;
}
int C(int n, int m) {
  if (n < m) return 0;
  return fac[n] * inv[m] % md * inv[n - m] % md;
}
// ---- O(n) 线性逆元表变体（来源 all.cpp 915-916），需要 1..n 每个数逆元时用 ----
// inv[0] = inv[1] = 1;
// for (int i = 2; i <= n; ++i) inv[i] = (md - md / i * inv[md % i] % md) % md;
#ifdef DEMO
signed main() {
  int n = 10;
  for (int i = fac[0] = 1; i <= n; ++i) fac[i] = fac[i - 1] * i % md;
  inv[n] = qpow(fac[n]);
  for (int i = n - 1; i >= 0; --i) inv[i] = inv[i + 1] * (i + 1) % md;
  cout << C(10, 3) << ' ' << C(5, 5) << ' ' << C(3, 5) << '\n';  // 120 1 0
}
#endif
