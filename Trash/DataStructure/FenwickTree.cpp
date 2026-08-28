#include <bits/stdc++.h>
using namespace std;
#define int long long

// Fenwick tree / binary indexed tree: point add + prefix sum
template <size_t N>
struct fenwick {
  int n;  // index upper bound
  int c[N];
  void update(int x, int y) { for (; x <= n; x += x & -x) c[x] += y; }
  int query(int x, int r = 0) { for (; x > 0; x -= x & -x) r += c[x]; return r; }
};

/*
 * ============================================================
 * Name: Fenwick tree / binary indexed tree (BIT)
 * Complexity: point add O(log n), prefix sum O(log n)
 * Usage: point add + prefix sum, wrapped as fenwick<N>:
 *        set bit.n (index upper bound), then bit.update(i, v) adds v at i,
 *        bit.query(r) returns sum(a[1..r]); range sum = query(r) - query(l - 1)
 * Source: all.cpp lines 6-10 (struct fenwick, classic version; wrapped and the
 *         2*n upper bound from the original problem simplified to a member n)
 * ============================================================
 * Example (uncomment to compile):
 * static fenwick<200009> bit;
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   bit.n = 10;
 *   bit.update(1, 3), bit.update(3, 4), bit.update(4, 2);
 *   // a = {3,0,4,2,0,...}
 *   cout << bit.query(4) << '\n';          // 9
 *   cout << bit.query(4) - bit.query(1) << '\n';  // range [2,4] sum = 6
 *   return 0;
 * }
 * ============================================================
 */
