#include <bits/stdc++.h>
using namespace std;
#define int long long


template <size_t N> struct euler_sieve {
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

