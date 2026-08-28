#include <bits/stdc++.h>
using namespace std;

#define int long long
constexpr int N = 5e3 + 9, M = 5e4 + 9, inf = 0x3f3f3f3f3f3f3f3f;
struct mcmf {
  int hd[N], nxt[M * 2], to[M * 2], cap[M * 2], cst[M * 2], tot = 1, dst[N], cur[N], vst[N];
  int q[N + 2], nv = 2;   // ring queue (N+2 slots) + real vertex count
  void add(int x, int y, int z, int c) {
    nxt[++tot] = hd[x], hd[x] = tot, to[tot] = y, cap[tot] = z, cst[tot] = c;
    nxt[++tot] = hd[y], hd[y] = tot, to[tot] = x, cap[tot] = 0, cst[tot] = -c;
    if (x >= nv) nv = x + 1;
    if (y >= nv) nv = y + 1;
  }
  pair<int, int> dfs(int x, int t, int w) {
    if (x == t) return {w, 0};
    vst[x] = 1;
    int flow = 0, cost = 0, dx = dst[x];
    for (int i = cur[x]; i && w; i = nxt[i]) {
      cur[x] = i;
      if (vst[to[i]]) continue;
      int cp = min(cap[i], w);
      if (cp > 0 && dx + cst[i] == dst[to[i]]) {
        auto [f, cs] = dfs(to[i], t, cp);
        flow += f, w -= f, cost += f * cst[i] + cs;
        cap[i] -= f, cap[i ^ 1] += f;
      }
    }
    vst[x] = 0;
    if (!flow) dst[x] = -1;
    return {flow, cost};
  }
  pair<int, int> calc(int s, int t) {
    int flow = 0, cost = 0;
    while (true) {
      memset(dst, 0x3f, nv * sizeof(int));
      memcpy(cur, hd, nv * sizeof(int));
      int qh = 0, qt = 0;   // ring array queue; vst keeps at most N+1 queued
      q[qt] = s, dst[s] = 0;
      if (++qt > N + 1) qt = 0;
      while (qh != qt) {
        int x = q[qh];
        if (++qh > N + 1) qh = 0;
        vst[x] = false;
        int dx = dst[x];
        for (int i = hd[x]; i; i = nxt[i]) {
          int nd = dx + cst[i];
          if (cap[i] > 0 && nd < dst[to[i]]) {
            dst[to[i]] = nd;
            if (!vst[to[i]]) {
              vst[to[i]] = true;
              q[qt] = to[i];
              if (++qt > N + 1) qt = 0;
            }
          }
        }
      }
      if (dst[t] > 1e9) return {flow, cost};
      auto [x, y] = dfs(s, t, inf);
      flow += x, cost += y;
    }
  }
};

/*
 * ============================================================
 * ============================================================
 * Name: min-cost max-flow (MCMF: SPFA shortest paths + push along them)
 * Complexity: each SPFA O(VE); O(F) augmentation rounds (F = flow value);
 *             overall O(F * V * E) territory
 * Usage: maximum flow with minimum total cost, wrapped as struct mcmf; the
 *        static array version needs N / M tuned per problem (original
 *        problem N = 5e3+9, M = 5e4+9); #define int long long pairs with
 *        inf = 0x3f3f3f3f3f3f3f3f
 * Source: all.cpp lines 28964-29015 (kept verbatim; the original declared a
 *         global instance "} e;" — declare your own mcmf e;)
 * ============================================================
 * Example (uncomment to compile):

 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   int n, m, s, t;
 *   cin >> n >> m >> s >> t;
 *   static mcmf e;   // ~2MB instance: static storage avoids stack overflow (global in the original)
 *   for (int i = 1; i <= m; ++i) {
 *     int x, y, z, c;
 *     cin >> x >> y >> z >> c;
 *     e.add(x, y, z, c);
 *   }
 *   auto [x, y] = e.calc(s, t);
 *   cout << x << ' ' << y;   // max flow and min cost
 *   cout.flush();            // demo: flush buffered output
 * }
 * ============================================================
 */
