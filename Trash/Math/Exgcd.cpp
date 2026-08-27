#include <bits/stdc++.h>
using namespace std;
/*
 * 名称：扩展欧几里得（exgcd）
 * 复杂度：O(log min(a,b))
 * 用途：解不定方程 a*x + b*y = c（c 为 gcd(a,b) 的倍数）、求模逆元、解同余方程
 * 使用示例：auto [x, y, g] = exgcd(a, b);  // 满足 a*x + b*y = g = gcd(a,b)
 * 来源：all.cpp 42447-42451（作者三参数版本；两参数干净封装基于同一实现）
 */
#define int long long
// 作者原版：三参数，返回 {x, y, d} 满足 a*x + b*y = c（要求 c 是 gcd(a,b) 的倍数），d = gcd(a,b)
tuple<int, int, int> exgcd(int a, int b, int c) {
  if (!b) return {c / a, 0, a};
  auto [y, x, d] = exgcd(b, a % b, c);
  return {x, y - a / b * x, d};
}
// 干净封装：返回 {x, y, g}，满足 a*x + b*y = g = gcd(a, b)
tuple<int, int, int> exgcd(int a, int b) {
  return exgcd(a, b, std::gcd(a, b));
}
#ifdef DEMO
signed main() {
  auto [x, y, g] = exgcd(30, 12);
  cout << x << ' ' << y << ' ' << g << '\n';  // 1 -2 6，30*1 + 12*(-2) = 6
  auto [x2, y2, d2] = exgcd(30, 12, 6);  // 三参数版本：解 a*x + b*y = c
  cout << x2 << ' ' << y2 << ' ' << d2 << '\n';  // 1 -2 6
}
#endif
