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
 * ============================================================
 * Name: flows with lower bounds (feasible / max / min flow)
 * Complexity: O(Dinic) = O(V^2 E) worst case, much faster in practice
 * Usage: networks whose edges carry flow lower bounds [l, r], wrapped as
 *        bounded_flow<N, M> (contains Dinic's add/maxflow plus the balance
 *        array in[]):
 *        1) circulation (no fixed source/sink): a feasible flow with every
 *           edge's flow in [l, r];
 *        2) max/min flow with fixed source/sink, respecting the lower bounds
 * Principle: split each edge into "mandatory l" and "optional [0, r-l]":
 *        force l first, balance every vertex's in/out difference with a super
 *        source/sink (in[x] = sum of in-lowers - sum of out-lowers;
 *        in[x] > 0 adds S->x with cap in[x], else x->T with cap -in[x]);
 *        S->T saturating flow = feasible; for fixed SS/TT add an INF edge
 *        TT->SS to reduce to circulation — the flow on that edge is the min
 *        feasible flow; then augment SS->TT for the maximum
 * Notes: init() between test cases; SS/TT = original source/sink, S/T =
 *        super source/sink; edge (u,v,l,r) needs add(u, v, r-l) plus
 *        in[u] -= l, in[v] += l
 * ============================================================
 * Example (uncomment to compile; fixed-source/sink max flow, P5198 style):

 * static bounded_flow<100009, 1000009> bf;
 * signed main() {
 *   int n, m, SS, TT;
 *   cin >> n >> m >> SS >> TT;
 *   bf.init();
 *   for (int i = 1; i <= m; ++i) {
 *     int u, v, l, r;
 *     cin >> u >> v >> l >> r;
 *     bf.in[u] -= l, bf.in[v] += l;        // force the lower bound
 *     bf.add(u, v, r - l);                 // excess capacity
 *   }
 *   int S = n + 1, T = n + 2, sum = 0;
 *   for (int i = 1; i <= n; ++i) {
 *     if (bf.in[i] > 0) bf.add(S, i, bf.in[i]), sum += bf.in[i];
 *     else if (bf.in[i] < 0) bf.add(i, T, -bf.in[i]);
 *   }
 *   bf.add(TT, SS, bf.INF);                // fixed source/sink -> circulation
 *   if (bf.maxflow(S, T) != sum) return cout << "NO\n", 0;
 *   int flow = 0;
 *   for (int i = bf.hd[SS]; i; i = bf.nxt[i])
 *     if (bf.to[i] == TT) flow = bf.cap[i];      // flow on the TT->SS edge = min feasible flow
 *   flow += bf.maxflow(SS, TT);                  // keep augmenting for the max flow
 *   cout << flow << '\n';
 * }
 * ============================================================
 */
