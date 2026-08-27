#include <bits/stdc++.h>
using namespace std;
#define int long long

// floor_sum(n, m, a, b) = Σ_{i=0}^{n-1} floor((a*i + b) / m)
// 约束（与 ACL 一致）：0 <= n < 2^32，1 <= m < 2^32，a、b 可为负
int floor_sum(int n, int m, int a, int b) {
  unsigned long long ans = 0;
  if (a < 0) {  // 负斜率先化正：扣除整块 n(n-1)/2 * ceil(-a/m)
    int a2 = a % m;
    if (a2 < 0) a2 += m;
    ans -= 1ULL * n * (n - 1) / 2 * ((a2 - a) / m);
    a = a2;
  }
  if (b < 0) {
    int b2 = b % m;
    if (b2 < 0) b2 += m;
    ans -= 1ULL * n * ((b2 - b) / m);
    b = b2;
  }
  unsigned long long un = n, um = m, ua = a, ub = b;
  while (true) {  // 类欧几里得辗转：O(log) 轮，规模同 gcd 收敛
    if (ua >= um) {
      ans += un * (un - 1) / 2 * (ua / um);
      ua %= um;
    }
    if (ub >= um) {
      ans += un * (ub / um);
      ub %= um;
    }
    unsigned long long y_max = ua * un + ub;  // 无符号乘法：n,m < 2^32 时安全
    if (y_max < um) break;
    un = y_max / um;
    ub = y_max % um;
    swap(ua, um);  // 横纵互换：转为更小规模的对称问题
  }
  return (int)ans;
}

/*
 * ============================================================
 * 名称：类欧几里得 floor_sum（Σ floor((a*i+b)/m)，对齐 ACL）
 * 复杂度：O(log max(a, m))（每轮至少规模减半，同 gcd 辗转）
 * 用途：经典整点计数：直线 y=(a*x+b)/m 下方（含边界）整点数；
 *       数字取整求和、等差数列的模值分布等一批问题
 * 接口：floor_sum(n, m, a, b) = Σ_{i=0}^{n-1} floor((a*i + b) / m)；
 *       a、b 可为负（ACL 同款语义）；0 <= n < 2^32，1 <= m < 2^32
 * 原理：先把 a、b 化非负（负数部分整块扣除）；随后辗转：
 *       整系数部分 (a/m、b/m) 直接等差计数；剩余仿射部分横纵互换，
 *       把 Σ floor((a*i+b)/m) 化为更小规模（新 n = (a*n+b)/m），
 *       收敛速度同 gcd；内部用无符号 64 位乘法（ACL 同款，n,m<2^32 安全）
 * 来源：AtCoder Library math.hpp floor_sum 移植（算法一致）
 * 注意：i 从 0 起；返回值可能很大（long long）；m 至少为 1
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   cout << floor_sum(4, 2, 3, 1) << '\n';
 *   // i=0..3: floor(1/2)+floor(4/2)+floor(7/2)+floor(10/2) = 0+2+3+5 = 10
 *   cout << floor_sum(10, 7, -3, 5) << '\n';
 *   // i=0..9: 0+0-1-1-1-2-2-3-3-4 = -17（下取整，含负数项）
 *   cout << floor_sum(1, 1, 0, 0) << '\n';   // 0
 *   cout << floor_sum(0, 3, 9, 2) << '\n';   // 0（空和）
 * }
 */
