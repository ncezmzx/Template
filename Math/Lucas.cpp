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

// ---------- Lucas：C(n, m) mod p（p 为素数，p <= 1e6 量级；先 init_lucas(p) 建表）----------
vector<int> fac_, ifac_;
void init_lucas(int p) {
  fac_.assign(p, 1), ifac_.assign(p, 1);
  for (int i = 1; i < p; ++i) fac_[i] = fac_[i - 1] * i % p;
  ifac_[p - 1] = pw(fac_[p - 1], p - 2, p);
  for (int i = p - 1; i; --i) ifac_[i - 1] = ifac_[i] * i % p;
}
int C_small(int n, int m, int p) {  // 0 <= n, m < p
  if (m < 0 || m > n) return 0;
  return fac_[n] * ifac_[m] % p * ifac_[n - m] % p;
}
int lucas(int n, int m, int p) {  // p 素数；n, m 可达 1e18
  if (m < 0 || m > n) return 0;
  if (m == 0) return 1;
  return C_small(n % p, m % p, p) * lucas(n / p, m / p, p) % p;
}

// ---------- exLucas：C(n, m) mod P（P 任意正整数）----------
int fact_pe(int n, int p, int pe) {  // n! 剥去全部 p 因子后 mod pe
  if (!n) return 1;
  int res = 1;
  for (int i = 1; i <= pe; ++i)  // [1, pe] 一整块中非 p 倍数之积
    if (i % p) res = res * i % pe;
  res = pw(res, n / pe, pe);
  for (int i = 1; i <= n % pe; ++i)  // 尾部零头
    if (i % p) res = res * i % pe;
  return res * fact_pe(n / p, p, pe) % pe;  // p 倍数部分除 p 递归
}
int C_mod_pe(int n, int m, int p, int pe, int e) {  // C(n, m) mod p^e
  if (m < 0 || m > n) return 0;
  int k = 0;  // C(n, m) 中 p 的幂次（Kummer / Legendre 公式）
  int a = n, b = m, c = n - m;
  while (a) a /= p, b /= p, c /= p, k += a - b - c;
  if (k >= e) return 0;
  int r = fact_pe(n, p, pe) * inv_mod(fact_pe(m, p, pe), pe) % pe;
  r = r * inv_mod(fact_pe(n - m, p, pe), pe) % pe;
  return r * pw(p, k, pe) % pe;
}
int exlucas(int n, int m, int P) {  // C(n, m) mod P；P 分解为素数幂后 CRT 合并
  if (m < 0 || m > n) return 0;
  int r = 0, mod = 1;
  int PP = P;
  for (int p = 2; p * p <= PP; ++p)
    if (PP % p == 0) {
      int e = 0, pe = 1;
      while (PP % p == 0) PP /= p, ++e, pe *= p;
      int cr = C_mod_pe(n, m, p, pe, e), t = (cr - r) % pe;  // CRT 合并（互素）
      if (t < 0) t += pe;
      r += mod * (t * inv_mod(mod % pe, pe) % pe), mod *= pe;
    }
  if (PP > 1) {  // 剩余大素数因子
    int cr = C_mod_pe(n, m, PP, PP, 1), t = (cr - r) % PP;
    if (t < 0) t += PP;
    r += mod * (t * inv_mod(mod % PP, PP) % PP), mod *= PP;
  }
  return r;
}

/*
 * ============================================================
 * 名称：Lucas / exLucas（大组合数取模）
 * 复杂度：Lucas O(log_p n)（建表 O(p)）；exLucas O(√P + Σ p^e·log_p n)
 * 用途：lucas(n, m, p)：C(n, m) mod p，p 为素数（n, m 可达 1e18），
 *       使用前 init_lucas(p) 建阶乘表（p <= 1e6 量级）；
 *       exlucas(n, m, P)：P 任意正整数（素数幂分解 + CRT 合并）
 * 原理：Lucas 定理 C(n,m) ≡ C(n/p, m/p)·C(n%p, m%p) (mod p)，
 *       按 p 进制逐位展开；exLucas 对每个素数幂 p^e 用"阶乘剥 p
 *       因子"（fact_pe 按循环节 [1, p^e] 的非 p 倍数之积 + 递归）
 *       求出 C 的 p-free 部分再乘回 p^k（k 为 Kummer 进位数），
 *       各素数幂结果用 CRT 合并
 * 注意：exLucas 的 P 建议 <= 1e9（内部 mod * pe 累乘需不溢出 ll）；
 *       与 Math/CRT.cpp 的 exgcd/inv_mod 重复（本文件自包含）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   init_lucas(7);
 *   cout << lucas(10, 3, 7) << '\n';        // 1（C(10,3)=120 = 17×7+1）
 *   cout << lucas(25, 12, 7) << '\n';       // 0（25=34₇, 12=15₇，C(4,5)=0 → 0）
 *   cout << exlucas(10, 3, 12) << '\n';     // 0（C(10,3)=120 ≡ 0 mod 12）
 *   cout << exlucas(10, 2, 12) << '\n';     // 9（C(10,2)=45 = 3×12+9）
 * }
 */
