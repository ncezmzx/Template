#include <bits/stdc++.h>
using namespace std;
#define int long long

// Matrix-Tree 定理：生成树计数（无向图 / 有向外向树），模 998244353
constexpr long long MOD = 998244353;
int pw(int x, int n, int p) {
  int r = 1 % p;
  x %= p;
  while (n) {
    if (n & 1) r = r * x % p;
    x = x * x % p, n >>= 1;
  }
  return r;
}
// 高斯消元行列式（mod 素数）；a 为 n×n（会被改写）
int det_mod(vector<vector<int>>& a, int n) {
  int res = 1;
  for (int i = 0; i < n; ++i) {
    int k = -1;
    for (int r = i; r < n; ++r)
      if (a[r][i]) {
        k = r;
        break;
      }
    if (k < 0) return 0;
    if (k != i) swap(a[i], a[k]), res = (MOD - res) % MOD;  // 交换变号
    res = res * a[i][i] % MOD;
    int iv = pw(a[i][i], MOD - 2, MOD);
    for (int r = i + 1; r < n; ++r) {
      if (!a[r][i]) continue;
      int f = a[r][i] * iv % MOD;
      for (int c = i; c < n; ++c) a[r][c] = (a[r][c] - f * a[i][c] % MOD + MOD) % MOD;
    }
  }
  return res;
}
// 无向图（重边允许，自环忽略）的生成树个数
int count_spanning(int n, const vector<pair<int, int>>& edges) {
  if (n <= 1) return 1;
  vector<vector<int>> L(n - 1, vector<int>(n - 1, 0));
  for (auto& e : edges) {
    int u = e.first, v = e.second;
    if (u == v) continue;
    if (u < n - 1) L[u][u]++;
    if (v < n - 1) L[v][v]++;
    if (u < n - 1 && v < n - 1) L[u][v] = (L[u][v] + MOD - 1) % MOD, L[v][u] = (L[v][u] + MOD - 1) % MOD;
  }
  return det_mod(L, n - 1);
}
// 有向图以 root 为根的外向树（父 → 子，每个非根恰一条入边）个数
int count_arborescence(int n, const vector<pair<int, int>>& edges, int root) {
  if (n <= 1) return 1;
  vector<vector<int>> L(n, vector<int>(n, 0));  // 入度 Laplacian
  vector<int> idx;
  for (int i = 0; i < n; ++i)
    if (i != root) idx.push_back(i);
  for (auto& e : edges) {
    int u = e.first, v = e.second;  // 边 u → v：v 的入边
    if (v == root || u == v) continue;
    L[v][v]++;
    L[v][u] = (L[v][u] + MOD - 1) % MOD;
  }
  vector<vector<int>> A(idx.size(), vector<int>(idx.size()));
  for (int i = 0; i < (int)idx.size(); ++i)
    for (int j = 0; j < (int)idx.size(); ++j) A[i][j] = L[idx[i]][idx[j]];
  return det_mod(A, (int)idx.size());
}

/*
 * ============================================================
 * 名称：Matrix-Tree 定理（生成树计数）
 * 复杂度：O(n³)（行列式消元）
 * 用途：count_spanning(n, edges)：无向图（含重边）生成树个数；
 *       count_arborescence(n, edges, root)：有向图以 root 为根的
 *       外向树个数（内向树把边反向后即是）；均模 998244353
 * 原理：无向图：Laplacian L = D - A 去任意一行一列的行列式 =
 *       生成树个数（Kirchhoff）；有向外向树：入度 Laplacian
 *       （L[v][v] = indeg(v)，边 u→v 贡献 L[v][u] -= 1）去掉
 *       root 行列的行列式
 * 注意：自环不计入；模数固定 998244353（换模改 MOD 与逆元）；
 *       需要计数恰好 k 条特殊边的生成树（@KruskalWQS / 多项式
 *       Laplacian）另见对应模板
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   // 三角形 0-1-2 + 一条 0-1 重边：生成树 {01,12} {12,20} {20,01} {01',12} {12? ,20? } 共 5 棵
 *   vector<pair<int,int>> es{{0, 1}, {1, 2}, {2, 0}, {0, 1}};
 *   cout << count_spanning(3, es) << '\n';   // 5（Laplacian [[3,-2],[-2,3]] det = 5）
 *   vector<pair<int,int>> es2{{0, 1}, {0, 2}, {1, 2}};
 *   cout << count_arborescence(3, es2, 0) << '\n';  // 2（{0→1,0→2} 与 {0→1,1→2}）
 * }
 */
