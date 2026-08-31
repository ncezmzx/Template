#include <bits/stdc++.h>
using namespace std;

// Global balanced BST maintaining a monoid `S` with lazy tags `F`, following the
// same algebraic conventions as SegmentTree_Semigroup.cpp:
//   S + S -> S   (associative info combine; e() is the identity)
//   S + F -> S   (apply a tag to a combined info; S is the first operand)
//   F + F -> F   (tag merge; left tag first, then right)
//   e()          identity info;  id() identity tag
// Each heavy chain becomes a weighted-median BST whose in-order traversal equals
// the chain's dfn order (top -> bottom); `mx` is the forward (in-order) product
// and `rmx` the reversed one, so ordered path products work for non-commutative
// op. Lazy tags act pointwise, so both products transform identically under a tag.
template <size_t N, class S, S (*e)(), class F, F (*id)()>
struct global_bst {
  int n, idx;
  S val[N], mx[N], rmx[N];
  F tag[N];
  int fa[N], dep[N], sz[N], son[N], top[N], dfn[N], who[N], light[N];
  int ls[N], rs[N], f[N], w[N], Lp[N], Rp[N], root[N], pstk[N];
  vector<int> es[N];
  void dfs1(int x, int ff) {
    fa[x] = ff, dep[x] = dep[ff] + 1, sz[x] = 1;
    for (int y : es[x])
      if (y != ff) {
        dfs1(y, x), sz[x] += sz[y];
        if (sz[y] > sz[son[x]]) {
          if (son[x]) light[x] += sz[son[x]];
          son[x] = y;
        }
        else light[x] += sz[y];
      }
  }
  void dfs2(int x, int tp) {
    top[x] = tp, dfn[x] = ++idx, who[idx] = x;
    if (son[x]) dfs2(son[x], tp);
    for (int y : es[x])
      if (y != fa[x] && y != son[x]) dfs2(y, y);
  }
  void up(int x) {
    Lp[x] = ls[x] ? Lp[ls[x]] : dfn[x];
    Rp[x] = rs[x] ? Rp[rs[x]] : dfn[x];
    S a = ls[x] ? mx[ls[x]] : e(), b = rs[x] ? mx[rs[x]] : e();
    mx[x] = (a + val[x]) + b; // in-order:  ls, x, rs
    a = ls[x] ? rmx[ls[x]] : e(), b = rs[x] ? rmx[rs[x]] : e();
    rmx[x] = (b + val[x]) + a; // reversed:  rs, x, ls
  }
  void apply(int x, F g) {
    if (!x) return;
    val[x] = val[x] + g;
    mx[x] = mx[x] + g;
    rmx[x] = rmx[x] + g;
    tag[x] = tag[x] + g;
  }
  void pushdown(int x) {
    if (tag[x] != id()) {
      apply(ls[x], tag[x]);
      apply(rs[x], tag[x]);
      tag[x] = id();
    }
  }
  int build_chain(int l, int r) { // weighted-median BST of one heavy chain
    if (l > r) return 0;
    int tot = 0;
    for (int i = l; i <= r; ++i) tot += w[who[i]];
    int acc = 0, mid = l;
    for (int i = l; i <= r; ++i) {
      acc += w[who[i]];
      if (acc * 2 >= tot) {
        mid = i;
        break;
      }
    }
    int x = who[mid];
    ls[x] = build_chain(l, mid - 1);
    rs[x] = build_chain(mid + 1, r);
    if (ls[x]) f[ls[x]] = x;
    if (rs[x]) f[rs[x]] = x;
    up(x);
    return x;
  }
  void build(int n_) { // HLD + one BST per heavy chain; val[1..n] must be set first
    n = n_, idx = 0;
    for (int i = 1; i <= n; ++i) { // reset HLD/BST state so build() is repeatable per test case
      tag[i] = id();
      fa[i] = dep[i] = sz[i] = son[i] = top[i] = dfn[i] = light[i] = 0;
      ls[i] = rs[i] = f[i] = root[i] = 0;
    }
    for (int h = 1; h <= n; ++h)
      if (!fa[h] && top[h] == 0) dfs1(h, 0), dfs2(h, h);
    for (int h = 1; h <= n; ++h)
      if (top[h] == h) {
        int len = 0;
        for (int x = h; x; x = son[x]) ++len;
        for (int i = 0; i < len; ++i) w[who[dfn[h] + i]] = light[who[dfn[h] + i]] + 1;
        root[h] = build_chain(dfn[h], dfn[h] + len - 1);
      }
  }
  void range_apply(int x, int l, int r, F g) {
    if (!x || r < Lp[x] || Rp[x] < l) return;
    if (l <= Lp[x] && Rp[x] <= r) return apply(x, g), void();
    pushdown(x);
    if (l <= dfn[x] && dfn[x] <= r) val[x] = val[x] + g;
    range_apply(ls[x], l, r, g), range_apply(rs[x], l, r, g);
    up(x);
  }
  S range_prod(int x, int l, int r, bool rev = false) {
    if (!x || r < Lp[x] || Rp[x] < l) return e();
    if (l <= Lp[x] && Rp[x] <= r) return rev ? rmx[x] : mx[x];
    pushdown(x);
    S res = e();
    if (l <= dfn[x] && dfn[x] <= r) res = val[x];
    S a = range_prod(rev ? rs[x] : ls[x], l, r, rev);
    S b = range_prod(rev ? ls[x] : rs[x], l, r, rev);
    return (a + res) + b;
  }
  void path_apply(int u, int v, F g) {
    while (top[u] != top[v]) {
      if (dep[top[u]] < dep[top[v]]) swap(u, v);
      range_apply(root[top[u]], dfn[top[u]], dfn[u], g);
      u = fa[top[u]];
    }
    if (dep[u] > dep[v]) swap(u, v);
    range_apply(root[top[u]], dfn[u], dfn[v], g);
  }
  // ordered product along u -> v (correct for non-commutative op):
  // left accumulates the u-side segments (each reversed) in path order, right the
  // v-side segments (forward) in path order; the final chain is finished by the
  // LCA w = shallower of the two current endpoints, included exactly once.
  S path_prod(int u, int v) {
    S left = e(), right = e();
    while (top[u] != top[v]) {
      if (dep[top[u]] < dep[top[v]]) { // v-side top deeper: climb v
        right = range_prod(root[top[v]], dfn[top[v]], dfn[v]) + right;
        v = fa[top[v]];
      }
      else { // climb u
        left = left + range_prod(root[top[u]], dfn[top[u]], dfn[u], true);
        u = fa[top[u]];
      }
    }
    if (dep[u] > dep[v]) // v == w: finish the u side (reversed, includes w)
      left = left + range_prod(root[top[u]], dfn[v], dfn[u], true);
    else // u == w: finish the v side (forward, includes w)
      right = range_prod(root[top[u]], dfn[u], dfn[v]) + right;
    return left + right;
  }
  void point_set(int x, S v) {
    int tp2 = 0; // reuse member stack array instead of a large local
    for (int y = x; y; y = f[y]) pstk[++tp2] = y;
    while (tp2) pushdown(pstk[tp2--]);
    val[x] = v;
    for (int y = x; y; y = f[y]) up(y);
  }
};

