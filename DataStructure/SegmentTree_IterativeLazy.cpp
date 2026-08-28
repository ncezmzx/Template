#include <bits/stdc++.h>
using namespace std;
#define int long long

// iterative (zkw-style) segment tree: range add + range sum with lazy tags
template <size_t N>
struct segtree_iterative_lazy {
  int n, sz, H;
  long long sum[N << 1], tag[N << 1], len[N << 1];
  void init(int n_) {
    n = n_;
    sz = 1, H = 0;
    while (sz < n) sz <<= 1, ++H;
    for (int i = 1; i <= sz * 2; ++i) sum[i] = tag[i] = 0;
    for (int i = sz; i < sz + n; ++i) len[i] = 1;
    for (int i = sz - 1; i >= 1; --i) len[i] = len[i << 1] + len[i << 1 | 1];
  }
  void apply(int p, long long v) {
    sum[p] += v * len[p];
    tag[p] += v;
  }
  void push(int p) {
    if (tag[p]) apply(p << 1, tag[p]), apply(p << 1 | 1, tag[p]), tag[p] = 0;
  }
  void pull(int p) { sum[p] = sum[p << 1] + sum[p << 1 | 1] + tag[p] * len[p]; }
  void range_add(int l, int r, long long v) {  // 1-indexed inclusive
    l += sz - 1, r += sz - 1;
    int l0 = l, r0 = r;
    for (int i = H; i >= 1; --i) {
      push(l0 >> i);
      if ((r0 >> i) != (l0 >> i)) push(r0 >> i);
    }
    while (l <= r) {
      if (l & 1) apply(l++, v);
      if (!(r & 1)) apply(r--, v);
      l >>= 1, r >>= 1;
    }
    for (int i = 1; i <= H; ++i) {
      pull(l0 >> i);
      if ((r0 >> i) != (l0 >> i)) pull(r0 >> i);
    }
  }
  long long range_sum(int l, int r) {
    l += sz - 1, r += sz - 1;
    int l0 = l, r0 = r;
    for (int i = H; i >= 1; --i) {
      push(l0 >> i);
      if ((r0 >> i) != (l0 >> i)) push(r0 >> i);
    }
    long long res = 0;
    while (l <= r) {
      if (l & 1) res += sum[l++];
      if (!(r & 1)) res += sum[r--];
      l >>= 1, r >>= 1;
    }
    return res;
  }
  void point_set(int x, long long v) {  // a[x] = v via delta
    range_add(x, x, v - range_sum(x, x));
  }
};

/*
 * ============================================================
 * Name: iterative segment tree (zkw style, range add + range sum, lazy tags)
 * Complexity: range update / query O(log n), recursion-free (small constants)
 * Usage: range add, range sum, point assignment (point_set via delta),
 *        wrapped as segtree_iterative_lazy<N>: init(n) then range_add /
 *        range_sum / point_set; a faster alternative to recursive segment trees
 * Principle: full binary tree in an array (sz = least power of two >= n);
 *        len[p] stores each node's segment length; before update/query, push
 *        lazy tags along the boundary root-to-leaf paths, tag fully covered
 *        nodes directly, then pull the paths back
 * Notes: indices 1..n; n may be any positive integer (padding segments have
 *        len = 0 and contribute nothing); change apply/pull to support other
 *        additive info (e.g. range max with adjusted tag semantics)
 * ============================================================
 * Example (uncomment to compile):
 * static segtree_iterative_lazy<400009> st;
 * signed main() {
 *   st.init(5);
 *   st.range_add(2, 4, 3);                  // a = {0,3,3,3,0}
 *   cout << st.range_sum(1, 5) << '\n';     // 9
 *   st.point_set(3, 10);                    // a = {0,3,10,3,0}
 *   cout << st.range_sum(1, 5) << '\n';     // 16
 *   cout << st.range_sum(3, 3) << '\n';     // 10
 * }
 * ============================================================
 */
