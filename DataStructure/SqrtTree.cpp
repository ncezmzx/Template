#include <bits/stdc++.h>
using namespace std;
#define int long long

// Sqrt Tree: static associative range queries in O(log log n)
template <size_t N>
struct sqrt_tree {
  int n, a[N], bsz, bn;
  int pre[1000][1000], suf[1000][1000];
  int t[4000], tsz;
  int qop(int x, int y) { return min(x, y); }  // associative op (change as needed)
  int qe() { return 0x3f3f3f3f3f3f3f3f; }      // identity element
  void build_seg(int p, int l, int r) {
    if (l == r) {
      t[p] = a[l];
      return;
    }
    int m = (l + r) >> 1;
    build_seg(p << 1, l, m), build_seg(p << 1 | 1, m + 1, r);
    t[p] = qop(t[p << 1], t[p << 1 | 1]);
  }
  int tquery(int p, int l, int r, int x, int y) {
    if (x <= l && r <= y) return t[p];
    int m = (l + r) >> 1, res = qe();
    if (x <= m) res = qop(res, tquery(p << 1, l, m, x, y));
    if (m < y) res = qop(res, tquery(p << 1 | 1, m + 1, r, x, y));
    return res;
  }
  void build(int n_) {  // block prefix/suffix aggregates + segment tree over blocks
    n = n_;
    bsz = sqrt(n) + 1, bn = (n + bsz - 1) / bsz;
    for (int b = 1; b <= bn; ++b) {
      int L = (b - 1) * bsz + 1, R = min(b * bsz, n);
      pre[b][1] = a[L];
      for (int i = L + 1; i <= R; ++i) pre[b][i - L + 1] = qop(pre[b][i - L], a[i]);
      suf[b][R - L + 1] = a[R];
      for (int i = R - 1; i >= L; --i) suf[b][i - L + 1] = qop(suf[b][i - L + 2], a[i]);
    }
    tsz = 1;
    while (tsz < bn) tsz <<= 1;
    for (int i = 1; i <= tsz * 2; ++i) t[i] = qe();
    for (int i = 1; i <= bn; ++i) {
      int len = min(bsz, n - (i - 1) * bsz);
      t[i + tsz - 1] = pre[i][len];
    }
    for (int i = tsz - 1; i >= 1; --i) t[i] = qop(t[i << 1], t[i << 1 | 1]);
  }
  int query(int l, int r) {  // associative aggregate over a[l..r]
    int bl = (l - 1) / bsz + 1, br = (r - 1) / bsz + 1;
    if (bl == br) {
      int L = (bl - 1) * bsz + 1;
      int res = qe();
      for (int i = l; i <= r; ++i) res = qop(res, a[i]);
      return res;
    }
    int res = qop(suf[bl][l - (bl - 1) * bsz], pre[br][r - (br - 1) * bsz]);
    if (bl + 1 <= br - 1) res = qop(res, tquery(1, 1, tsz, bl + 1, br - 1));
    return res;
  }
};

/*
 * ============================================================
 * Name: Sqrt Tree (static associative range queries)
 * Complexity: preprocessing O(n log log n) (block prefix/suffix + inter-block
 *             segment tree), query O(log log n)
 * Usage: static associative range queries (min/max/gcd/sum, defined by qop),
 *        wrapped as sqrt_tree<N>: fill a[1..n], build(n), query(l, r);
 *        smaller constants than a segment tree; this is the two-level version
 *        (block prefix/suffix + inter-block segment tree; recursive blocking
 *        reaches O(1), see OI-Wiki)
 * Principle: blocks of size ~sqrt(n): each block precomputes prefix/suffix
 *        aggregates; inter-block aggregate kept in a segment tree (~sqrt(n)
 *        blocks); a query = left block suffix + middle blocks via the tree +
 *        right block prefix
 * Notes: qop must be associative; qe() the identity; static only (updates
 *        need rebuilding a whole block in O(sqrt n))
 * ============================================================
 * Example (uncomment to compile; range minimum):
 * static sqrt_tree<200009> sqt;
 * signed main() {
 *   int n;
 *   cin >> n;
 *   for (int i = 1; i <= n; ++i) cin >> sqt.a[i];
 *   sqt.build(n);
 *   int q;
 *   cin >> q;
 *   while (q--) {
 *     int l, r;
 *     cin >> l >> r;
 *     cout << sqt.query(l, r) << '\n';
 *   }
 * }
 * ============================================================
 */
