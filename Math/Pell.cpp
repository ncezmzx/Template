#include <bits/stdc++.h>
using namespace std;
#define int long long

// Pell 方程 x² - D y² = 1 的最小正整数解（基本解），连分数法
long long isqrt(long long x) {
  long long r = (long long)sqrtl((long double)x);
  while ((r + 1) * (r + 1) <= x) ++r;
  while (r * r > x) --r;
  return r;
}
// D 为正且非完全平方数；返回 (x1, y1) 基本解
pair<long long, long long> pell(long long D) {
  long long a0 = isqrt(D);
  vector<long long> A;  // A[i] = 第 i+1 个部分商 a_{i+1}（周期长为 L = A.size()）
  long long m = 0, d = 1, a = a0;
  do {  // √D 的连分数周期在 a == 2a0 处结束
    m = d * a - m;
    d = (D - m * m) / d;
    a = (a0 + m) / d;
    A.push_back(a);
  } while (a != 2 * a0);
  long long L = A.size();
  // 收敛分数 p_i/q_i（p_{-2}=0,p_{-1}=1；q_{-2}=1,q_{-1}=0），用 __int128 防中间溢出
  auto conv = [&](long long upto) {
    long long p = a0, q = 1;      // p_0, q_0
    long long p1 = 1, q1 = 0;     // p_{-1}, q_{-1}
    for (long long i = 1; i <= upto; ++i) {
      long long ai = A[(i - 1) % L];
      __int128 pn = (__int128)ai * p + p1, qn = (__int128)ai * q + q1;
      p1 = p, p = (long long)pn, q1 = q, q = (long long)qn;
    }
    return make_pair(p, q);
  };
  // 周期 L 偶：基本解 (p_{L-1}, q_{L-1})；L 奇：需第 2L-1 个收敛分数
  return L % 2 == 0 ? conv(L - 1) : conv(2 * L - 1);
}

/*
 * ============================================================
 * 名称：Pell 方程（x² - D y² = 1 的基本解）
 * 复杂度：O(√D)（连分数周期 ≤ 2√D·log；收敛分数 O(L)）
 * 用途：求 x² - D y² = 1 的最小正整数解 (x1, y1)（基本解）；其全部
 *       正整数解为 (x1 + y1√D)^k。负 Pell（x² - D y² = -1）有解当且仅当
 *       √D 的连分数周期为奇数，此时基本解为 (p_{L-1}, q_{L-1})
 * 接口：pell(D) → pair(x1, y1)。要求 D 为正且非完全平方数
 * 原理：√D 的连分数是周期的，基本解来自其收敛分数：周期 L 为偶取
 *       (p_{L-1}, q_{L-1})，为奇取 (p_{2L-1}, q_{2L-1})
 * 注意：中间乘积用 __int128；若 D 太大导致基本解超出 long long（如
 *       D=1021 有 74 位），需换高精度；D 为完全平方数时无意义
 * 来源：OI-Wiki《Pell 方程》（https://oi-wiki.org/math/number-theory/pell-equation/）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   auto [x, y] = pell(61);  // 1766319049² - 61·226153980² = 1
 *   cout << x << ' ' << y << '\n';
 *   auto [x2, y2] = pell(2); // 3² - 2·2² = 1
 *   cout << x2 << ' ' << y2 << '\n';
 * }
 * ============================================================
 */
