#include <bits/stdc++.h>
using namespace std;
#define int long long

// Dinic core + flows with lower bounds (feasible / max / min flow)
template <size_t N, size_t M>
struct bounded_flow {
  static constexpr int INF = 0x3f3f3f3f3f3f3f3f;
  int hd[N], nxt[M], to[M], cap[M], tot = 1, cur[N], dep[N], q[N], nv = 2;
  int in[N];  // lower-bound balance: in[x] = sum(in-lower) - sum(out-lower)
  void init() {
    tot = 1, nv = 2;
    memset(hd, 0, sizeof hd), memset(in, 0, sizeof in);
  }
  void add(int x, int y, int c) {
    nxt[++tot] = hd[x], hd[x] = tot, to[tot] = y, cap[tot] = c;
    nxt[++tot] = hd[y], hd[y] = tot, to[tot] = x, cap[tot] = 0;
    if (x >= nv) nv = x + 1;
    if (y >= nv) nv = y + 1;  // track real vertex count to shrink memset/memcpy
  }
  bool bfs(int s, int t) {
    memset(dep, -1, nv * sizeof(int));
    int qh = 0, qt = 0;  // flat array queue
    q[qt++] = s, dep[s] = 0;
    while (qh < qt) {
      int x = q[qh++], dx = dep[x] + 1;
      for (int i = hd[x]; i; i = nxt[i])
        if (cap[i] && dep[to[i]] == -1) dep[to[i]] = dx, q[qt++] = to[i];
    }
    return dep[t] != -1;
  }
  int dfs(int x, int t, int w) {
    if (x == t) return w;
    int flow = 0, dx = dep[x] + 1;
    for (int& i = cur[x]; i && w; i = nxt[i]) {
      int v = to[i];
      if (cap[i] && dep[v] == dx) {
        int k = dfs(v, t, min(w, cap[i]));
        flow += k, w -= k, cap[i] -= k, cap[i ^ 1] += k;
      }
    }
    return flow;
  }
  int maxflow(int s, int t) {
    int r = 0;
    while (bfs(s, t)) {
      memcpy(cur, hd, nv * sizeof(int));
      r += dfs(s, t, INF);
    }
    return r;
  }
};

/*
 * ============================================================
 * Name: flows with lower bounds (feasible / max / min flow)
 * Complexity: O(Dinic) = O(V^2 E) worst case, much faster in practice
 * Usage: networks whose edges carry lower bounds [l, r], `bounded_flow<N, M>`
 *        (Dinic's add / maxflow plus the balance array in[]):
 *        circulation (no fixed source / sink): a feasible flow with every
 *        edge's flow in [l, r]; max / min flow with fixed source / sink
 *        respecting the lower bounds.
 *        Edge (u, v, l, r): add(u, v, r - l) plus in[u] -= l, in[v] += l.
 * Principle: split each edge into mandatory l and optional [0, r-l]; balance
 *            every vertex's in/out difference with a super source / sink (in[x]
 *            > 0 adds S->x with cap in[x], else x->T with cap -in[x]); S->T
 *            saturating flow means feasible; for fixed SS / TT add an INF edge
 *            TT->SS to reduce to a circulation (the flow on it is the min
 *            feasible flow), then augment SS->TT for the maximum
 * Notes: init() between test cases; SS / TT are the original source / sink, S /
 *        T the super source / sink
 * ============================================================
 */
