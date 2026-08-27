#include <bits/stdc++.h>
using namespace std;
#define int long long

int gcd_bin(int a, int b) {
  if (!a) return b;
  if (!b) return a;
  int s = __builtin_ctzll(a | b);
  a >>= __builtin_ctzll(a), b >>= __builtin_ctzll(b);
  while (a != b) {
    if (a > b) a = (a - b) >> __builtin_ctzll(a - b);
    else b = (b - a) >> __builtin_ctzll(b - a);
  }
  return a << s;
}

/*
 * ============================================================
 * 名称：二进制 GCD（Binary GCD，__builtin_ctz 加速）
 * 复杂度：O(log max(a,b)) 上界，实际均摊极快（常数远小于欧几里得取模版），
 *         多数情况下接近 O(1)，故也称 O(1) 级 gcd
 * 用途：替代 std::gcd / 欧几里得算法的取模，在需要大量 gcd 计算时（如
 *       枚举约数、区间 gcd 分块、Pollard-Rho 内部）显著提速；
 *       仅依赖位运算，无除法取模，对 64 位整数友好
 * 原理：先剥离公共因子 2；两奇数相减必为偶数，右移去掉因子 2 后规模减半，
 *       如此往复直至两数相等；最后乘回公共的 2 的幂
 * 用法：gcd_bin(a, b)；a、b 为非负 64 位整数；0 特判已包含
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   int a, b;
 *   cin >> a >> b;
 *   cout << gcd_bin(a, b) << '\n';
 * }
 * ============================================================
 */