/*
 * ============================================================
 * Name: Global balanced BST over a monoid with lazy tags
 *        (static-tree path operations, ordered for non-commutative op)
 * Complexity: preprocessing O(n log n); path apply / path prod / point set
 *             amortized O(log n)
 * Usage: path updates and queries on a static tree,
 *        `global_bst<N, S, e, F, id>`: fill es, set val[1..n], build(n), then
 *        path_apply / path_prod / point_set.
 *        S and F must define operator+: S + S (info combine), S + F (apply tag
 *        to info, S first), F + F (compose tags, "left first then right").
 *        O(log n) in-chain operations vs O(log^2 n) for HLD + segment tree.
 *        Works for sum / max / xor / gcd / matrix products / strings, etc.
 *        (x + (f + g)) == ((x + f) + g): f is applied first, then g.
 *        Caveat (same as semigroup_segtree): range-add on a sum needs the length
 *        folded into S (e.g. S = {sum, cnt} and S + F adds f * cnt); a
 *        range-assign tag works as-is since pointwise tags ignore length.
 * Principle: after HLD each heavy chain becomes a weighted-median BST (in-order
 *            = chain order, subtrees cover contiguous dfn ranges); path ops =
 *            chain climbing + in-chain range ops. mx/rmx keep forward and
 *            reversed products so path_prod(u, v) is in true path order.
 * Notes: vertex weights (push edge weights down to the child); forests are
 *        handled; `who` maps dfn -> node (array `id` renamed to avoid clashing
 *        with the template's identity-tag function); lazy tags require
 *        F to support `tag != id()`.
 * ============================================================
 */
