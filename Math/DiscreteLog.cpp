#include <bits/stdc++.h>
using namespace std;
#define int long long

int pw(int x, int n, int m) {
  int r = 1 % m;
  x %= m;
  if (x < 0) x += m;
  while (n) {
    if (n & 1) r = r * x % m;
    x = x * x % m, n >>= 1;
  }
  return r;
}
int exgcd(int a, int b, int& x, int& y) {
  if (!b) return x = 1, y = 0, a;
  int g = exgcd(b, a % b, y, x);
  y -= a / b * x;
  return g;
}
int inv_mod(int a, int m) {  // 需 gcd(a, m) = 1
  int x, y;
  exgcd(a, m, x, y);
  return (x % m + m) % m;
}

// BSGS：最小 x >= 0 使 a^x ≡ b (mod m)，需 gcd(a, m) = 1；无解返回 -1；O(√m)
int bsgs(int a, int b, int m) {
  if (m == 1) return 0;
  a %= m, b %= m;
  int k = (int)sqrtl(m) + 1;
  unordered_map<int, int> mp;
  mp.reserve(2 * k);
  int cur = 1;
  for (int j = 0; j < k; ++j) {  // baby：a^j（存最小 j，保证解最小）
    if (!mp.count(cur)) mp[cur] = j;
    cur = cur * a % m;
  }
  int ainvk = pw(inv_mod(a, m), k, m);  // a^{-k}
  cur = b;
  for (int i = 0; i <= k; ++i) {  // giant：b·a^{-ik}，命中 a^j → x = ik + j
    auto it = mp.find(cur);
    if (it != mp.end()) return i * k + it->second;
    cur = cur * ainvk % m;
  }
  return -1;
}

// exBSGS：任意 gcd(a, m)；无解返回 -1
// 每轮 a^x ≡ b (mod m) 且 g = gcd(a, m) > 1 时要求 g | b，
// 两边除 g：(a/g)·a^{x-1} ≡ b/g (mod m/g)；累计 d = ∏(a/g)，
// 循环结束（gcd(a, m) = 1）后解 a^t ≡ b·d^{-1}，x = t + cnt
int exbsgs(int a, int b, int m) {
  if (m == 1) return 0;
  a %= m, b %= m;
  if (b == 1 % m) return 0;  // a^0 = 1
  int cnt = 0, d = 1;
  for (int g = __gcd(a, m); g > 1; g = __gcd(a, m)) {
    if (b % g) return -1;
    ++cnt;
    b /= g, m /= g;
    d = d * (a / g) % m;
    if (d == b) return cnt;  // a^{cnt} ≡ b (mod 原 m)
  }
  int r = bsgs(a, b * inv_mod(d, m) % m, m);
  return r < 0 ? -1 : r + cnt;
}

/*
 * ============================================================
 * 名称：BSGS / exBSGS（离散对数）
 * 复杂度：BSGS O(√m)；exBSGS O(√m + log²m)
 * 用途：求最小 x ≥ 0 使 a^x ≡ b (mod m)：
 *       bsgs(a, b, m) 要求 gcd(a, m) = 1；
 *       exbsgs(a, b, m) 无限制（逐步剥 gcd 后归约到 BSGS）
 * 原理：x = i·k + j（k = ⌈√m⌉）：b·a^{-ik} = a^j——baby 步把 a^j
 *       （存最小 j）放入哈希表，giant 步枚举 i 乘 a^{-k} 首个命中
 *       即最小解；exBSGS 每轮把两边除以 g = gcd(a, m)（要求 g | b），
 *       方程变为 (a/g)·a^{x-1} ≡ b/g (mod m/g)，系数累积进 d，
 *       d == b 时 x = cnt 提前命中；结束后 a 与 m 互素，解
 *       a^t ≡ b·d^{-1} (mod m) 得 x = t + cnt
 * 注意：m = 1 或 b ≡ 1 返回 0；解的上界 < m（阶 ≤ m）；
 *       与 Math/CRT.cpp 的 exgcd 重复（本文件自包含）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   cout << bsgs(3, 1, 7) << '\n';      // 0（3^0 = 1）
 *   cout << bsgs(3, 6, 7) << '\n';      // 3（3^3 = 27 ≡ 6）
 *   cout << bsgs(2, 3, 7) << '\n';      // -1（2^x mod 7 ∈ {1,2,4}）
 *   cout << exbsgs(2, 8, 16) << '\n';   // 3（2^3 = 8）
 *   cout << exbsgs(4, 2, 12) << '\n';   // -1（4^x mod 12 ∈ {4, 4, ...}）
 * }
 */
