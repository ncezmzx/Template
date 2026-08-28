#include <bits/stdc++.h>
using namespace std;
#define int long long

// SPFA shortest paths with SLF heuristic: negative edges, negative-cycle detection,
// difference constraints
template <size_t N, size_t M>
struct spfa {
  int n_, ecnt, hd[N], nxt[M], to[M], cnt_[N];
  long long wt[M], dis[N];
  bool inq[N];
  void init(int n) {
    n_ = n, ecnt = 1;
    fill(hd, hd + n + 1, 0);  // clear from 0 (diff_constraints uses super source 0)
  }
  void add(int u, int v, long long w) {  // directed edge
    ++ecnt, to[ecnt] = v, wt[ecnt] = w, nxt[ecnt] = hd[u], hd[u] = ecnt;
  }
  // shortest paths from s; false = negative cycle reachable from s (dis invalid)
  bool run(int s) {
    fill(dis + 1, dis + n_ + 1, LLONG_MAX);
    fill(cnt_ + 1, cnt_ + n_ + 1, 0);
    fill(inq + 1, inq + n_ + 1, false);
    deque<int> q;
    dis[s] = 0, inq[s] = true, q.push_back(s);
    while (!q.empty()) {
      int u = q.front();
      q.pop_front(), inq[u] = false;
      for (int e = hd[u]; e; e = nxt[e]) {
        int v = to[e];
        if (dis[u] + wt[e] < dis[v]) {
          dis[v] = dis[u] + wt[e];
          if (!inq[v]) {
            if (++cnt_[v] >= n_) return false;  // too many enqueues -> negative cycle
            inq[v] = true;
            if (!q.empty() && dis[v] < dis[q.front()]) q.push_front(v);  // SLF
            else q.push_back(v);
          }
        }
      }
    }
    return true;
  }
  // solve x_a - x_b <= c for all constraints {a, b, c}; false = unsolvable
  // edge b -> a (weight c); super source 0 reaches everything; x[i] = dis[i]
  bool diff_constraints(const vector<array<long long, 3>>& cs, int n, vector<long long>& x) {
    init(n + 1);
    for (const auto& t : cs) add(t[1], t[0], t[2]);  // x_a - x_b <= c -> b -> a
    for (int i = 1; i <= n; ++i) add(0, i, 0);
    if (!run(0)) return false;
    x.assign(n + 1, 0);
    for (int i = 1; i <= n; ++i) x[i] = dis[i];
    return true;
  }
};

/*
 * ============================================================
 * ============================================================
 * Name: SPFA shortest paths (negative-cycle detection + difference constraints)
 * Complexity: usually O(km) (small k), worst O(nm); difference constraints same
 * Usage: single-source shortest paths with negative edges (where Dijkstra
 *        fails), negative-cycle detection, feasible solutions of difference
 *        constraint systems, wrapped as spfa<N, M>: init(n) / add(u, v, w);
 *        run(s) returns false on a negative cycle, else dis[] holds the
 *        distances; diff_constraints(cs, n, x) solves constraints of the
 *        form x_a - x_b <= c (cs elements are {a, b, c})
 * Principle: queue-optimized Bellman-Ford — only relaxed vertices need
 *        re-relaxation; SLF (shorter to the front) cuts constants; a vertex
 *        enqueued >= n times proves an infinitely relaxable (negative)
 *        cycle; difference constraints view x_a <= x_b + c as triangle
 *        inequalities, a super source ensures reachability, unsolvable <=>
 *        negative cycle
 * Notes: negative-cycle checks need a source that reaches the whole graph
 *        (the super source 0 for difference constraints); for minimum
 *        solutions negate the constraints (x_b - x_a <= -c, longest paths,
 *        or flip all signs)
 * ============================================================
 * Example (uncomment to compile):

 * static spfa<5009, 20009> sp;
 * signed main() {
 *   sp.init(3);
 *   sp.add(1, 2, -2), sp.add(2, 3, -2), sp.add(3, 1, -2);  // negative cycle
 *   cout << sp.run(1) << '\n';                              // 0 (negative cycle)
 *   sp.init(3);
 *   sp.add(1, 2, 4), sp.add(1, 3, -1), sp.add(2, 3, 2);
 *   cout << sp.run(1) << ' ' << sp.dis[3] << '\n';          // 1 -1 (direct edge 1->3 costs -1)
 *   vector<array<long long, 3>> cs{{2, 1, 3}, {3, 2, -1}, {1, 3, 5}};
 *   // x2 - x1 <= 3, x3 - x2 <= -1, x1 - x3 <= 5
 *   vector<long long> x;
 *   bool ok = sp.diff_constraints(cs, 3, x);
 *   cout << ok << ' ' << x[2] - x[1] << ' ' << x[3] - x[2] << '\n';  // 1, differences satisfy the constraints
 * }
 */
