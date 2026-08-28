#include <bits/stdc++.h>
using namespace std;
#define int long long

// linear RMQ: O(n) preprocessing, O(1) query (Cartesian tree + Euler tour +
// +-1 block RMQ with bitmask tables)
template <size_t N>
struct rmq_linear {
  static constexpr int B = 16;
  int n, a[N];
  int lc[N], rc[N], fa[N], stk[N];
  int euler[N * 2], dep[N * 2], fst[N], ecnt;
  short f[1 << (B - 1)][B][B];
  char done[1 << (B - 1)];
  int bmask[(N << 1) / B + 5], mnv[(N << 1) / B + 5], mnp[(N << 1) / B + 5];  // Euler tour has length 2n
  int stp[20][(N << 1) / B + 5], lg2[(N << 1) / B + 5];
  void cartesian_build() {
    int tp = 0;
    for (int i = 1; i <= n; ++i) {
      int last = 0;
      while (tp && a[stk[tp]] > a[i]) last = stk[tp--];
      if (tp) rc[stk[tp]] = i, fa[i] = stk[tp];
      lc[i] = last;
      if (last) fa[last] = i;
      stk[++tp] = i;
    }
  }
  void euler_dfs(int x, int d) {
    fst[x] = ++ecnt;
    euler[ecnt] = x, dep[ecnt] = d;
    if (lc[x]) {
      euler_dfs(lc[x], d + 1);
      euler[++ecnt] = x, dep[ecnt] = d;
    }
    if (rc[x]) {
      euler_dfs(rc[x], d + 1);
      euler[++ecnt] = x, dep[ecnt] = d;
    }
  }
  void pm1_build(int m) {  // +-1 RMQ: bitmask per block + sparse table over blocks
    int bcnt = (m + B - 1) / B;
    for (int b = 0; b < bcnt; ++b) {
      int L = b * B + 1, R = min(m, (b + 1) * B), len = R - L + 1;
      int mask = 0;
      for (int i = 0; i < len - 1; ++i)
        if (dep[L + i + 1] > dep[L + i]) mask |= 1 << i;
      bmask[b] = mask;
      if (!done[mask]) {
        done[mask] = 1;
        for (int i = 0; i < len; ++i) {
          f[mask][i][i] = i;
          for (int j = i + 1; j < len; ++j)
            f[mask][i][j] =
                dep[L + f[mask][i][j - 1]] < dep[L + j] ? f[mask][i][j - 1] : j;
        }
      }
      int p = f[mask][0][len - 1];
      mnv[b] = dep[L + p], mnp[b] = L + p;
      stp[0][b] = b;
    }
    for (int k = 1; (1 << k) <= bcnt; ++k)
      for (int i = 0; i + (1 << k) <= bcnt; ++i) {
        int x = stp[k - 1][i], y = stp[k - 1][i + (1 << (k - 1))];
        stp[k][i] = mnv[x] <= mnv[y] ? x : y;
      }
    for (int i = 1; i <= bcnt; ++i) lg2[i] = __lg(i);
  }
  int pm1_query(int l, int r) {
    int bl = (l - 1) / B, br = (r - 1) / B;
    int L1 = bl * B + 1, L2 = br * B + 1;
    int res = L1 + f[bmask[bl]][l - L1][br == bl ? r - L1 : B - 1];
    if (bl != br) {
      int rp = L2 + f[bmask[br]][0][r - L2];
      if (dep[rp] < dep[res]) res = rp;
      if (bl + 1 <= br - 1) {
        int k = lg2[br - bl - 1];
        int x = stp[k][bl + 1], y = stp[k][br - (1 << k)];
        int b = mnv[x] <= mnv[y] ? x : y;
        if (mnv[b] < dep[res]) res = mnp[b];
      }
    }
    return res;
  }
  void build(int n_) {  // fill a[1..n] first
    n = n_;
    memset(lc, 0, sizeof lc);
    memset(rc, 0, sizeof rc);
    memset(fa, 0, sizeof fa);
    memset(fst, 0, sizeof fst);
    memset(done, 0, sizeof done);
    cartesian_build();
    ecnt = 0;
    euler_dfs(stk[1], 1);
    pm1_build(ecnt);
  }
  int query(int u, int v) {  // position of the minimum in a[min(u,v)..max(u,v)]
    int l = fst[u], r = fst[v];
    if (l > r) swap(l, r);
    return euler[pm1_query(l, r)];
  }
};

/*
 * ============================================================
 * Name: linear RMQ (O(n) preprocessing, O(1) query; from OI-Wiki topic/rmq)
 * Complexity: preprocessing O(n), query O(1)
 * Usage: static range minimum query returning the index, wrapped as
 *        rmq_linear<N>: fill a[1..n], build(n), then query(u, v) returns the
 *        position of the minimum of a over [u, v]
 * Principle (three steps of the OI-Wiki linear RMQ):
 *   1) build the (min) Cartesian tree: the range minimum is the LCA of the
 *      two endpoints in that tree;
 *   2) the Euler tour turns LCA into a "+-1 RMQ" on depths (adjacent depths
 *      differ by exactly +-1);
 *   3) +-1 RMQ via blocks + bitmasks: block size B = 16 gives only
 *      2^(B-1) = 32768 shapes; brute-force preprocess all in-block ranges per
 *      shape (16MB); a sparse table over the blocks (n/B of them) takes O(n)
 * Notes: with equal values the leftmost minimum is returned (strict > pop in
 *        the Cartesian build); dep is the +-1 sequence, f tables are reused
 *        per shape; array indices 1..n
 * ============================================================
 * Example (uncomment to compile):
 * static rmq_linear<200009> rmq;
 * signed main() {
 *   int n;
 *   cin >> n;
 *   for (int i = 1; i <= n; ++i) cin >> rmq.a[i];
 *   rmq.build(n);
 *   int q;
 *   cin >> q;
 *   while (q--) {
 *     int l, r;
 *     cin >> l >> r;
 *     cout << rmq.a[rmq.query(l, r)] << '\n';
 *   }
 * }
 * ============================================================
 */
