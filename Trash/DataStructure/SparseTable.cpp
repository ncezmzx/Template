#include <bits/stdc++.h>
using namespace std;

// sparse table: static range minimum RMQ
template <size_t N>
struct sparse_table {
  int n;
  int a[N], d[N][20], lg[N];
  void init(int n_) {
    n = n_;
    lg[0] = -1;
    for (int i = 1; i <= n; ++i) d[i][0] = a[i], lg[i] = lg[i >> 1] + 1;
    for (int j = 1; (1 << j) <= n; ++j)
      for (int i = 1; i + (1 << (j - 1)) <= n; ++i)
        d[i][j] = min(d[i][j - 1], d[i + (1 << (j - 1))][j - 1]);
  }
  int query(int l, int r) {
    int k = lg[r - l + 1];
    return min(d[l][k], d[r - (1 << k) + 1][k]);
  }
};

/*
 * ============================================================
 * Name: sparse table (ST table, static RMQ, here range minimum)
 * Complexity: preprocessing O(n log n), query O(1)
 * Usage: static range minimum queries (no updates), wrapped as
 *        sparse_table<N>: fill a[1..n], init(n), query(l, r);
 *        swap min for max to get range maxima
 * Source: all.cpp lines 27805-27821 (struct ST_Table, originally built over a
 *         suffix array's height array; wrapped and self-contained here)
 * ============================================================
 * Example (uncomment to compile):
 * static sparse_table<200005> st;
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   st.a[1] = 3, st.a[2] = 1, st.a[3] = 4, st.a[4] = 1, st.a[5] = 5;
 *   st.init(5);
 *   cout << st.query(1, 5) << '\n';  // 1
 *   cout << st.query(2, 3) << '\n';  // 1
 *   cout << st.query(3, 5) << '\n';  // 1
 *   return 0;
 * }
 * ============================================================
 */
