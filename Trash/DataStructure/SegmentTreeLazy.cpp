#include <bits/stdc++.h>
using namespace std;

// segment tree with lazy tags: range add + range sum (+ prefix-search)
template <size_t N>
struct segtree_lazy {
  int n;
  long long sgt[N << 2], tg[N << 2];  // sgt = range sum, tg = lazy add
  void apply(int u, int l, int r, long long x) { sgt[u] += (r - l + 1) * x, tg[u] += x; }
  void down(int u, int l, int r) {
    if (tg[u]) {
      int m = (l + r) >> 1;
      apply(u << 1, l, m, tg[u]), apply(u << 1 | 1, m + 1, r, tg[u]);
      tg[u] = 0;
    }
  }
  void update(int u, int l, int r, int x, int y, int z) {  // a[x..y] += z
    if (x <= l && r <= y) return apply(u, l, r, z);
    int m = (l + r) >> 1;
    down(u, l, r);
    if (m >= x) update(u << 1, l, m, x, y, z);
    if (m < y) update(u << 1 | 1, m + 1, r, x, y, z);
    sgt[u] = sgt[u << 1] + sgt[u << 1 | 1];
  }
  // first index whose prefix sum >= k (values assumed non-negative)
  int find(int u, int l, int r, long long k) {
    if (l == r) return l;
    int m = (l + r) >> 1;
    down(u, l, r);
    if (sgt[u << 1] >= k) return find(u << 1, l, m, k);
    else return find(u << 1 | 1, m + 1, r, k - sgt[u << 1]);
  }
  long long query(int u, int l, int r, int x, int y) {  // sum(a[x..y])
    if (x <= l && r <= y) return sgt[u];
    int m = (l + r) >> 1;
    long long res = 0;
    down(u, l, r);
    if (m >= x) res += query(u << 1, l, m, x, y);
    if (m < y) res += query(u << 1 | 1, m + 1, r, x, y);
    return res;
  }
};

/*
 * ============================================================
 * Name: segment tree (range add + range sum, lazy tags)
 * Complexity: build/update/query all O(log n)
 * Usage: range add, range sum, and find locating the first prefix sum >= k,
 *        wrapped as segtree_lazy<N>:
 *        st.update(1, 1, n, l, r, z) adds z to a[l..r];
 *        st.query(1, 1, n, l, r) sums a[l..r];
 *        st.find(1, 1, n, k) finds the first prefix sum >= k
 * Source: all.cpp lines 48453-48484 (range-add + range-sum version with find;
 *         wrapped into a struct); the original had no build (global arrays are
 *         zero-initialized, update works directly); lines 12453-12489 of the
 *         original are a range-add + range-max variant if needed
 * ============================================================
 * Example (uncomment to compile):
 * static segtree_lazy<300009> st;
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   int n = 5;
 *   st.update(1, 1, n, 2, 4, 3);            // a = {0,3,3,3,0}
 *   cout << st.query(1, 1, n, 1, 5) << '\n'; // 9
 *   cout << st.query(1, 1, n, 3, 3) << '\n'; // 3
 *   cout << st.find(1, 1, n, 7) << '\n';     // first prefix sum >= 7 -> 4
 *   return 0;
 * }
 * ============================================================
 */
