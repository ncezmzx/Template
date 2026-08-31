#include <bits/stdc++.h>
using namespace std;
#define int long long

// Leftist heap: mergeable min-heap, O(log n) merge
template <size_t N> struct leftist_heap {
  int tot;
  int val[N], dis[N], ch[N][2], fa[N];
  int node(int v) {
    int x = ++tot;
    val[x] = v, dis[x] = 1, fa[x] = x;
    return x;
  }
  int merge(int a, int b) {
    if (!a) return b;
    if (!b) return a;
    if (val[a] > val[b]) swap(a, b);
    ch[a][1] = merge(ch[a][1], b);
    if (ch[a][1]) fa[ch[a][1]] = a;
    if (dis[ch[a][0]] < dis[ch[a][1]]) swap(ch[a][0], ch[a][1]); // keep right spine short
    dis[a] = dis[ch[a][1]] + 1;
    return a;
  }
  int find(int x) { // root of the heap containing x (no path compression)
    while (fa[x] != x) x = fa[x];
    return x;
  }
  int pop(int x) { // remove root x, return the merged remainder
    int l = ch[x][0], r = ch[x][1];
    ch[x][0] = ch[x][1] = 0, dis[x] = 1;
    if (l) fa[l] = l;
    if (r) fa[r] = r;
    return merge(l, r);
  }
};

/*
 * ============================================================
 * Name: leftist heap (mergeable min-heap)
 * Complexity: merge O(log n); pop / build amortized O(log n); find climbs fa
 * Usage: mergeable min-heap, `leftist_heap<N>`: node(v) singleton; merge(h1,
 *        h2) melds; pop(h) removes the root; find(x) root of x's heap.
 *        A heap is identified by its root node (h is passed by reference);
 *        empty heap = 0.
 * Principle: keeps dis[left] >= dis[right], so a merge only recurses down one
 *            right spine of length O(log n)
 * Notes: find does no path compression (after melds fa is the heap tree itself,
 *        compression would break pop's parent links);
 *        the root holds the smallest val (negate values for a max-heap);
 *        complements std::priority_queue, which is not mergeable
 * ============================================================
 */
