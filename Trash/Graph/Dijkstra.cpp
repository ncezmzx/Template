#include <bits/stdc++.h>
using namespace std;
#define int long long

// heap-optimized Dijkstra (single source, non-negative weights)
template <size_t N>
struct dijkstra {
  static constexpr int INF = 0x3f3f3f3f3f3f3f3f;
  int dst[N];
  vector<pair<int, int>> es[N];  // directed edges (add both directions for undirected)
  void run(int s) {
    memset(dst, 0x3f, sizeof dst);          // 0x3f... is a huge long long
    priority_queue<pair<int, int>> q;       // max-heap of {-dist, vertex} = min-heap
    q.push({dst[s] = 0, s});
    while (!q.empty()) {
      auto [d, x] = q.top();
      d = -d, q.pop();
      if (d != dst[x]) continue;            // stale entry
      for (auto [y, z] : es[x])
        if (dst[y] > dst[x] + z) {          // relax
          dst[y] = dst[x] + z;
          q.emplace(-dst[y], y);
        }
    }
  }
};

/*
 * ============================================================
 * Name: heap-optimized Dijkstra (single-source shortest paths, non-negative weights)
 * Complexity: O((n + m) log n)
 * Usage: wrapped as dijkstra<N>: es[x].emplace_back(y, z) adds a directed edge
 *        (two for undirected), run(s), then dst[i] is the distance s -> i;
 *        multi-source works by seeding several sources into the queue
 * Implementation: a max-heap of {-dist, vertex} simulates a min-heap;
 *        stale pops are skipped by d != dst[x]
 * Source: all.cpp lines 52116-52157 (core loop kept verbatim, wrapped)
 * ============================================================
 * Example (uncomment to compile):
 * static dijkstra<100009> dij;
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   int n, m;
 *   cin >> n >> m;
 *   for (int i = 1, x, y, z; i <= m; ++i) {
 *     cin >> x >> y >> z;
 *     dij.es[x].emplace_back(y, z), dij.es[y].emplace_back(x, z);   // undirected example
 *   }
 *   dij.run(1);
 *   for (int i = 1; i <= n; ++i)
 *     cout << (dij.dst[i] >= dij.INF ? -1 : dij.dst[i]) << " \n"[i == n];  // -1 if unreachable
 * }
 * ============================================================
 */
