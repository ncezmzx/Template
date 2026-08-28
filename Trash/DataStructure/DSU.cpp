#include <bits/stdc++.h>
using namespace std;
#define int long long

// disjoint set union with path compression (merge x into y's set)
template <size_t N>
struct dsu {
  int fa[N];
  dsu() { iota(fa, fa + N, 0); }
  int get(int x) { return x == fa[x] ? x : fa[x] = get(fa[x]); }
  bool mer(int x, int y) {  // x -> y; true if they were separate
    if ((x = get(x)) == (y = get(y))) return false;
    return fa[x] = y, true;
  }
};

/*
 * ============================================================
 * Name: disjoint set union (DSU, path compression, merge by direction)
 * Complexity: near O(alpha(n)) (inverse Ackermann)
 * Usage: connectivity / set merging, wrapped as dsu<N>:
 *        get(x) finds the root, mer(x, y) merges x's set into y's
 * Source: all.cpp lines 55992-56000 (templatized, logic unchanged)
 * ============================================================
 * Example (uncomment to compile):
 * static dsu<300009> d;   // static: fa[] is ~2.4MB, keep it off the stack
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   d.mer(1, 2), d.mer(2, 3);
 *   cout << d.get(3) << ' ' << d.get(1) << '\n';  // 3 3
 *   cout << d.mer(1, 3) << '\n';                  // 0 (already in one set)
 *   d.mer(4, 5);
 *   cout << d.get(4) << ' ' << d.get(5) << '\n';  // 5 5
 *   return 0;
 * }
 * ============================================================
 */
