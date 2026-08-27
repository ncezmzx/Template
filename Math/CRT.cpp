#include <bits/stdc++.h>
using namespace std;
#define int long long

// 扩展欧几里得：求 x, y 使 a*x + b*y = gcd(a, b)（x 取最小非负）
int exgcd(int a, int b, int& x, int& y) {
  if (!b) return x = 1, y = 0, a;
  int g = exgcd(b, a % b, y, x);
  y -= a / b * x;
  return g;
}
// 返回 (g, x)：g = gcd(a, b)，a*x ≡ g (mod b)，0 <= x < b（b != 0）
pair<int, int> inv_gcd(int a, int b) {
  a %= b;
  if (a < 0) a += b;
  int x, y, g = exgcd(a, b, x, y);
  if (g != 1) x = (x % (b / g) + (b / g)) % (b / g);  // 通解的最小非负 x（g 整除意义下）
  else x = (x % b + b) % b;
  return {g, x};
}
int safe_mod(int a, int m) { a %= m; return a < 0 ? a + m : a; }

// 中国剩余定理（可合并非互质模数，对齐 ACL crt）：
// 解 x ≡ r[i] (mod m[i])；返回 {r, m} 表示 x ≡ r (mod m)，且 0 <= r < m = lcm；
// 无解返回 {0, 0}
pair<int, int> crt(vector<int> r, vector<int> m) {
  int r0 = 0, m0 = 1;  // 已合并部分：x ≡ r0 (mod m0)
  for (int i = 0; i < (int)r.size(); i++) {
    int r1 = safe_mod(r[i], m[i]), m1 = m[i];
    if (m0 < m1) swap(r0, r1), swap(m0, m1);
    if (m0 % m1 == 0) {
      if (r0 % m1 != r1) return {0, 0};
      continue;
    }
    // 合并：x = r0 + t*m0 ≡ r1 (mod m1) -> t*m0 ≡ r1-r0 (mod m1)
    int g, im;
    tie(g, im) = inv_gcd(m0, m1);
    if ((r1 - r0) % g) return {0, 0};
    int u1 = m1 / g;
    int t = (r1 - r0) / g % u1 * im % u1;  // 0 <= t < u1
    r0 += t * m0;                          // |r0| < m0 + m0*(u1-1) < lcm
    m0 *= u1;                              // m0 = lcm(m0, m1)
    if (r0 < 0) r0 += m0;
  }
  return {r0, m0};
}

/*
 * ============================================================
 * 名称：中国剩余定理 CRT（非互质模数可合并，对齐 ACL crt）
 * 复杂度：单次合并 O(log)（exgcd），总计 O(k log M)，k 为方程数
 * 用途：解同余方程组 x ≡ r[i] (mod m[i])（模数不必互质）：
 *       返回 {r, m}：唯一解 x ≡ r (mod lcm(m))，0 <= r < lcm；
 *       无解返回 {0, 0}；亦即 exCRT 的向量化封装
 * 原理：逐个合并 (r0, m0) 与 (r1, m1)：设 g = gcd(m0, m1)，
 *       (r1-r0) 必须被 g 整除；否则无解；
 *       否则 t ≡ (r1-r0)/g * inv(m0/g) (mod m1/g)，
 *       新解 r0 += t*m0，新模 lcm = m0*m1/g（全程不超 long long）
 * 接口：crt(r[], m[])（等长数组）；exgcd / inv_gcd 可单独复用
 * 来源：AtCoder Library math.hpp crt 移植（算法一致）
 * 注意：需保证 1 <= m[i]；r[i] 可为负（内部 safe_mod）；
 *       lcm 需不爆 long long（ACL 同约束）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   // 物不知数：x%3==2, x%5==3, x%7==2 → x ≡ 23 (mod 105)
 *   auto res = crt({2, 3, 2}, {3, 5, 7});
 *   cout << res.first << ' ' << res.second << '\n';   // 23 105
 *   // 非互质有解：x ≡ 3 (mod 4), x ≡ 5 (mod 6) → x ≡ 11 (mod 12)
 *   auto sol = crt({3, 5}, {4, 6});
 *   cout << sol.first << ' ' << sol.second << '\n';   // 11 12
 *   // 非互质无解：x ≡ 1 (mod 4) 与 x ≡ 2 (mod 6) 奇偶矛盾
 *   auto bad = crt({1, 2}, {4, 6});
 *   cout << bad.first << ' ' << bad.second << '\n';   // 0 0
 *   // 非互质有解：x ≡ 2 (mod 4), x ≡ 4 (mod 10) → x ≡ 14 (mod 20)
 *   auto ok = crt({2, 4}, {4, 10});
 *   cout << ok.first << ' ' << ok.second << '\n';     // 14 20
 * }
 */
