#include <bits/stdc++.h>
using namespace std;
#define int long long


struct digit_dp_binary {
  int k, dp[100][100][2], c, a[100];


  int dfs(int p, int q, int top) {
    if (q < 0) return 0;
    if (!p) return q == 0;
    auto &ret = dp[p][q][top];
    if (~ret) return ret;
    ret = 0;
    for (int i = 0, up = top ? a[p] : 1; i <= up; ++i) ret += dfs(p - 1, q - i, top && i == up);
    return ret;
  }
  int sol(int x) {
    memset(dp, -1, sizeof dp);
    for (c = 0; x; x >>= 1) a[++c] = x & 1;
    return dfs(c, k, 1);
  }
};

