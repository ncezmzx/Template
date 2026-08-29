#include <bits/stdc++.h>
using namespace std;
#define int long long

// Matrix-Tree theorem: spanning tree counting (undirected / directed arborescences), mod 998244353
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
// determinant by Gaussian elimination (prime mod); a is n x n and gets overwritten
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
    if (k != i) swap(a[i], a[k]), res = (MOD - res) % MOD;  // row swap flips sign
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
// number of spanning trees of an undirected graph (multi-edges ok, loops ignored)
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
// number of out-arborescences rooted at root (each non-root has exactly one in-edge)
int count_arborescence(int n, const vector<pair<int, int>>& edges, int root) {
  if (n <= 1) return 1;
  vector<vector<int>> L(n, vector<int>(n, 0));  // in-degree Laplacian
  vector<int> idx;
  for (int i = 0; i < n; ++i)
    if (i != root) idx.push_back(i);
  for (auto& e : edges) {
    int u = e.first, v = e.second;  // edge u -> v: an in-edge of v
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
 * Name: Matrix-Tree theorem (spanning tree counting)
 * Complexity: O(n^3) (determinant elimination)
 * Usage: count_spanning(n, edges): spanning trees of an undirected graph
 *        (multi-edges allowed);
 *        count_arborescence(n, edges, root): out-arborescences rooted at root
 *        of a directed graph (reverse all edges for in-arborescences); both
 *        modulo 998244353.
 * Principle: undirected: the determinant of the Laplacian L = D - A with any
 *            one row and column removed (Kirchhoff);
 *            directed out-arborescences: the in-degree Laplacian (L[v][v] =
 *            indeg(v), an edge u->v contributes L[v][u] -= 1) with the root's
 *            row and column removed
 * Notes: self-loops are ignored; the modulus is fixed at 998244353 (change MOD
 *        and the inverse to switch); counting trees with exactly k special
 *        edges is a different problem (see KruskalWQS / polynomial Laplacians)
 * ============================================================
 */
