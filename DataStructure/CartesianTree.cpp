#include <bits/stdc++.h>
using namespace std;
#define int long long

// Cartesian tree (min-heap ordered), built by a monotonic stack in O(n)
template <size_t N>
struct cartesian_tree {
  int n, a[N], lc[N], rc[N], fa[N], stk[N];
  void build(int n_) {
    n = n_;
    for (int i = 1; i <= n; ++i) lc[i] = rc[i] = fa[i] = 0;
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
};

/*
 * ============================================================
 * Name: Cartesian tree (min-heap ordered, monotonic stack build in O(n))
 * Complexity: O(n)
 * Usage: given a sequence a[1..n], build the binary tree satisfying, wrapped
 *        as cartesian_tree<N> (fill a[1..n], build(n); the tree lives in
 *        lc / rc / fa):
 *        1) in-order traversal = the original sequence (positions as keys);
 *        2) heap property: parent value < child values (min-heap; flip the
 *           comparison for max-heap).
 *        Classic applications:
 *        - RMQ: the minimum over [l, r] is the value at lca(l, r)
 *          (preprocess LCA on the Cartesian tree);
 *        - largest rectangle in a histogram / maximal submatrix: the subtree
 *          interval rooted at x is the longest interval where a[x] is the
 *          minimum; its length is sz[x] (computable), area = a[x] * sz[x];
 *        - relation to Treap: a Treap with fixed keys (positions) and
 *          priorities (values) is a Cartesian tree
 * Principle: a monotonic stack maintains the current rightmost chain; each
 *        new element pops larger stack tops, the popped chain becomes its
 *        left subtree, and it attaches to the right of the new stack top
 * Notes: equal values need a tiebreak (e.g. position order) to avoid
 *        ambiguity; after the build the root is the node with fa = 0 (the
 *        stack bottom); null nodes have lc/rc/fa = 0
 * ============================================================
 * Example (uncomment to compile; longest interval where each position is the minimum):
 * static cartesian_tree<100009> ct;
 * signed main() {
 *   int n;
 *   cin >> n;
 *   for (int i = 1; i <= n; ++i) cin >> ct.a[i];
 *   ct.build(n);
 *   vector<int> sz(n + 1, 1);
 *   function<void(int)> dfs = [&](int x) {
 *     if (ct.lc[x]) dfs(ct.lc[x]), sz[x] += sz[ct.lc[x]];
 *     if (ct.rc[x]) dfs(ct.rc[x]), sz[x] += sz[ct.rc[x]];
 *   };
 *   for (int x = 1; x <= n; ++x)
 *     if (!ct.fa[x]) { dfs(x); break; }
 *   for (int i = 1; i <= n; ++i) cout << ct.a[i] * sz[i] << ' ';
 * }
 * ============================================================
 */
