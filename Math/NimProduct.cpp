#include <bits/stdc++.h>
using namespace std;

// Nim 积（nimber multiplication）：特征 2 域上的乘法，与 Nim 和（异或）配成域
// 支持 64 位 nimber；按"最大费马 2 幂 F(k)=2^{2^k}"分治，递归深度 ≤ 5
using ull = unsigned long long;

ull nim_prod(ull a, ull b) {
  if (a < b) swap(a, b);
  if (b == 0) return 0;
  if (b == 1) return a;
  int k = 0;
  while (k < 5 && (1ULL << (1 << (k + 1))) <= a) ++k;  // 最大 k 使 F(k) ≤ a < F(k+1)
  ull m = 1ULL << (1 << k);                            // m = F(k)
  ull a1 = a >> (1 << k), a0 = a & (m - 1);            // a = a1·m ⊕ a0
  ull b1 = b >> (1 << k), b0 = b & (m - 1);
  ull a1b1 = nim_prod(a1, b1);
  ull a0b0 = nim_prod(a0, b0);
  ull t = nim_prod(a1 ^ a0, b1 ^ b0) ^ a1b1 ^ a0b0;    // t = a1⊗b0 ⊕ a0⊗b1
  // a⊗b = a0b0 ⊕ (t ⊗ m) ⊕ (a1b1 ⊗ (m ⊗ m))，其中 m⊗m = m ⊕ (m>>1)（=3m/2）
  return a0b0 ^ (t << (1 << k)) ^ (a1b1 << (1 << k)) ^ nim_prod(a1b1, m >> 1);
}

/*
 * ============================================================
 * 名称：Nim 积（Nimber Multiplication）
 * 复杂度：O(log² 64) 级递归（深度 ≤ 5，每层常数次分治）
 * 用途：多个独立"翻硬币类"子游戏的 SG 值相乘（二维 Nim、Coin Turning
 *       游戏乘积）；nimber 构成特征 2 的域，配 XOR 的 Nim 和可做任意
 *       域运算（含求逆：x ⊗ x^{-1} = 1）
 * 接口：nim_prod(a, b)，a、b 为 64 位无符号 nimber
 * 原理：设 m = F(k) = 2^{2^k} 为不超过 max(a,b) 的最大费马 2 幂，把
 *       a = a1·m ⊕ a0、b = b1·m ⊕ b0（各部分 < m）。利用分配律与两个
 *       关键性质：① m ⊗ z = m·z（z < m 时即普通移位）；② m⊗m = 3m/2
 *       = m ⊕ (m>>1)。得 a⊗b = a0b0 ⊕ (a1b0⊕a0b1)m ⊕ a1b1·(m⊕m>>1)，
 *       其中各项按更小的 k 递归
 * 注意：输入需 < 2^64；Nim 和 = 按位异或（^）；求逆可用 nimber 域内
 *       快速幂（a^{-1} = a^{2^{64}-2}）
 * 来源：OI-Wiki《公平组合游戏 · Nim 数》
 *       （https://oi-wiki.org/math/game-theory/impartial-game/）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   cout << nim_prod(2, 2) << '\n';  // 3
 *   cout << nim_prod(3, 3) << '\n';  // 2
 *   cout << nim_prod(4, 4) << '\n';  // 6
 *   cout << nim_prod(2, 4) << '\n';  // 8（不同费马 2 幂 = 普通积）
 * }
 * ============================================================
 */
