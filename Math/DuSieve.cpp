#include <bits/stdc++.h>
using namespace std;
#define int long long

// Min-25 style prefix sums of phi / mu via Du's sieve (O(n^{2/3}))
template <size_t SN> struct du_sieve {
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
 * Name: Du's sieve (prefix sums of mu / phi)
 * Complexity: O(n^{2/3})
 * Usage: `du_sieve<SN>`: after one init(), sum_phi(n) = sum_{i<=n} phi(i) and
 *        sum_mu(n) = sum_{i<=n} mu(i), with n up to ~1e10 within 64 bits.
 * Principle: from f * g = h, sum_d g(d) * S_f(n/d) = S_h(n); phi * 1 = Id and
 *            mu * 1 = eps give the two recurrences, division blocking
 *            enumerates floor(n/d), and recursion plus hash memoization reuse
 *            the linear-sieve prefix for small values
 * Notes: SN arrays ~2e6 (two long long arrays, ~32MB, tune as needed); other
 *        multiplicative functions follow the same pattern once you find an easy
 *        h
 * ============================================================
 */
