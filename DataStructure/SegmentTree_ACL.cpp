#include <bits/stdc++.h>
using namespace std;

// faithful port of ACL's lazy_segtree (function-by-function alignment with atcoder/lazysegtree.hpp):
// 0-indexed, half-open [l, r), static full 2^k array layout, fully iterative
template <class S, S (*op)(S, S), S (*e)(), class F, S (*mapping)(F, S), F (*composition)(F, F), F (*id)()>
struct lazy_segtree {
  lazy_segtree() : lazy_segtree(0) {}
  explicit lazy_segtree(int n_) : lazy_segtree(vector<S>(n_, e())) {}
  explicit lazy_segtree(const vector<S>& v) : _n((int)v.size()) {
    size = 1, log = 0;
    while (size < _n) size <<= 1, ++log;  // bit_ceil
    d = vector<S>(2 * size, e());
    lz = vector<F>(size, id());
    for (int i = 0; i < _n; i++) d[size + i] = v[i];
    for (int i = size - 1; i >= 1; i--) update(i);
  }

  void set(int p, S x) {  // point assignment d[p] = x
    p += size;
    for (int i = log; i >= 1; i--) push(p >> i);
    d[p] = x;
    for (int i = 1; i <= log; i++) update(p >> i);
  }
  S get(int p) {  // point query
    p += size;
    for (int i = log; i >= 1; i--) push(p >> i);
    return d[p];
  }
  S prod(int l, int r) {  // op(a[l..r)); empty range returns e()
    if (l == r) return e();
    l += size, r += size;
    for (int i = log; i >= 1; i--) {  // push only the ancestors relevant to the boundaries
      if (((l >> i) << i) != l) push(l >> i);
      if (((r >> i) << i) != r) push((r - 1) >> i);
    }
    S sml = e(), smr = e();
    while (l < r) {  // fold the canonical decomposition bottom-up
      if (l & 1) sml = op(sml, d[l++]);
      if (r & 1) smr = op(d[--r], smr);
      l >>= 1, r >>= 1;
    }
    return op(sml, smr);
  }
  S all_prod() { return d[1]; }

  void apply(int p, F f) {  // point apply
    p += size;
    for (int i = log; i >= 1; i--) push(p >> i);
    d[p] = mapping(f, d[p]);
    for (int i = 1; i <= log; i++) update(p >> i);
  }
  void apply(int l, int r, F f) {  // range [l, r) apply
    if (l == r) return;
    l += size, r += size;
    for (int i = log; i >= 1; i--) {
      if (((l >> i) << i) != l) push(l >> i);
      if (((r >> i) << i) != r) push((r - 1) >> i);
    }
    int l2 = l, r2 = r;  // fully covered nodes are tagged directly
    while (l < r) {
      if (l & 1) all_apply(l++, f);
      if (r & 1) all_apply(--r, f);
      l >>= 1, r >>= 1;
    }
    l = l2, r = r2;
    for (int i = 1; i <= log; i++) {  // pull the boundary ancestors back
      if (((l >> i) << i) != l) update(l >> i);
      if (((r >> i) << i) != r) update((r - 1) >> i);
    }
  }

  template <bool (*g)(S)>
  int max_right(int l) { return max_right(l, [](S x) { return g(x); }); }
  template <class G>  // max r with g(prod(l, r)) true; requires g(e()) true
  int max_right(int l, G g) {
    if (l == _n) return _n;
    l += size;
    for (int i = log; i >= 1; i--) push(l >> i);
    S sm = e();
    do {
      while (l % 2 == 0) l >>= 1;
      if (!g(op(sm, d[l]))) {          // this canonical block fails: descend inside
        while (l < size) {
          push(l);
          l = 2 * l;
          if (g(op(sm, d[l]))) sm = op(sm, d[l]), l++;
        }
        return l - size;
      }
      sm = op(sm, d[l]);
      l++;
    } while ((l & -l) != l);
    return _n;
  }

