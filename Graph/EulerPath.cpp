#include <bits/stdc++.h>
using namespace std;

// Euler path / circuit (iterative Hierholzer)
// undirected: paired edge ids (i and i^1) with used marks; directed: edge deletion via head pointers
constexpr int N = 2e5 + 9, M = 4e5 + 9;

struct euler_undirected {
  int n, ecnt = 1, hd[N], nxt[M], to[M], deg[N];
  bool used[M];
  void init(int n_) {
    n = n_, ecnt = 1;
    fill(hd + 1, hd + n + 1, 0), fill(deg + 1, deg + n + 1, 0);
    fill(used, used + M, false);  // reset edge marks (object reuse)
  }
  void add(int u, int v) {  // undirected edge (multi-edges and loops ok)
    ++ecnt, to[ecnt] = v, nxt[ecnt] = hd[u], hd[u] = ecnt, ++deg[u];
    ++ecnt, to[ecnt] = u, nxt[ecnt] = hd[v], hd[v] = ecnt, ++deg[v];
  }
  int odd_cnt() {  // number of odd-degree vertices (0: circuit; 2: path; else none)
    int c = 0;
    for (int i = 1; i <= n; ++i) c += deg[i] & 1;
    return c;
  }
  // vertex sequence starting at s that uses every edge (empty if impossible);
  // requires odd_cnt() == 0 (any non-isolated s) or 2 (s odd-degree), and connectivity
  vector<int> path(int s) {
    vector<int> stk, res;
    stk.push_back(s);
    while (!stk.empty()) {
      int v = stk.back();
      int& e = hd[v];
      while (e && used[e]) e = nxt[e];
      if (e) {
        used[e] = used[e ^ 1] = true;
        stk.push_back(to[e]);
        e = nxt[e];
      } else
        res.push_back(v), stk.pop_back();
    }
    reverse(res.begin(), res.end());
    return res;
  }
};

struct euler_directed {
  int n, ecnt = 1, hd[N], nxt[M], to[M], in_[N], out_[N];
  void init(int n_) {
    n = n_, ecnt = 1;
    fill(hd + 1, hd + n + 1, 0), fill(in_ + 1, in_ + n + 1, 0), fill(out_ + 1, out_ + n + 1, 0);
  }
  void add(int u, int v) {
    ++ecnt, to[ecnt] = v, nxt[ecnt] = hd[u], hd[u] = ecnt, ++out_[u], ++in_[v];
  }
  bool has_path(int& s) {  // checks in/out degree conditions; sets s to a valid start
    int a = 0, b = 0;
    s = 0;
    for (int i = 1; i <= n; ++i) {
      int d = out_[i] - in_[i];
      if (d > 1 || d < -1) return false;
      if (d == 1) ++a, s = i;
      if (d == -1) ++b;
      if (!s && (out_[i] || in_[i])) s = i;
    }
    if (a == 0 && b == 0) return true;                     // circuit (s is non-isolated)
    return a == 1 && b == 1;                               // path
  }
  // vertex sequence using every edge (empty if none; vertices with edges must be reachable from s)
  vector<int> path(int s) {
    vector<int> stk, res;
    stk.push_back(s);
    while (!stk.empty()) {
      int v = stk.back();
      if (hd[v]) {
        int e = hd[v];
        hd[v] = nxt[e];  // delete edge (directed needs no used marks)
        stk.push_back(to[e]);
      } else
        res.push_back(v), stk.pop_back();
    }
    reverse(res.begin(), res.end());
    return res;
  }
};

/*
 * ============================================================
 * ============================================================
 * Name: Euler path / circuit (iterative Hierholzer)
 * Complexity: O(n + m) (each edge visited once)
 * Usage: Euler trails/circuits of undirected and directed graphs (every edge
 *        exactly once), wrapped as euler_undirected / euler_directed:
 *        undirected existence: connected (ignoring isolated vertices) +
 *        0 odd-degree vertices (circuit) or 2 (path, starting at an odd one);
 *        directed existence: connected + all in/out differences 0 (circuit)
 *        or exactly one +1/-1 pair (path, starting at out-in = 1)
 * Interface: init(n) / add(u, v); odd_cnt() (undirected degree check);
 *        has_path(s) (directed degree check, sets start s);
 *        path(s) returns the vertex sequence (length m+1; empty on failure)
 * Principle: Hierholzer — walk unused edges from the start, dead-end
 *        vertices are pushed onto the answer stack; the reversed answer is
 *        the Euler order (iterative: no stack overflow)
 * Notes: path does NOT check connectivity (degree-valid but disconnected
 *        graphs yield a "partial path" shorter than m+1 — detectable);
 *        loops/multi-edges supported; objects hold MB-sized arrays, declare
 *        them global or static (never on the stack)
 * ============================================================
 * Example (uncomment to compile; static objects avoid stack overflow):

 * signed main() {
 *   static euler_undirected g;
 *   g.init(4);
 *   g.add(1, 2), g.add(2, 3), g.add(3, 4), g.add(4, 1), g.add(1, 3);
 *   cout << g.odd_cnt() << '\n';              // 2 (1 and 3 odd-degree -> path)
 *   auto p = g.path(1);                       // e.g. 1 2 3 4 1 3 (order not unique)
 *   for (int v : p) cout << v << ' ';         // length = 6 = m + 1
 *   cout << '\n';
 *   static euler_directed h;
 *   h.init(3);
 *   h.add(1, 2), h.add(2, 3), h.add(3, 1);
 *   int s;
 *   cout << h.has_path(s) << ' ' << s << '\n';  // 1 1 (circuit)
 * }
 */
