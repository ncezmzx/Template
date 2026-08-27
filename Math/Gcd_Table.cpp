#include <bits/stdc++.h>
using namespace std;

constexpr int K = 11;
int G[1 << K][1 << K];

void init_gcd() {
  for (int i = 0; i < (1 << K); ++i)
    for (int j = 0; j < (1 << K); ++j) G[i][j] = std::gcd(i, j);
}

int gcd_tab(int a, int b) {
  if (!a) return b;
  if (!b) return a;
  int s = __builtin_ctz(a | b);
  a >>= __builtin_ctz(a), b >>= __builtin_ctz(b);
  while (a != b) {
    if (a > b) {
      if (b < (1 << K)) return G[a % b][b] << s;
      a = (a - b) >> __builtin_ctz(a - b);
    } else {
      if (a < (1 << K)) return G[b % a][a] << s;
      b = (b - a) >> __builtin_ctz(b - a);
    }
  }
  return a << s;
}

/*
 * ============================================================
 * 名称：查表法 O(1) GCD（预计算小表 + 二进制 GCD 提前终止）
 * 复杂度：预处理 O(2^K * 2^K)（K=11 时约 4M 次，int 数组内存约 16MB）；查询均摊 O(1)
 * 用途：需要极大量 gcd 计算的场景（如 1e6 次以上）；当较小数落入表内
 *       （< 2^K）时一次查表直接出结果，否则退化为二进制 GCD 的几步迭代
 * 原理：gcd(a, b) = gcd(a mod b, b)（欧几里得）；若 b < 2^K，则 a mod b < b < 2^K，
 *       两者均在表内，一次查表得到答案。配合 ctz 剥离因子 2 保证两侧都是奇数
 * 注意：本实现面向 32 位非负整数（__builtin_ctz）；64 位请改用
 *       Gcd_Binary.cpp（或把 K 调大同时换 __builtin_ctzll，内存随 2^K 指数增长）
 * 用法：init_gcd(); 之后 gcd_tab(a, b) 即为 O(1) 级 gcd
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   init_gcd();
 *   int a, b;
 *   cin >> a >> b;
 *   cout << gcd_tab(a, b) << '\n';
 * }
 * ============================================================
 */
