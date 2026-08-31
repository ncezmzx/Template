#include <bits/stdc++.h>
using namespace std;

// persistent DSU: union by size (no path compression, required for persistence)
// two persistent segment trees store fa and siz; version 0 is the initial state
template <size_t N, size_t MV> struct persistent_dsu {
  int lc[N], rc[N], val[N], tot;
  int faroot[MV], sizroot[MV], vcnt, n_;
  int build(int l, int r, int type) { // type 0: fa (leaf = index); type 1: siz (leaf = 1)
    int u = ++tot;
    lc[u] = rc[u] = 0;
    if (l == r) {
      val[u] = type ? 1 : l;
      return u;
    }
    int m = l + r >> 1;
    lc[u] = build(l, m, type), rc[u] = build(m + 1, r, type);
    return u;
  }
  int upd(int pre, int l, int r, int p, int v) {
    int u = ++tot;
    lc[u] = lc[pre], rc[u] = rc[pre], val[u] = val[pre];
    if (l == r) {
      val[u] = v;
      return u;
    }
    int m = l + r >> 1;
    p <= m ? lc[u] = upd(lc[pre], l, m, p, v) : rc[u] = upd(rc[pre], m + 1, r, p, v);
    return u;
  }
  int qry(int u, int l, int r, int p) {
    if (l == r) return val[u];
    int m = l + r >> 1;
    return p <= m ? qry(lc[u], l, m, p) : qry(rc[u], m + 1, r, p);
  }
  void init(int n) {
    n_ = n, tot = 0, vcnt = 0;
    faroot[0] = build(1, n, 0);
    sizroot[0] = build(1, n, 1);
  }
  int find(int ver, int x) { // root of x in version ver (depth O(log n))
    int f = qry(faroot[ver], 1, n_, x);
    return f == x ? x : find(ver, f);
  }
  int merge(int ver, int x, int y) { // merge x, y on version ver; returns new version id
    int rx = find(ver, x), ry = find(ver, y);
    ++vcnt;
    if (rx == ry) {
      faroot[vcnt] = faroot[ver], sizroot[vcnt] = sizroot[ver];
      return vcnt;
    }
    int sx = qry(sizroot[ver], 1, n_, rx), sy = qry(sizroot[ver], 1, n_, ry);
    if (sx < sy) swap(rx, ry), swap(sx, sy);
    faroot[vcnt] = upd(faroot[ver], 1, n_, ry, rx); // hang the smaller under the larger
    sizroot[vcnt] = upd(sizroot[ver], 1, n_, rx, sx + sy);
    return vcnt;
  }
  bool same(int ver, int x, int y) { return find(ver, x) == find(ver, y); }
};

/*
 * ============================================================
 * Name: persistent DSU
 * Complexity: one merge / query O(log^2 n)
 * Usage: DSU with historical versions, `persistent_dsu<N, MV>`: init(n);
 *        merge(ver, x, y) -> new version id;
 *        same(ver, x, y); find(ver, x); "rollback to version k" = set ver = k.
 * Principle: persistent segment trees persist fa[] and siz[]; a version is the
 *            pair of roots; union by size keeps the height O(log n) so find may
 *            recurse
 * Notes: vertices 1..n; N ~ 4n + 36m (n, m = 2e5 -> N ~ 8e6); indices stored as
 *        32-bit int to save memory;
 *        path compression is impossible — it would corrupt structures shared
 *        with old versions
 * Source: OI-Wiki "Persistent DSU" (https://oi-wiki.org/ds/persistent-in-dsu/)
 * ============================================================
 */
