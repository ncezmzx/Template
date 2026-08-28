#include <bits/stdc++.h>
using namespace std;
#define int long long

// Min-25 style prefix sums of phi / mu via Du's sieve (O(n^{2/3}))
template <size_t SN>
struct du_sieve {
  int phi_[SN], mu_[SN], prime_[SN], pc_;
  bool vis_[SN];
  long long sphi_[SN], smu_[SN];
  unordered_map<long long, long long> mph_, mmu_;
  // linear sieve of phi / mu and their prefix sums
  void init() {
    pc_ = 0;
    memset(vis_, 0, sizeof vis_);
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
  // sum of phi(i) for i <= n; uses phi * 1 = Id
  long long sum_phi(long long n) {
    if (n < SN) return sphi_[n];
    if (mph_.count(n)) return mph_[n];
    long long res = n * (n + 1) / 2;
    for (long long l = 2, r; l <= n; l = r + 1) {
      r = n / (n / l);
      res -= (r - l + 1) * sum_phi(n / l);
    }
    return mph_[n] = res;
  }
  // sum of mu(i) for i <= n; uses mu * 1 = eps
  long long sum_mu(long long n) {
    if (n < SN) return smu_[n];
    if (mmu_.count(n)) return mmu_[n];
    long long res = 1;
    for (long long l = 2, r; l <= n; l = r + 1) {
      r = n / (n / l);
      res -= (r - l + 1) * sum_mu(n / l);
    }
    return mmu_[n] = res;
  }
};

/*
 * ============================================================
 * ============================================================
 * Name: Du's sieve (prefix sums of mu / phi)
 * Complexity: O(n^{2/3}) (pre-sieve SN ~ n^{2/3} + division blocking + hash memoization)
 * Usage: wrapped as du_sieve<SN>: after one init(),
 *        sum_phi(n) = sum_{i<=n} phi(i), sum_mu(n) = sum_{i<=n} mu(i),
 *        n up to ~1e10 within 64 bits
 * Principle: with f * g = h, sum_d g(d) * S_f(n/d) = S_h(n) —
 *        phi * 1 = Id: S_phi(n) = n(n+1)/2 - sum_{d>=2} S_phi(floor(n/d));
 *        mu * 1 = eps: S_mu(n) = 1 - sum_{d>=2} S_mu(floor(n/d));
 *        division blocking enumerates floor(n/d), recursion + hash memo,
 *        small values answered from the linear-sieve prefix
 * Notes: SN arrays ~2e6 (two long long arrays ~ 32MB, tune as needed);
 *        derive other multiplicative functions the same way (find an easy h)
 * ============================================================
 * Example (uncomment to compile):

 * static du_sieve<2000009> ds;
 * signed main() {
 *   ds.init();
 *   cout << ds.sum_phi(10) << '\n';    // 32（1+1+2+2+4+2+6+4+6+4）
 *   cout << ds.sum_mu(10) << '\n';     // -1（1-1-1+0-1+1-1+0+0+1）
 *   cout << ds.sum_phi(10000000000LL) << '\n';  // n = 1e10, answers in ~1s
 * }
 */
