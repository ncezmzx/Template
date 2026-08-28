#include <bits/stdc++.h>
using namespace std;

// Li Chao segment tree: insert lines/segments y = kx + b over domain [1, n],
// query the minimum at a point; full-line inserts create no nodes, segment
// inserts create O(log n) nodes each
template <size_t N, size_t SP>
struct lichao_segtree {
  static constexpr long long INF = 4000000000000000000LL;
  int n_, tot, rt, lc[SP], rc[SP];
  long long K[SP], B[SP];  // each node stores one line y = Kx + B (sentinel K=0, B=INF)
  int new_node() {
    ++tot, lc[tot] = rc[tot] = 0, K[tot] = 0, B[tot] = INF;
    return tot;
  }
  void init(int n) { n_ = n, tot = 0, rt = new_node(); }
  long long f(long long k, long long b, int x) const { return k * x + b; }  // must not overflow
  void insert_full(int u, int l, int r, long long k, long long b) {  // keep the better line on [l,r]
    int m = l + r >> 1;
    bool lef = f(k, b, l) < f(K[u], B[u], l), mid = f(k, b, m) < f(K[u], B[u], m);
    if (mid) swap(k, K[u]), swap(b, B[u]);  // midpoint winner stays at this node
    if (l == r) return;
    if (lef != mid) {  // crossing in the left half -> loser goes left
      if (!lc[u]) lc[u] = new_node();
      insert_full(lc[u], l, m, k, b);
    } else {  // crossing in the right half -> loser goes right
      if (!rc[u]) rc[u] = new_node();
      insert_full(rc[u], m + 1, r, k, b);
    }
  }
  void ins(int& u, int l, int r, int x, int y, long long k, long long b) {  // segment restricted to [x,y]
    if (!u) u = new_node();
    if (x <= l && r <= y) return insert_full(u, l, r, k, b);
    int m = l + r >> 1;
    if (x <= m) ins(lc[u], l, m, x, y, k, b);
    if (y > m) ins(rc[u], m + 1, r, x, y, k, b);
  }
  void add_line(long long k, long long b) { insert_full(rt, 1, n_, k, b); }             // full-domain line
  void add_seg(int x, int y, long long k, long long b) { ins(rt, 1, n_, x, y, k, b); }  // segment [x,y]
  long long query(int x) const {  // min over all lines/segments covering x (INF = none)
    int u = rt, l = 1, r = n_;
    long long res = INF;
    while (u) {
      res = min(res, f(K[u], B[u], x));
      int m = l + r >> 1;
      if (x <= m) u = lc[u], r = m;
      else u = rc[u], l = m + 1;
    }
    return res;
  }
};

/*
 * ============================================================
 * Name: Li Chao segment tree (line/segment insert, point minimum query)
 * Complexity: insert O(log n) (<= 2 log n new nodes); query O(log n)
 * Usage: maintain a set of lines/segments and query the minimum function
 *        value at any integer x, wrapped as lichao_segtree<N, SP>:
 *        init(n) sets domain [1,n]; add_line(k,b) full-domain line;
 *        add_seg(x,y,k,b) segment restricted to [x,y]; query(x) the minimum;
 *        typical uses: online slope-trick DP, coverage extrema problems
 * Principle: each node stores the line best at its midpoint; on insert, compare
 *        with the node's line at the midpoint and push the loser into the
 *        left/right child depending on where the lines cross (two lines cross
 *        at most once); query takes the min over all lines on the root-to-leaf path
 * Notes: k*x + b must fit in long long (input guarantee); for maxima flip all
 *        comparisons in insert_full/query; sentinel B=INF means empty
 * ============================================================
 * Example (uncomment to compile):
 * static lichao_segtree<1000009, 2000009> lc;
 * signed main() {
 *   lc.init(10);
 *   lc.add_line(1, 0);            // y = x
 *   lc.add_line(-1, 5);           // y = 5 - x
 *   lc.add_seg(4, 8, 0, 1);       // y = 1 on [4,8]
 *   cout << lc.query(2) << ' ' << lc.query(3) << '\n';  // 2 2 (the two lines cross at (2.5, 2.5))
 *   cout << lc.query(5) << ' ' << lc.query(9) << '\n';  // 0 -4 (at 5, y=5-x=0 wins; at 9 the segment does not cover, y=5-9)
 * }
 */
