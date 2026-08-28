#include <bits/stdc++.h>
using namespace std;

// segment tree merging: dynamic value-segment trees, merge b into a
template <size_t SP>
struct segtree_merge {
  int tot, lc[SP], rc[SP], sz[SP];
  void init() { tot = 0; }
  int new_node() {
    return ++tot, lc[tot] = rc[tot] = 0, sz[tot] = 0, tot;
  }
  void insert(int& u, int l, int r, int x, int w = 1) {  // count of value x += w
    if (!u) u = new_node();
    sz[u] += w;
    if (l == r) return;
    int m = l + r >> 1;
    x <= m ? insert(lc[u], l, m, x, w) : insert(rc[u], m + 1, r, x, w);
  }
  int merge(int a, int b, int l, int r) {  // merge b into a, return new root (b invalidated)
    if (!a || !b) return a + b;
    if (l == r) return sz[a] += sz[b], a;
    int m = l + r >> 1;
    lc[a] = merge(lc[a], lc[b], l, m);
    rc[a] = merge(rc[a], rc[b], m + 1, r);
    sz[a] = sz[lc[a]] + sz[rc[a]];
    return a;
  }
  int kth(int u, int l, int r, int k) {  // k-th smallest value (-1 if k exceeds total)
    if (!u || k > sz[u]) return -1;
    while (l < r) {
      int m = l + r >> 1;
      if (k <= sz[lc[u]]) u = lc[u], r = m;
      else k -= sz[lc[u]], u = rc[u], l = m + 1;
    }
    return l;
  }
  int count_le(int u, int l, int r, int x) {  // number of values <= x
    if (!u || x < l) return 0;
    if (r <= x) return sz[u];
    int m = l + r >> 1;
    return count_le(lc[u], l, m, x) + (x > m ? count_le(rc[u], m + 1, r, x) : 0);
  }
};

/*
 * ============================================================
 * Name: segment tree merging (dynamic value-segment trees)
 * Complexity: one merge amortized O(log V) (total cost = total node count);
 *             insert O(log V), kth / count_le O(log V)
 * Usage: merge several value-segment trees for statistics, wrapped as
 *        segtree_merge<SP>: alternative to DSU-on-tree merging, subtree
 *        k-th smallest, per-subtree value-range counting (one tree per
 *        position/subtree); init() clears; insert(root, 1, V, x[, w]);
 *        merge(a, b, 1, V) folds b into a; kth / count_le query
 * Principle: dynamic nodes leave empty subtrees as 0; merging recurses with
 *        two pointers and attaches the non-empty side in O(1) when one side
 *        is empty; merging two trees costs at most the smaller tree's node
 *        count, O((n + q) log V) overall
 * Notes: after merge, tree b's nodes are invalidated; value range V must be
 *        compressed first; SP = total inserts x log V, adjust as needed
 * ============================================================
 * Example (uncomment to compile; merge two trees and query):
 * static segtree_merge<4000009> sm;
 * signed main() {
 *   sm.init();
 *   int A = 0, B = 0;
 *   for (int x : {1, 3, 5, 5}) sm.insert(A, 1, 10, x);
 *   for (int x : {2, 4, 5})    sm.insert(B, 1, 10, x);
 *   A = sm.merge(A, B, 1, 10);
 *   cout << sm.kth(A, 1, 10, 5) << ' ' << sm.kth(A, 1, 10, 7) << '\n';  // 5 -1 (6 values total: 1,2,3,4,5,5)
 *   cout << sm.count_le(A, 1, 10, 3) << ' ' << sm.count_le(A, 1, 10, 5) << '\n';  // 3 6
 * }
 */
