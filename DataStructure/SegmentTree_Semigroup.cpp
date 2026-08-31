#include <bits/stdc++.h>
using namespace std;

template <class S, S (*e)(), class F, F (*id)()>
struct semigroup_segtree {
  struct node {
    S s;
    F tag;
    int lc, rc;
  };
  static constexpr int DEP = 64; // depth cap (value domain up to 2^63)
  int n, root;
  vector<node> pool;
  int stk[DEP * 3 + 4];                       // descent path of updates (pushed in descent order, pulled in reverse)
  int pth[DEP + 2], pl[DEP + 2], pr[DEP + 2]; // leaf path for queries/bisecting (with intervals)

  void clear() {
    root = 0;
    pool.clear();
    pool.push_back({e(), id(), 0, 0}); // pool[0] is always the sentinel; nodes numbered from 1
  }
  void init(int n_) {
    n = n_;
    clear();
  }
  // aligned with ACL's vector constructor: a[1..n_] are initial values, BFS builds the full tree
  // (O(n); only when n is fully buildable)
  void init(int n_, const vector<S> &a) {
    init(n_);
    if (n_ < 1) return;
    pool.reserve(2 * n_);
    root = new_node();
    vector<int> qu{root}, ql{1}, qr{n_}; // BFS order: build children first, then pull in reverse
    for (int i = 0; i < (int)qu.size(); i++) {
      int u = qu[i];
      if (ql[i] == qr[i]) {
        pool[u].s = a[ql[i]];
        continue;
      }
      int m = (ql[i] + qr[i]) >> 1, c1 = new_node(), c2 = new_node();
      pool[u].lc = c1, pool[u].rc = c2; // take return values before writing members (invalidation after reallocation)
      qu.push_back(c1), ql.push_back(ql[i]), qr.push_back(m);
      qu.push_back(c2), ql.push_back(m + 1), qr.push_back(qr[i]);
    }
    for (int i = (int)qu.size() - 1; i >= 0; i--) // reverse BFS order = children before parents
      if (ql[i] != qr[i]) pull(qu[i]);
  }
  void reserve(int k) { pool.reserve(k); }
  int new_node() {
    pool.push_back({e(), id(), 0, 0});
    return (int)pool.size() - 1;
  }

  void apply(int u, F f) {
    if (!u) return;
    pool[u].s = pool[u].s + f;
    pool[u].tag = pool[u].tag + f;
  }
  void push(int u) {
    // note: new_node() may grow pool; take the return value before writing members, otherwise
    // pool[u].lc = new_node() (unspecified evaluation order in C++14) can write into freed memory (UAF)
    if (!pool[u].lc) {
      int c = new_node();
      pool[u].lc = c;
    }
    if (!pool[u].rc) {
      int c = new_node();
      pool[u].rc = c;
    }
    if (pool[u].tag != id()) {
      apply(pool[u].lc, pool[u].tag);
      apply(pool[u].rc, pool[u].tag);
      pool[u].tag = id();
    }
  }
  void pull(int u) { pool[u].s = pool[pool[u].lc].s + pool[pool[u].rc].s; }

  // ---- iterative core: [x, y] decomposes into "split node + left/right boundary paths" ----
  // split node u: the unique (inner) node with x <= mid(u) < y; in the canonical decomposition
  // of [x, y], fully covered nodes = right children taken when "going left" along the left
  // boundary path (including its endpoint) + the symmetric right side.
  void update(int x, int y, F f) { // apply tag f to range [x, y]
    if (x > y) return;
    if (!root) root = new_node();
    int u = root, l = 1, r = n, tp = 0;
    while (l != r) { // descend to the split node (or the x==y leaf), pushing on the way
      int m = (l + r) >> 1;
      if (y <= m) push(u), stk[tp++] = u, u = pool[u].lc, r = m;
      else if (x > m) push(u), stk[tp++] = u, u = pool[u].rc, l = m + 1;
      else break;
    }
    if (l == r) {
      apply(u, f); // x == y: lands directly on a leaf
    }
    else {
      push(u), stk[tp++] = u; // the split node
      int m = (l + r) >> 1;
      for (int v = pool[u].lc, ll = l, rr = m;;) { // left boundary path (following x)
        if (x <= ll) {                             // [ll, rr] fully covered (rr < y always holds)
          apply(v, f);
          break;
        }
        push(v), stk[tp++] = v;
        int mm = (ll + rr) >> 1;
        if (x <= mm) apply(pool[v].rc, f), v = pool[v].lc, rr = mm; // right child fully covered
        else v = pool[v].rc, ll = mm + 1;
      }
      for (int v = pool[u].rc, ll = m + 1, rr = r;;) { // right boundary path (following y)
        if (rr <= y) {                                 // [ll, rr] fully covered (ll > x always holds)
          apply(v, f);
          break;
        }
        push(v), stk[tp++] = v;
        int mm = (ll + rr) >> 1;
        if (y > mm) apply(pool[v].lc, f), v = pool[v].rc, ll = mm + 1; // left child fully covered
        else v = pool[v].lc, rr = mm;
      }
    }
    while (tp) pull(stk[--tp]); // pull in reverse descent order (children always before parents)
  }
  void update(int x, F f) { update(x, x, f); }

