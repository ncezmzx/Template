#include <bits/stdc++.h>
using namespace std;
#define int long long

// Hopcroft-Karp maximum bipartite matching, O(E sqrt V)
template <size_t N>
struct hopcroft_karp {
  int nl, nr, vstamp = 0;
  int dis[N], match[N], vis[N], q[N];
  vector<int> g[N];  // adjacency of left vertices (right ids must not collide)
  bool bfs() {  // layer free-left vertices by augmenting-path length
    bool f = false;
    int qh = 0, qt = 0;  // flat queue, left vertices only
    for (int i = 1; i <= nl; ++i)
      if (!match[i]) dis[i] = 0, q[qt++] = i;
      else dis[i] = -1;
    dis[0] = -1;
    while (qh < qt) {
      int u = q[qh++], du = dis[u] + 1;
      for (int v : g[u]) {
        if (dis[match[v]] == -1) {
          dis[match[v]] = du;
          if (match[v]) q[qt++] = match[v];
          else f = true;
        }
      }
    }
    return f;
  }
  bool dfs(int u) {
    int du = dis[u] + 1;  // hoisted level check
    for (int v : g[u]) {
      if (match[v] && dis[match[v]] != du) continue;
      if (vis[v] == vstamp) continue;
      vis[v] = vstamp;
      if (!match[v] || dfs(match[v])) {
        match[u] = v, match[v] = u;
        return true;
      }
    }
    dis[u] = -1;
    return false;
  }
  // match[u]=v and match[v]=u after success; clear g/match between cases
  int solve(int nl_, int nr_) {
    nl = nl_, nr = nr_;
    for (int i = 1; i <= nl + nr; ++i) match[i] = 0;
    int ans = 0;
    while (bfs()) {
      ++vstamp;  // timestamps replace per-phase memset(vis)
      for (int i = 1; i <= nl; ++i)
        if (!match[i] && dfs(i)) ++ans;
    }
    return ans;
  }
};

/*
 * ============================================================
 * Name: Hopcroft-Karp (maximum bipartite matching)
 * Complexity: O(E sqrt(V))
 * Usage: maximum bipartite matching, wrapped as hopcroft_karp<N>:
 *        g[left vertex] holds right-side ids, solve(nl, nr) returns the
 *        matching size; the result lives in match[] (match[u]=v and
 *        match[v]=u); left vertices 1..nl, right vertices 1..nr or remapped
 * Principle: BFS layers free left vertices by augmenting-path length (dis);
 *        DFS only follows "next layer" edges (multi-path augmentation), so
 *        each phase finds all shortest augmenting paths; O(sqrt(V)) phases,
 *        O(E) each
 * Notes: right ids must not collide with left ids (the match array is
 *        shared; map right vertices to nl+1..nl+nr or use disjoint ranges);
 *        clear g/match between test cases
 * ============================================================
 * Example (uncomment to compile):
 * static hopcroft_karp<200009> hk;
 * signed main() {
 *   int nl, nr, m;
 *   cin >> nl >> nr >> m;
 *   for (int i = 1, u, v; i <= m; ++i) {
 *     cin >> u >> v;
 *     hk.g[u].push_back(v + nl);   // map right vertices to nl+1..nl+nr
 *   }
 *   cout << hk.solve(nl, nl + nr) << '\n';
 * }
 * ============================================================
 */
