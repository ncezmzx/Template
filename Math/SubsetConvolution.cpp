#include <bits/stdc++.h>
using namespace std;
#define int long long

// 子集卷积：h[S] = Σ_{T⊆S} f[T] · g[S\T]（mod MOD）
// 通过"按 popcount 分层 + OR 的 zeta/Möbius 变换"在 O(n² 2^n) 内完成
constexpr int MOD = 998244353;
inline void addm(int& x, int y) { (x += y) >= MOD && (x -= MOD); }
inline void subm(int& x, int y) { (x -= y) < 0 && (x += MOD); }

// f、g 长度为 2^n（下标为子集 bitmask），返回长度 2^n 的子集卷积
vector<int> subset_convolution(int n, const vector<int>& f, const vector<int>& g) {
  int m = 1 << n;
  vector<vector<int>> F(n + 1, vector<int>(m)), G(n + 1, vector<int>(m)), H(n + 1, vector<int>(m));
  for (int s = 0; s < m; ++s) {
    F[__builtin_popcount((unsigned)s)][s] = f[s];
    G[__builtin_popcount((unsigned)s)][s] = g[s];
  }
  for (int k = 0; k <= n; ++k)  // OR 的 zeta 变换（子集和）
    for (int i = 0; i < n; ++i)
      for (int s = 0; s < m; ++s)
        if (s >> i & 1) addm(F[k][s], F[k][s ^ (1 << i)]), addm(G[k][s], G[k][s ^ (1 << i)]);
  for (int k = 0; k <= n; ++k)
    for (int j = 0; j <= k; ++j)
      for (int s = 0; s < m; ++s)
        H[k][s] = (H[k][s] + F[j][s] * G[k - j][s]) % MOD;
  for (int k = 0; k <= n; ++k)  // OR 的 Möbius 反演
    for (int i = 0; i < n; ++i)
      for (int s = 0; s < m; ++s)
        if (s >> i & 1) subm(H[k][s], H[k][s ^ (1 << i)]);
  vector<int> h(m);
  for (int s = 0; s < m; ++s) h[s] = H[__builtin_popcount((unsigned)s)][s];
  return h;
}

/*
 * ============================================================
 * 名称：子集卷积（Subset Convolution）
 * 复杂度：O(n² 2^n)（n = log₂ 长度；朴素为 O(3^n)）
 * 用途：h[S] = Σ_{T⊆S} f[T]g[S\T]（要求 |T|+|S\T|=|S| 的限制），常见于
 *       集合幂级数、集合划分计数、树上集合 DP 合并等
 * 接口：subset_convolution(n, f, g)，f/g 长度 2^n，返回 2^n 的 h
 * 原理：对每个 popcount 分层得 F[k][S]（只保留 |S|=k 的项），各层做 OR
 *       zeta 变换后按"层卷积"（对每个 S 做一元多项式乘法），再 Möbius
 *       反演，最后取回 H[popcount(S)][S]
 * 注意：结果对 MOD 取模；f/g 须为 2 的幂长度；MOD 需为质数（乘法只取模）
 * 来源：OI-Wiki《集合幂级数 / 子集卷积》（https://oi-wiki.org/math/poly/sps/）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   int n = 2;
 *   vector<int> f{1, 2, 3, 4}, g{5, 6, 7, 8};  // 下标 0..3 为子集 0,1,2,3
 *   auto h = subset_convolution(n, f, g);
 *   for (int x : h) cout << x << ' ';  // h[3] = f[0]g[3]+f[1]g[2]+f[2]g[1]+f[3]g[0]
 *   cout << '\n';
 * }
 * ============================================================
 */
