#include <bits/stdc++.h>
using namespace std;
#define int long long

// Hopcroft-Karp maximum bipartite matching, O(E sqrt V)
template <size_t N> // n = nl + nr
struct hopcroft_karp {
  int nl, nr, vstamp = 0;
  int dis[N], match[N], vis[N], q[N];
  vector<int> g[N];
  bool bfs() {
    bool f = false;
    int qh = 0, qt = 0;
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
    int du = dis[u] + 1;
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
  int solve(int nl_, int nr_) {
    nl = nl_, nr = nr_;
    for (int i = 1; i <= nl + nr; ++i) match[i] = 0;
    int ans = 0;
    while (bfs()) {
      ++vstamp;
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
 * Usage: maximum bipartite matching, `hopcroft_karp<N>`: g[left vertex] holds
 *        right-side ids; solve(nl, nr) returns the matching size;
 *        the result lives in match[] (match[u] = v and match[v] = u); left
 *        vertices 1..nl.
 * Principle: BFS layers free left vertices by augmenting-path length; DFS only
 *            follows next-layer edges (multi-path augmentation), so each phase
 *            finds all shortest augmenting paths: O(sqrt(V)) phases of O(E)
 *            each
 * Notes: right ids must not collide with left ids (the match array is shared,
 *        so map right vertices to nl+1..nl+nr or use disjoint ranges); clear g
 *        / match between test cases
 * ============================================================
 */
