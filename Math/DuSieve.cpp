#include <bits/stdc++.h>
using namespace std;
#define int long long

// 杜教筛：积性函数前缀和 S(n) = Σ_{i=1..n} f(i)，利用 f = g * h 且 g、h 前缀和易算：
// S_f(n) = (S_{g*h}(n) - Σ_{d>=2} S_h(d)·S_f(n/d)) / h(1)... 本文件给 μ 与 φ 的现成实现
constexpr int SN = 2e6 + 9;  // 预筛上限（取 n^{2/3} 量级最佳，按需调整）
int phi_[SN], mu_[SN], prime_[SN], pc_;
bool vis_[SN];
long long sphi_[SN], smu_[SN];
unordered_map<long long, long long> mph_, mmu_;
void sieve_init() {  // 线性筛 phi / mu 及其前缀和
  phi_[1] = mu_[1] = 1;
  for (int i = 2; i < SN; ++i) {
    if (!vis_[i]) prime_[++pc_] = i, phi_[i] = i - 1, mu_[i] = -1;
    for (int j = 1; j <= pc_ && i * prime_[j] < SN; ++j) {
      vis_[i * prime_[j]] = true;
      if (i % prime_[j] == 0) {
        phi_[i * prime_[j]] = phi_[i] * prime_[j], mu_[i * prime_[j]] = 0;
        break;
      }
      phi_[i * prime_[j]] = phi_[i] * (prime_[j] - 1), mu_[i * prime_[j]] = -mu_[i];
    }
  }
  for (int i = 1; i < SN; ++i) sphi_[i] = sphi_[i - 1] + phi_[i], smu_[i] = smu_[i - 1] + mu_[i];
}
long long sum_phi(long long n) {  // Σ_{i=1..n} φ(i)：φ * 1 = Id → S(n) = n(n+1)/2 - Σ_{d>=2} S(n/d)
  if (n < SN) return sphi_[n];
  if (mph_.count(n)) return mph_[n];
  long long res = n * (n + 1) / 2;
  for (long long l = 2, r; l <= n; l = r + 1) {
    r = n / (n / l);
    res -= (r - l + 1) * sum_phi(n / l);
  }
  return mph_[n] = res;
}
long long sum_mu(long long n) {  // Σ_{i=1..n} μ(i)：μ * 1 = ε → S(n) = 1 - Σ_{d>=2} S(n/d)
  if (n < SN) return smu_[n];
  if (mmu_.count(n)) return mmu_[n];
  long long res = 1;
  for (long long l = 2, r; l <= n; l = r + 1) {
    r = n / (n / l);
    res -= (r - l + 1) * sum_mu(n / l);
  }
  return mmu_[n] = res;
}

/*
 * ============================================================
 * 名称：杜教筛（μ / φ 前缀和）
 * 复杂度：O(n^{2/3})（预筛 SN = n^{2/3} + 数论分块 + 哈希记忆化）
 * 用途：sum_phi(n) = Σ_{i<=n} φ(i)，sum_mu(n) = Σ_{i<=n} μ(i)，
 *       n 可达 1e10 级（64 位内）；使用前 sieve_init() 一次
 * 原理：f * g = h 时 Σ_{d} g(d)·S_f(n/d) = S_h(n)——
 *       φ * 1 = Id：S_φ(n) = n(n+1)/2 - Σ_{d>=2} S_φ(⌊n/d⌋)；
 *       μ * 1 = ε：S_μ(n) = 1 - Σ_{d>=2} S_μ(⌊n/d⌋)；
 *       数论分块枚举 ⌊n/d⌋，递归 + 哈希记忆化，小值走线性筛前缀
 * 注意：SN 内数组约 2e6（long long ×2 ≈ 32MB，按需调整）；
 *       需要其他积性函数时仿照推导（找易算卷积 h）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   sieve_init();
 *   cout << sum_phi(10) << '\n';    // 32（1+1+2+2+4+2+6+4+6+4）
 *   cout << sum_mu(10) << '\n';     // -1（1-1-1+0-1+1-1+0+0+1）
 *   cout << sum_phi(10000000000LL) << '\n';  // n = 1e10 秒级出值
 * }
 */