  void set(int x, S v) { // point assignment: set the info at x to v
    if (!root) root = new_node();
    int u = root, l = 1, r = n, tp = 0;
    while (l != r) { // iteratively descend to the leaf (pushing/building on the way), then pull in reverse
      push(u), stk[tp++] = u;
      int m = (l + r) >> 1;
      if (x <= m) u = pool[u].lc, r = m;
      else u = pool[u].rc, l = m + 1;
    }
    pool[u].s = v;
    while (tp) pull(stk[--tp]);
  }

  S query(int x, int y) { // info over [x, y] (allocates nothing: unbuilt subtrees count as e())
    if (x > y || !root) return e();
    int u = root, l = 1, r = n;
    while (l != r) {
      int m = (l + r) >> 1;
      if (y <= m) {
        if (!u) return e();
        if (pool[u].tag != id()) push(u); // queries push only when tagged (nodes created only then)
        u = pool[u].lc, r = m;
      }
      else if (x > m) {
        if (!u) return e();
        if (pool[u].tag != id()) push(u);
        u = pool[u].rc, l = m + 1;
      }
      else break;
    }
    if (!u || l == r) return u ? pool[u].s : e();
    if (pool[u].tag != id()) push(u);
    int m = (l + r) >> 1;
    S res = e();
    for (int v = pool[u].lc, ll = l, rr = m;;) { // left boundary: canonical blocks appear top-down right-to-left,
      if (x <= ll) {                             // so new blocks fold in on the left
        if (v) res = pool[v].s + res;
        break;
      }
      if (!v) break; // unbuilt subtree: the rest is all e()
      if (pool[v].tag != id()) push(v);
      int mm = (ll + rr) >> 1;
      if (x <= mm) {
        int c = pool[v].rc;
        if (c) res = pool[c].s + res;
        v = pool[v].lc, rr = mm;
      }
      else v = pool[v].rc, ll = mm + 1;
    }
    for (int v = pool[u].rc, ll = m + 1, rr = r;;) { // right boundary: canonical blocks appear top-down left-to-right,
      if (rr <= y) {                                 // so new blocks fold in on the right
        if (v) res = res + pool[v].s;
        break;
      }
      if (!v) break;
      if (pool[v].tag != id()) push(v);
      int mm = (ll + rr) >> 1;
      if (y > mm) {
        int c = pool[v].lc;
        if (c) res = res + pool[c].s;
        v = pool[v].rc, ll = mm + 1;
      }
      else v = pool[v].lc, rr = mm;
    }
    return res;
  }
  S query(int x) { return query(x, x); }
  S get(int x) { return query(x, x); }               // ACL interface: point query
  S all_prod() { return root ? pool[root].s : e(); } // ACL interface: info over the whole [1, n]

  // ACL interface: max r (x-1 <= r <= n) with g(query(x, r)) true; requires g(e()) == true.
  // Implemented as greedy over the right-side canonical blocks walking up from x's leaf,
  // plus binary descent inside the failing block
  template <class G> int max_right(int x, G g) {
    if (x > n || !root) return n;
    int u = root, l = 1, r = n, tp = 0;
    while (l != r) {
      if (pool[u].tag != id()) push(u);
      pth[tp] = u, pl[tp] = l, pr[tp] = r, tp++;
      int m = (l + r) >> 1;
      if (x <= m) u = pool[u].lc, r = m;
      else u = pool[u].rc, l = m + 1;
    }
    pth[tp] = u; // leaf [x, x] (may be the sentinel 0: treated as e())
    S sm = e();
    if (!g(sm + pool[u].s)) return x - 1; // the first block (the leaf) already fails
    sm = sm + pool[u].s;
    for (int i = tp - 1; i >= 0; i--) {
      int p = pth[i];
      if (pth[i + 1] != pool[p].lc) continue; // only right-sibling blocks "coming from the left child"
      int b = pool[p].rc, m = (pl[i] + pr[i]) >> 1;
      if (!b) continue;           // empty block = e(), automatically fine
      if (g(sm + pool[b].s)) { // fold in the whole block
        sm = sm + pool[b].s;
        continue;
      }
      int w = b, lw = m + 1, rw = pr[i]; // descend inside the block for the rightmost valid position
      while (w && lw < rw) {
        if (pool[w].tag != id()) push(w);
        int mm = (lw + rw) >> 1;
        if (g(sm + pool[pool[w].lc].s)) sm = sm + pool[pool[w].lc].s, w = pool[w].rc, lw = mm + 1;
        else w = pool[w].lc, rw = mm;
      }
      if (!w) continue; // remaining unbuilt = e(), fold in the whole block
      return lw - 1;    // leaf lw fails: answer is lw-1
    }
    return n;
  }

