#include <bits/stdc++.h>
using namespace std;
#define int long long

// Leftist heap: mergeable min-heap, O(log n) merge
template <size_t N>
struct leftist_heap {
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
    if (dis[ch[a][0]] < dis[ch[a][1]]) swap(ch[a][0], ch[a][1]);  // keep right spine short
    dis[a] = dis[ch[a][1]] + 1;
    return a;
  }
  int find(int x) {  // root of the heap containing x (no path compression)
    while (fa[x] != x) x = fa[x];
    return x;
  }
  int pop(int x) {  // remove root x, return the merged remainder
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
 * Complexity: merge O(log n), pop/build amortized O(log n); find climbs fa
 * Usage: mergeable priority queue, wrapped as leftist_heap<N>:
 *        node(v) makes a singleton heap, merge(h1, h2) melds, pop(h) removes
 *        the root, find(x) locates the root of x's heap; often combined with
 *        DSU-style "one heap per set" tricks (MST variants, greedy problems)
 * Principle: maintains dist (distance to the nearest null child) and always
 *        keeps the heavier side on the left (dis[left] >= dis[right]), so
 *        each merge recurses along one right spine of length O(log n)
 * Notes: find does no path compression (after melds, fa is the heap tree
 *        itself; compression would break pop's parent links); complements
 *        the STL priority_queue (not mergeable); the root holds the smallest
 *        val (min-heap; negate values for a max-heap)
 * ============================================================
 * Example (uncomment to compile):
 * static leftist_heap<100009> lh;
 * signed main() {
 *   vector<int> h;
 *   for (int x : {5, 2, 8, 3, 7}) h.push_back(lh.node(x));
 *   while (h.size() > 1) {
 *     sort(h.begin(), h.end(), [&](int a, int b) { return lh.val[a] > lh.val[b]; });
 *     int a = h.back(); h.pop_back();
 *     int b = h.back(); h.pop_back();
 *     h.push_back(lh.merge(a, b));
 *   }
 *   cout << lh.val[h[0]] << '\n';   // 2 (the overall minimum)
 * }
 * ============================================================
 */
