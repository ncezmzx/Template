#include <bits/stdc++.h>
using namespace std;

// iterative (bottom-up) segment tree: point update + range max of pairs
template <size_t N>
struct segtree_iterative {
  int n;
  pair<int, int> sgt[N << 1];  // leaves at [n, 2n-1], root at 1
  // point update: position x becomes y, then recompute ancestors
  void update(int x, const pair<int, int>& y) {
    sgt[x += n - 1] = y;
    for (x >>= 1; x; x >>= 1) sgt[x] = max(sgt[x << 1], sgt[x << 1 | 1]);
  }
  // max over [l, r] (out-of-range l/r are clamped; empty range -> {-1,-1})
  pair<int, int> query(int l, int r) {
    l = max(1, l), r = min(r, n);
    if (l > r) return {-1, -1};
    pair<int, int> res;
    for (l += n - 1, r += n; l ^ r; l >>= 1, r >>= 1) {
      if (l & 1) res = max(res, sgt[l++]);
      if (r & 1) res = max(res, sgt[--r]);
    }
    return res;
  }
};

/*
 * ============================================================
 * Name: iterative segment tree (bottom-up, point update + range query)
 * Complexity: point update O(log n), range query O(log n)
 * Usage: point assignment + range max (here of pairs; change the merge
 *        function for sum/min), wrapped as segtree_iterative<N>:
 *        set st.n, then update(x, {v, x}); query(l, r) returns the max
 *        (value, index) pair over [l, r]
 * Source: all.cpp lines 14784-14803 (sgt array version; the block also had a
 *         blk-based variant sgt1, dropped as duplicate); query clamps l/r into
 *         [1, n] and returns {-1,-1} for an empty range
 * ============================================================
 * Example (uncomment to compile):
 * static segtree_iterative<1000009> st;
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   st.n = 5;
 *   st.update(1, {3, 1}), st.update(2, {7, 2}), st.update(3, {5, 3}), st.update(4, {9, 4}), st.update(5, {2, 5});
 *   auto p = st.query(1, 5);      // (9, 4)
 *   cout << p.first << ' ' << p.second << '\n';
 *   p = st.query(2, 3);           // (7, 2)
 *   cout << p.first << ' ' << p.second << '\n';
 *   st.update(4, {1, 4});         // after the change [1,5] max is (7,2)
 *   cout << st.query(1, 5).first << '\n';
 *   return 0;
 * }
 * ============================================================
 */
