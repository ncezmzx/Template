#include <bits/stdc++.h>
using namespace std;
#define int long long


template <size_t N> struct km_matching {
  static constexpr int INF = 0x3f3f3f3f3f3f3f3f;
  int n, w[N][N], lx[N], ly[N], match[N], slack[N], pre[N], visx[N], visy[N];
  void augment(int s) {
    for (int i = 1; i <= n; ++i) visx[i] = visy[i] = 0, slack[i] = INF;
    int y = 0, ny = 0;
    match[0] = s;
    while (match[y]) {
      int x = match[y], d = INF;
      visy[y] = 1;
      for (int i = 1; i <= n; ++i)
        if (!visy[i]) {
          if (lx[x] + ly[i] - w[x][i] < slack[i]) slack[i] = lx[x] + ly[i] - w[x][i], pre[i] = y;
          d = min(d, slack[i]);
        }
      for (int i = 0; i <= n; ++i) {
        if (visy[i]) lx[match[i]] -= d, ly[i] += d;
        else slack[i] -= d;
      }
      ny = 0;
      for (int i = 1; i <= n; ++i)
        if (!visy[i] && !slack[i]) ny = i;
      y = ny;
    }
    while (y) match[y] = match[pre[y]], y = pre[y];
  }

  int solve(int n_) {
    n = n_;
    for (int i = 1; i <= n; ++i) lx[i] = -INF, ly[i] = 0, match[i] = 0;
    for (int i = 1; i <= n; ++i)
      for (int j = 1; j <= n; ++j) lx[i] = max(lx[i], w[i][j]);
    for (int i = 1; i <= n; ++i) augment(i);
    int res = 0;
    for (int i = 1; i <= n; ++i) res += w[match[i]][i];
    return res;
  }
};

