#define int long long

constexpr int MOD = 998244353;
inline void addm(int &x, int y) { (x += y) >= MOD && (x -= MOD); }
inline void subm(int &x, int y) { (x -= y) < 0 && (x += MOD); }

vector<int> subset_convolution(int n, const vector<int> &f, const vector<int> &g) {
  int m = 1 << n;
  vector<vector<int>> F(n + 1, vector<int>(m)), G(n + 1, vector<int>(m)), H(n + 1, vector<int>(m));
  for (int s = 0; s < m; ++s) {
    F[__builtin_popcount((unsigned)s)][s] = f[s];
    G[__builtin_popcount((unsigned)s)][s] = g[s];
  }
  for (int k = 0; k <= n; ++k)
    for (int i = 0; i < n; ++i)
      for (int s = 0; s < m; ++s)
        if (s >> i & 1) addm(F[k][s], F[k][s ^ (1 << i)]), addm(G[k][s], G[k][s ^ (1 << i)]);
  for (int k = 0; k <= n; ++k)
    for (int j = 0; j <= k; ++j)
      for (int s = 0; s < m; ++s) H[k][s] = (H[k][s] + F[j][s] * G[k - j][s]) % MOD;
  for (int k = 0; k <= n; ++k)
    for (int i = 0; i < n; ++i)
      for (int s = 0; s < m; ++s)
        if (s >> i & 1) subm(H[k][s], H[k][s ^ (1 << i)]);
  vector<int> h(m);
  for (int s = 0; s < m; ++s) h[s] = H[__builtin_popcount((unsigned)s)][s];
  return h;
}

