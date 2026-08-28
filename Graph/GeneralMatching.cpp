#include <bits/stdc++.h>
using namespace std;

// general graph maximum matching (blossom algorithm), O(n^3); 1-indexed
// colors: col 1 = outer (black, queued, includes root), col 2 = inner (white)
constexpr int N = 509;
struct Blossom {
  int n, par[N], match_[N], pre[N], col[N], q_[N], hd, tl;
  vector<int> g[N];
  void init(int n_) {
    n = n_;
    for (int i = 0; i <= n; ++i) g[i].clear();
    fill(match_ + 1, match_ + n + 1, 0);
  }
  void add(int u, int v) { g[u].push_back(v), g[v].push_back(u); }
  int find_(int x) { return par[x] == x ? x : par[x] = find_(par[x]); }
  int lca(int x, int y) {  // first common point of the two alternating chains (blossom base)
    static int tim = 0, vis[N] = {0};
    ++tim;
    for (;; swap(x, y))
      if (x) {
        x = find_(x);
        if (vis[x] == tim) return x;
        vis[x] = tim;
        x = pre[match_[x]];  // outer -> pre of its inner mate (outer again); ends at root
      }
  }
  void blossom(int x, int y, int l) {  // walk x's chain to base l: redirect pre, contract
    while (find_(x) != l) {
      pre[x] = y;                    // outer x's pre points to the other side
      int w = match_[x];             // inner mate
      if (col[w] == 2) col[w] = 1, q_[tl++] = w;  // inner vertices become outer, enqueued
      if (find_(x) == x) par[x] = l;  // contract into blossom l
      if (find_(w) == w) par[w] = l;
      y = w;
      x = pre[w];                    // climb the chain
    }
  }
  bool augment(int s) {  // find an augmenting path from free vertex s
    for (int i = 1; i <= n; ++i) par[i] = i, col[i] = 0;
    hd = tl = 0, q_[tl++] = s, col[s] = 1;
    while (hd < tl) {
      int u = q_[hd++];
      for (int v : g[u]) {
        if (find_(u) == find_(v) || col[v] == 2) continue;  // same blossom / inner (even cycle): skip
        if (col[v] == 1) {  // outer-outer edge: odd cycle -> contract blossom
          int l = lca(u, v);
          blossom(u, v, l), blossom(v, u, l);
        } else if (!match_[v]) {  // free: flip along the pre chain
          pre[v] = u;
          for (int x = v; x;) {
            int y = pre[x], z = match_[y];
            match_[x] = y, match_[y] = x;
            x = z;
          }
          return true;
        } else if (!col[v]) {  // uncolored: v inner, its mate outer and enqueued
          pre[v] = u, col[v] = 2;
          if (!col[match_[v]]) col[match_[v]] = 1, q_[tl++] = match_[v];
        }
      }
    }
    return false;
  }
  int solve() {  // max matching size; match_[i] = partner of i (0 = free)
    int res = 0;
    for (int i = 1; i <= n; ++i)
      if (!match_[i] && augment(i)) ++res;
    return res;
  }
};

/*
 * ============================================================
 * ============================================================
 * Name: general graph maximum matching (blossom algorithm)
 * Complexity: O(n^3) (much faster in practice)
 * Usage: maximum matching on non-bipartite graphs (odd cycles), wrapped as
 *        struct Blossom: solve() returns the matching size; match_[i] is the
 *        partner of i (0 = unmatched)
 * Principle: BFS alternating tree (col 1 outer/black = queued side incl.
 *        root, col 2 inner/white) searches augmenting paths; adjacent
 *        outer-outer pairs form odd cycles -> contract blossoms (DSU par),
 *        promoting inner blossom vertices to outer; lca climbs the
 *        pre/match chains with timestamps to find the blossom base; hitting
 *        a free vertex flips along pre. Outer-inner adjacency is always an
 *        even cycle and is skipped
 * Source: rewritten against the classic the-tourist/algo flows/blossom.cpp
 * Notes: callers should avoid adding self-loops/duplicate edges; adjacency
 *        is stored undirected and symmetric
 * ============================================================
 * Example (uncomment to compile):

 * signed main() {
 *   Blossom bm;                       // pentagon 1-2-3-4-5-1 (odd cycle)
 *   bm.init(5);
 *   bm.add(1, 2), bm.add(2, 3), bm.add(3, 4), bm.add(4, 5), bm.add(5, 1);
 *   cout << bm.solve() << '\n';       // 2
 *   bm.init(4);                       // triangle + isolated vertex
 *   bm.add(1, 2), bm.add(2, 3), bm.add(3, 1);
 *   cout << bm.solve() << '\n';       // 1
 * }
 */
