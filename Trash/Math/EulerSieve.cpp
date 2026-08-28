#include <bits/stdc++.h>
using namespace std;
#define int long long

// linear (Euler) sieve: primes + Euler totient in O(n)
template <size_t N>
struct euler_sieve {
  int phi[N], pri[N], c, vst[N];
  void run(int n) {
    phi[1] = 1;
    for (int i = 2; i <= n; ++i) {
      if (!vst[i]) pri[++c] = i, phi[i] = i - 1;
      for (int j = 1; j <= c && pri[j] * i <= n; ++j) {
        vst[i * pri[j]] = 1;
        if (i % pri[j] == 0) {
          phi[i * pri[j]] = pri[j] * phi[i];
          break;
        }
        phi[i * pri[j]] = phi[pri[j]] * phi[i];
      }
    }
  }
};
// phi prefix sums if the problem needs them:
// for (int i = 1; i <= n; ++i) phi[i] += phi[i - 1];

/*
 * ============================================================
 * Name: linear sieve + Euler totient (primes and phi simultaneously, O(n))
 * Complexity: O(n)
 * Usage: wrapped as euler_sieve<N>: run(n) fills pri[1..c] (all primes <= n)
 *        and phi[i] (count of integers <= i coprime to i)
 * Source: all.cpp 25674-25711 (originally inside main; extracted into
 *         run(n), loop bound parameterized from i < N to i <= n; the
 *         problem-specific prefix-sum line moved to a comment)
 * ============================================================
 * Example (uncomment to compile):
 * static euler_sieve<100009> es;
 * signed main() {
 *   es.run(100000);
 *   cout << es.c << '\n';  // number of primes in 1..1e5
 *   cout << es.phi[100] << ' ' << es.phi[97] << '\n';  // 40 96
 * }
 * ============================================================
 */
