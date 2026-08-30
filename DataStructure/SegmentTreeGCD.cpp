#include <bits/stdc++.h>
using namespace std;
#define int long long

// segment tree maintaining gcd: point update + prefix gcd query
template <size_t N>
struct segtree_gcd {
  int n, a[N], sgt[N << 2];
  void build(int u, int l, int r) {
    if (l == r) return sgt[u] = a[l], void();
    int m = (l + r) >> 1;
    build(u << 1, l, m), build(u << 1 | 1, m + 1, r);
    sgt[u] = __gcd(sgt[u << 1], sgt[u << 1 | 1]);
  }
  void update(int u, int l, int r, int x, int y) {  // a[x] = y
    if (l == r) return sgt[u] = y, void();
    int m = (l + r) >> 1;
    if (m >= x) update(u << 1, l, m, x, y);
    else update(u << 1 | 1, m + 1, r, x, y);
    sgt[u] = __gcd(sgt[u << 1], sgt[u << 1 | 1]);
  }
  // gcd of a[1..x] (prefix gcd)
  int query(int u, int l, int r, int x) {
    if (r <= x) return sgt[u];
    int m = (l + r) >> 1, ans = query(u << 1, l, m, x);
    if (m < x) ans = __gcd(ans, query(u << 1 | 1, m + 1, r, x));
    return ans;
  }
};

/*
 * ============================================================
 * Name: segment tree for gcd
 * Complexity: build O(n); point update O(log n); query O(log n)
 * Usage: point update + range gcd, `segtree_gcd<N>`: fill n and a[1..n],
 *        build(1, 1, n);
 *        update(1, 1, n, x, y) sets a[x] = y; query(1, 1, n, x) is the prefix
 *        gcd, i.e. gcd(a[1..x]).
 * Source: all.cpp lines 53170-53191 (wrapped into a struct, logic unchanged)
 * Notes: 1-indexed
 * ============================================================
 */
