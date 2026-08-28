#include <bits/stdc++.h>
using namespace std;

// Sqrt Tree: static associative range queries in O(log log n), generic monoid
template<class S, S (*op)(S, S), S (*e)(), size_t N = 200000>
struct SqrtTree {
  int n, bsz, bn, tsz;
  S pre[static_cast<int>(sqrt(N)) + 9][static_cast<int>(sqrt(N)) + 9];
  S suf[static_cast<int>(sqrt(N)) + 9][static_cast<int>(sqrt(N)) + 9];
  S t[(static_cast<int>(sqrt(N)) + 9) << 2], a[N];

  void sqt_build(int _n, const S *arr) {  // arr is 1-indexed, length _n
    n = _n, bsz = sqrt(n) + 1, bn = (n + bsz - 1) / bsz;
    for (int i = 1; i <= n; ++i) a[i] = arr[i];
    for (int b = 1; b <= bn; ++b) {
      int L = (b - 1) * bsz + 1, R = min(b * bsz, n);
      pre[b][1] = a[L];
      for (int i = L + 1; i <= R; ++i) pre[b][i - L + 1] = op(pre[b][i - L], a[i]);
      suf[b][R - L + 1] = a[R];
      for (int i = R - 1; i >= L; --i) suf[b][i - L + 1] = op(a[i], suf[b][i - L + 2]);
    }
    tsz = 1;
    while (tsz < bn) tsz <<= 1;
    for (int i = 1; i <= tsz * 2; ++i) t[i] = e();
    for (int i = 1; i <= bn; ++i) {
      int len = min(bsz, n - (i - 1) * bsz);
      t[i + tsz - 1] = pre[i][len];
    }
    for (int i = tsz - 1; i >= 1; --i) t[i] = op(t[i << 1], t[i << 1 | 1]);
  }

  S tquery(int p, int l, int r, int x, int y) {  // segment tree over whole blocks
    if (x <= l && r <= y) return t[p];
    int m = (l + r) >> 1;
    S res = e();
    if (x <= m) res = op(res, tquery(p << 1, l, m, x, y));
    if (m < y) res = op(res, tquery(p << 1 | 1, m + 1, r, x, y));
    return res;
  }

  S sqt_query(int l, int r) {  // associative aggregate over a[l..r]
    int bl = (l - 1) / bsz + 1, br = (r - 1) / bsz + 1;
    if (bl == br) {
      S res = e();
      for (int i = l; i <= r; ++i) res = op(res, a[i]);
      return res;
    }
    S res = suf[bl][l - (bl - 1) * bsz];
    if (bl + 1 <= br - 1) res = op(res, tquery(1, 1, tsz, bl + 1, br - 1));
    return op(res, pre[br][r - (br - 1) * bsz]);
  }
};

/*
 * ============================================================
 * Name: Sqrt Tree (static associative range queries), generic monoid
 * Complexity: preprocessing O(n log log n) (block prefix/suffix aggregates +
 *             inter-block segment tree); query O(log log n)
 * Usage: static associative range queries (min/max/gcd/sum etc. via the
 *        op/e template parameters), wrapped as SqrtTree<S, op, e, N>:
 *        fill a 1-indexed array, sqt_build(n, arr), then sqt_query(l, r);
 *        smaller constants than a segment tree (log log n, no recursion on
 *        the hot path); this is the two-level form (block prefix/suffix +
 *        inter-block segment tree; recursive blocking reaches O(1), see
 *        OI-wiki)
 * Principle: blocks of size ~sqrt(n): each block precomputes prefix/suffix
 *        aggregates; a segment tree maintains the whole-block aggregates
 *        (~sqrt(n) blocks); a query = left block suffix + middle whole
 *        blocks via the tree + right block prefix
 * Notes: op must be associative; e() is the identity; static only (no
 *        updates; an update needs rebuilding a whole block in O(sqrt n))
 * ============================================================
 * Example (uncomment to compile; range minimum):
 * int op(int x, int y) { return min(x, y); }
 * int e() { return 0x3f3f3f3f; }
 * static SqrtTree<int, op, e, 200009> s;
 * static int a[200009];
 * signed main() {
 *   int n;
 *   cin >> n;
 *   for (int i = 1; i <= n; ++i) cin >> a[i];
 *   s.sqt_build(n, a);
 *   int q;
 *   cin >> q;
 *   while (q--) {
 *     int l, r;
 *     cin >> l >> r;
 *     cout << s.sqt_query(l, r) << '\n';
 *   }
 * }
 * ============================================================
 */
