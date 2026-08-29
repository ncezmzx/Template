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
 * Complexity: build O(n)
 * Usage: `cartesian_tree<N>`: fill a[1..n], build(n); the tree lives in lc / rc
 *        / fa.
 *        in-order = the original sequence; min-heap on values (flip the
 *        comparison for max-heap).
 *        RMQ: min over [l, r] = a[lca(l, r)] (preprocess LCA on this tree).
 *        largest rectangle: the subtree interval of x is the longest interval
 *        where a[x] is the minimum, area = a[x] * sz[x].
 *        a Treap with fixed keys (positions) and priorities (values) is exactly
 *        a Cartesian tree.
 * Principle: monotonic stack: each new element pops larger stack tops, the
 *            popped chain becomes its left subtree, and it attaches right of
 *            the new top
 * Notes: equal values need a tiebreak (e.g. position order); the root is the
 *        node with fa = 0; null nodes are 0
 * ============================================================
 */