  // ACL interface: min l (1 <= l <= y+1) with g(query(l, y)) true; requires g(e()) == true
  template <class G> int min_left(int y, G g) {
    if (y < 1 || !root) return 1;
    int u = root, l = 1, r = n, tp = 0;
    while (l != r) {
      if (pool[u].tag != id()) push(u);
      pth[tp] = u, pl[tp] = l, pr[tp] = r, tp++;
      int m = (l + r) >> 1;
      if (y <= m) u = pool[u].lc, r = m;
      else u = pool[u].rc, l = m + 1;
    }
    pth[tp] = u;
    S sm = e();
    if (!g(pool[u].s + sm)) return y + 1; // the first block (the leaf) already fails
    sm = pool[u].s + sm;
    for (int i = tp - 1; i >= 0; i--) {
      int p = pth[i];
      if (pth[i + 1] != pool[p].rc) continue; // only left-sibling blocks "coming from the right child"
      int b = pool[p].lc, m = (pl[i] + pr[i]) >> 1;
      if (!b) continue;
      if (g(pool[b].s + sm)) { // fold in the whole block (new block on the left)
        sm = pool[b].s + sm;
        continue;
      }
      int w = b, lw = pl[i], rw = m; // descend inside the block for the leftmost valid position
      while (w && lw < rw) {
        if (pool[w].tag != id()) push(w);
        int mm = (lw + rw) >> 1;
        if (g(pool[pool[w].rc].s + sm)) sm = pool[pool[w].rc].s + sm, w = pool[w].lc, rw = mm;
        else w = pool[w].rc, lw = mm + 1;
      }
      if (!w) continue;
      return lw + 1; // leaf lw fails: answer is lw+1
    }
    return 1;
  }
};

/*
 * ============================================================
 * Name: custom-semigroup segment tree (dynamic nodes, lazy tags, iterative)
 * Complexity: O(log n) per operation (no recursion); node count O(#updates *
 *             log n), allocated dynamically
 * Usage: `semigroup_segtree<S, e, F, id>`: range operations over custom info
 *        and lazy tags (isomorphic to ACL lazy_segtree).
 *        S and F must define operator+: S + S (info combine), S + F (apply tag
 *        to info, S first), F + F (compose tags, "left first then right").
 *        e() / id() are the identities.
 *        Method list: see Interface below.
 * Interface (1-indexed, closed ranges [x, y]):
 *        update(x, y, z)     range update: apply tag z to [x, y]
 *        update(x, y)        point update: apply tag y at x
 *        set(x, y)           point assignment: set the info at x to y
 *        query(x, y)         range query, returns info; query(x) point (get(x) same)
 *        all_prod()          info over the whole [1, n]
 *        max_right(x, g)     max r with g(query(x, r)) true (needs g(e()) true)
 *        min_left(y, g)      min l with g(query(l, y)) true (needs g(e()) true)
 *        init(n)             empty tree (domain [1, n], may reach ~1e9)
 *        init(n, a)          ACL-style vector build: a[1..n] initial values, O(n)
 *        clear()/reserve(k)  clear / pre-allocate the node pool
 * Principle: nodes come from a dynamic pool (0 = sentinel, unbuilt subtrees
 *            count as e()); range ops descend to the split node (the unique
 *            node with x <= mid < y), then walk both boundary paths to the
 *            leaves: fully covered canonical blocks get tagged / read and
 *            updates pull back in reverse stack order
 * Notes: f + g means "f first, then g": (x + (f + g)) == ((x + f) + g).
 *        mapping must satisfy (e() + f) == e() — range-assignment semantics
 *        work, but range-add + range-sum does NOT (empty segments carry no
 *        length); for range-add either fold the length into info, or use
 *        SegmentTree_ACL.cpp.
 *        max_right / min_left follow ACL's canonical-block greedy semantics: a
 *        block that passes is not re-checked, so g should fail monotonically as
 *        the range grows.
 *        init / clear between test cases
 * ============================================================
 */