  template <bool (*g)(S)>
  int min_left(int r) { return min_left(r, [](S x) { return g(x); }); }
  template <class G>  // min l with g(prod(l, r)) true; requires g(e()) true
  int min_left(int r, G g) {
    if (r == 0) return 0;
    r += size;
    for (int i = log; i >= 1; i--) push((r - 1) >> i);
    S sm = e();
    do {
      r--;
      while (r > 1 && (r % 2)) r >>= 1;
      if (!g(op(d[r], sm))) {
        while (r < size) {
          push(r);
          r = 2 * r + 1;
          if (g(op(d[r], sm))) sm = op(d[r], sm), r--;
        }
        return r + 1 - size;
      }
      sm = op(d[r], sm);
    } while ((r & -r) != r);
    return 0;
  }

 private:
  int _n, size, log;
  vector<S> d;
  vector<F> lz;

  void update(int k) { d[k] = op(d[2 * k], d[2 * k + 1]); }
  void all_apply(int k, F f) {
    d[k] = mapping(f, d[k]);
    if (k < size) lz[k] = composition(f, lz[k]);
  }
  void push(int k) {
    all_apply(2 * k, lz[k]);
    all_apply(2 * k + 1, lz[k]);
    lz[k] = id();
  }
};

/*
 * ============================================================
 * Name: lazy segment tree, ACL style (static full tree, generic op/mapping, fully iterative)
 * Complexity: build O(n); set/get/prod/apply/max_right/min_left all O(log n)
 * Usage: a general segment tree fully isomorphic to AtCoder Library's
 *        lazy_segtree: op/e maintain the monoid info S;
 *        mapping/composition/id maintain the monoid action F; template
 *        parameters and interface align with ACL one by one (0-indexed,
 *        half-open [l, r)): set(p,x) get(p) prod(l,r) all_prod() apply(p,f)
 *        apply(l,r,f) max_right(l,g) min_left(r,g) (g must satisfy g(e())
 *        true; the semantics are greedy over canonical blocks — a whole block
 *        passing means skip it; best for g that fails monotonically as the
 *        range grows)
 * Trade-offs vs SegmentTree_Semigroup.cpp (dynamic-node version):
 *        this version: static full tree (n must be fully buildable), does NOT
 *             require mapping(f, e()) == e(), so segment-length-dependent
 *             actions like "range affine + range sum" work — ACL semantics;
 *        dynamic version: value domain up to ~1e9, but the action must be
 *             correct on the "implicit empty segment e()"
 * Principle: leaves at [size, 2*size) (size = least power of two >= n), full
 *        binary tree in an array; prod/apply first push lazy tags down along
 *        both boundaries from level log, then fold/tag bottom-up; set/get
 *        push along the single path then write/read directly; max_right/
 *        min_left greedily walk canonical blocks from the leaf, binary-
 *        descending inside the failing block (no recursion)
 * Notes: for point-only updates degenerate to the identity action (or use the
 *        zkw-style IterativeLazy); with n == 0 only all_prod/prod(l,l) are
 *        legal; prod on an empty range returns e()
 * ============================================================
 * Example (uncomment to compile; range affine + range sum, the classic case the dynamic version cannot do):
 * using S = pair<long long, long long>;  // (sum, length)
 * using F = pair<long long, long long>;  // a*x + b
 * S op(S a, S b) { return {a.first + b.first, a.second + b.second}; }
 * S e() { return {0, 0}; }
 * F id() { return {1, 0}; }
 * S mapping(F f, S s) { return {f.first * s.first + f.second * s.second, s.second}; }
 * F composition(F f, F g) {  // g first, then f: f.(g.x) = f.a*(g.a*x+g.b)+f.b
 *   return {f.first * g.first, f.first * g.second + f.second};
 * }
 * lazy_segtree<S, op, e, F, mapping, composition, id> st;
 * signed main() {
 *   vector<S> a{{1, 1}, {2, 1}, {3, 1}};    // {1,2,3}
 *   st = lazy_segtree<S, op, e, F, mapping, composition, id>(a);
 *   st.apply(1, 3, {2, 1});                 // a = {1,5,7}
 *   cout << st.prod(0, 3).first << '\n';    // 13
 *   st.set(0, {10, 1});                     // a = {10,5,7}
 *   cout << st.get(0).first << ' ' << st.prod(2, 3).first << '\n';  // 10 7
 *   cout << st.max_right(0, [](S s) { return s.first <= 11; }) << '\n';  // 1 (prefix sum 15 exceeds)
 *   cout << st.min_left(3, [](S s) { return s.first <= 11; }) << '\n';   // 2 (suffix sum 12 exceeds)
 * }
 */
