#include <bits/stdc++.h>
using namespace std;

// 自适应辛普森积分：∫_a^b f(x) dx，eps 为相对/绝对混合容差
double simpson(double (*f)(double), double a, double b) {
  double m = (a + b) / 2;
  return (b - a) / 6 * (f(a) + 4 * f(m) + f(b));
}
double asr(double (*f)(double), double a, double b, double eps, double s) {
  double m = (a + b) / 2, sl = simpson(f, a, m), sr = simpson(f, m, b);
  if (fabs(sl + sr - s) <= 15 * eps) return sl + sr + (sl + sr - s) / 15;  // 15 系数来自误差外推
  return asr(f, a, m, eps / 2, sl) + asr(f, m, b, eps / 2, sr);
}
double integrate(double (*f)(double), double a, double b, double eps = 1e-8) {
  return asr(f, a, b, eps, simpson(f, a, b));
}

/*
 * ============================================================
 * 名称：自适应辛普森积分
 * 复杂度：视函数形态，通常 O(log((b-a)/eps)) 层递归
 * 用途：数值定积分 ∫_a^b f(x) dx（f 为光滑函数；经典题：求阴影
 *       面积 / 圆与函数围成面积等解析积分困难场景）
 * 原理：辛普森公式 (b-a)/6·(f(a)+4f(m)+f(b)) 对三次多项式精确；
 *       分半后比较误差估计 |sl+sr-s| <= 15·eps 时用 Richardson
 *       外推值返回，否则递归细分（自适应）
 * 注意：函数有奇点/尖点时需手动分段；eps 建议 1e-6 ~ 1e-9
 *       （过小可能递归过深）；f 为普通函数指针（或改 lambda+模板）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * double f(double x) { return sqrt(4 - x * x); }  // 半径 2 的上半圆
 * signed main() {
 *   cout << integrate(f, 0, 2) << '\n';           // 3.14159（= π，圆面积的 1/4）
 *   cout << integrate([](double x) { return x * x * x; }, 0, 1) << '\n';  // 0.25（三次多项式精确）
 * }
 */
