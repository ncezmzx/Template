#include <bits/stdc++.h>
using namespace std;
#define int long long
using d64 = long double;


template <size_t N> struct slope_opt_wqs {
  constexpr static d64 eps = 1e-9;
  int n, k, a[N], s[N], f[N], g[N], q[N], hd, tl;
  d64 x(int i) { return s[i]; }
  d64 y(int i) { return f[i] + s[i] * s[i]; }
  d64 slope(int i, int j) { return (y(j) - y(i)) / (x(j) - x(i)); }


  pair<int, int> calc(int x) {
    for (int i = 0; i <= n; ++i) f[i] = g[i] = 0;
    hd = tl = 1, q[1] = 0;
    for (int i = 1; i <= n; ++i) {
      while (hd < tl && slope(q[hd], q[hd + 1]) < eps + 2 * (s[i] + 1)) ++hd;
      int j = q[hd];
      g[i] = g[j] + 1, f[i] = f[j] + (s[i] - s[j] + 1) * (s[i] - s[j] + 1) - x;
      while (hd < tl && slope(q[tl - 1], q[tl]) + eps > slope(q[tl], i)) --tl;
      q[++tl] = i;
    }
    return {f[n], g[n]};
  }

  int solve() {
    int l = -2e16, r = 2e16;
    while (l < r) {
      int m = (l + r) >> 1;
      if (calc(m).second >= k) r = m;
      else l = m + 1;
    }
    return calc(l).first + l * k;
  }
};

