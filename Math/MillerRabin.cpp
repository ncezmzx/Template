#include <bits/stdc++.h>
using namespace std;
#define int long long
int mul(int x, int y, int md) { return (unsigned __int128) x * y % md; }
int qpow(int a, int b, int md) {
  int r = 1;
  for (; b > 0; b >>= 1, a = mul(a, a, md))
    if (b & 1) r = mul(r, a, md);
  return r;
}
bool MillerRabin(int n) {
  if (n == 2) return true;
  if (n <= 1 || (n & 1 ^ 1)) return false;
  int u = n - 1, t = 0;
  while (u & 1 ^ 1) u >>= 1, ++t;
  auto chk = [&](int a) -> bool {
    int k = qpow(a % n, u, n);
    if (k <= 1) return true;
    for (int i = 0; i < t; ++i) {
      if (k == n - 1) return true;
      k = mul(k, k, n);
    }
    return false;
  };
  static constexpr int bas[7] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
  for (int i : bas) {
    if (n % i == 0) return n == i;
    if (!chk(i)) return false;
  }
  return true;
}
/*
 * ============================================================
 * 名称：Miller-Rabin 素性测试（确定性基）
 * 复杂度：O(k log^3 n)，k = 7 个基
 * 用途：判断 int64 范围内的数是否为素数（确定性），常配合 Pollard-Rho 做因数分解。
 * 来源：all.cpp 行 33194-33223（原样保留；注释已统一移至文件尾部）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   int tt;
 *   cin >> tt;
 *   while (tt--) {
 *     int n;
 *     cin >> n;
 *     cout << (MillerRabin(n) ? "Yes\n" : "No\n");
 *   }
 * }
 * ============================================================
 */
