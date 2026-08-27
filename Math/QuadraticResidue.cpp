#include <bits/stdc++.h>
using namespace std;
#define int long long

int pw(int x, int n, int p) {
  int r = 1 % p;
  x %= p;
  while (n) {
    if (n & 1) r = r * x % p;
    x = x * x % p, n >>= 1;
  }
  return r;
}

// Cipolla：解 x² ≡ a (mod p)，p 为奇素数；返回较小根，无解返回 -1；期望 O(log p)
struct cpx {  // F_p 上的"复数" x + y√w
  int x, y;
};
int W;  // √w 的平方（w = b² - a 非二次剩余）
cpx cmul(const cpx& a, const cpx& b, int p) {
  return {(a.x * b.x + a.y * b.y % p * W) % p, (a.x * b.y + a.y * b.x) % p};
}
int sqrt_mod(int a, int p) {
  a %= p;
  if (a == 0) return 0;
  if (pw(a, (p - 1) / 2, p) != 1) return -1;  // Euler 准则判二次剩余
  int b;
  for (b = 1;; ++b) {  // 找 w = b² - a 为非二次剩余（期望 2 次）
    W = ((b * b - a) % p + p) % p;
    if (W && pw(W, (p - 1) / 2, p) == p - 1) break;
  }
  cpx r{1, 0}, c{b, 1};
  for (int e = (p + 1) / 2; e; e >>= 1) {  // (b + √w)^{(p+1)/2}，虚部必为 0
    if (e & 1) r = cmul(r, c, p);
    c = cmul(c, c, p);
  }
  return min(r.x, p - r.x);
}

/*
 * ============================================================
 * 名称：二次剩余（Cipolla 开平方 mod p）
 * 复杂度：期望 O(log²p)（找 b 期望 2 次尝试）
 * 用途：sqrt_mod(a, p) 求 x 使 x² ≡ a (mod p)（p 奇素数），
 *       返回两根中较小者；a 非二次剩余返回 -1；a ≡ 0 返回 0
 * 原理：Euler 准则 a^{(p-1)/2} ≡ 1 判定可开方；随机/递增取 b 使
 *       w = b² - a 为非二次剩余，则在 F_p(√w)（p² 阶域）中
 *       (b + √w)^p = b - √w（Frobenius），故 (b+√w)^{p+1} = b² - w = a，
 *       其 (p+1)/2 次幂即为所求根（虚部为 0）
 * 注意：仅适用于奇素数 p（p = 2 时 a mod 2 即根）；
 *       两根为 x 与 p - x；0 的根仅 0
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   cout << sqrt_mod(2, 7) << '\n';          // 3（3² = 9 ≡ 2，另一根 4）
 *   cout << sqrt_mod(3, 7) << '\n';          // -1（3 非二次剩余 mod 7）
 *   cout << sqrt_mod(4, 998244353) << '\n';  // 2
 * }
 */
