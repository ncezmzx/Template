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

int lagrange(const vector<int>& x, const vector<int>& y, int k) {
  int n = (int)x.size(), res = 0;
  for (int i = 0; i < n; ++i) {
    int num = 1, den = 1;
    for (int j = 0; j < n; ++j)
      if (j != i) {
        num = num * ((k - x[j] + md) % md) % md;
        den = den * ((x[i] - x[j] + md) % md) % md;
      }
    res = (res + y[i] * num % md * qpow(den, md - 2)) % md;
  }
  return res;
}

constexpr int N = 2e6 + 9;
int fac[N], ifac[N];
int lagrange_1n(const vector<int>& y, int k) {
  int n = (int)y.size() - 1;
  if (k >= 1 && k <= n) return y[k];
  if (n == 0) return 0;
  fac[0] = ifac[0] = 1;
  for (int i = 1; i <= n; ++i) fac[i] = fac[i - 1] * i % md;
  ifac[n] = qpow(fac[n], md - 2);
  for (int i = n - 1; i >= 1; --i) ifac[i] = ifac[i + 1] * (i + 1) % md;
  vector<int> pre(n + 1), suf(n + 2);
  pre[0] = 1;
  for (int i = 1; i <= n; ++i) pre[i] = pre[i - 1] * ((k - i + md) % md) % md;
  suf[n + 1] = 1;
  for (int i = n; i >= 1; --i) suf[i] = suf[i + 1] * ((k - i + md) % md) % md;
  int res = 0;
  for (int i = 1; i <= n; ++i) {
    int a = pre[i - 1] * suf[i + 1] % md;
    int b = ifac[i - 1] * ifac[n - i] % md;
    if ((n - i) & 1) b = (md - b) % md;
    res = (res + y[i] * a % md * b) % md;
  }
  return res;
}

/*
 * ============================================================
 * 名称：拉格朗日插值
 * 复杂度：一般形式 O(n^2)；x 为 1..n 连续点时 O(n)
 * 用途：给定 n+1 个点 (x_i, y_i)（x_i 两两不同），求不超过 n 次的多项式在
 *       任意点 k 处的值：
 *       1) lagrange(x, y, k)：一般横坐标（可不在模意义下等差）
 *       2) lagrange_1n(y, k)：横坐标为 1..n 的 O(n) 版（前缀/后缀积 + 阶乘逆元），
 *          常用于"n 次多项式求 f(k)"（k 可远大于 n，甚至取模意义下的负值）
 * 原理：拉格朗日基多项式 L_i(k) = prod_{j!=i} (k - x_j) / (x_i - x_j)，
 *       答案 = sum y_i * L_i(k)
 * 注意：分母需非零（模素数且 x_i 两两不同）；lagrange_1n 中 y 下标 0..n 与
 *       横坐标 1..n 对应（y[0] 未使用）；k 用 long long 传入再取模
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   // 过 (0,1),(1,2),(2,4) 的二次多项式 f(k)=k^2+1
 *   cout << lagrange({0, 1, 2}, {1, 2, 4}, 5) << '\n';       // 26
 *   // 1^2+2^2+...+n^2 = n(n+1)(2n+1)/6 是 n 的三次多项式：
 *   // f(1)=1, f(2)=5, f(3)=14, f(4)=30 → 求 f(1e9)
 *   vector<int> y = {0, 1, 5, 14, 30};
 *   cout << lagrange_1n(y, 1000000000) << '\n';
 * }
 * ============================================================
 */
