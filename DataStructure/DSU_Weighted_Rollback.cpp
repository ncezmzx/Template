#include <bits/stdc++.h>
using namespace std;

// weighted DSU: d[x] = accumulated edge weight from x to its root (additive group, e.g. mod addition / xor)
constexpr int N = 2e5 + 9;
struct wdsu {
  int p[N];
  long long d[N];
  void init(int n) {
    for (int i = 0; i <= n; ++i) p[i] = i, d[i] = 0;
  }
  int find(int x) {  // path compression accumulating weights
    if (p[x] == x) return x;
    int r = find(p[x]);
    d[x] += d[p[x]], p[x] = r;
    return r;
  }
  // merge constraint y = x + w (x --w--> y); returns compatibility with existing constraints
  bool merge(int x, int y, long long w) {
    int rx = find(x), ry = find(y);
    if (rx == ry) return d[y] - d[x] == w;
    p[rx] = ry, d[rx] = d[y] - d[x] - w;  // val[rx]-val[ry], derived from val[y]-val[x]=w
    return true;
  }
  long long rel(int x, int y) { return d[y] - d[x]; }  // val[y] - val[x] (must share a root: find first)
};

// rollback DSU: union by rank without path compression; rollback to a historical size
struct rdsu {
  int p[N], rk[N];
  vector<pair<int, int>> hist;  // (attached root, did the rank increase)
  void init(int n) {
    for (int i = 0; i <= n; ++i) p[i] = i, rk[i] = 0;
    hist.clear();
  }
  int find(int x) {
    while (p[x] != x) x = p[x];
    return x;
  }
  bool merge(int a, int b) {  // true if merged (already-connected pushes a no-op record)
    int ra = find(a), rb = find(b);
    if (ra == rb) return hist.push_back({-1, 0}), false;
    if (rk[ra] < rk[rb]) swap(ra, rb);
    p[rb] = ra, hist.push_back({rb, rk[ra] == rk[rb]});
    rk[ra] += (rk[ra] == rk[rb]);
    return true;
  }
  void rollback(size_t target) {  // undo until hist.size() == target
    while (hist.size() > target) {
      auto pr = hist.back();
      hist.pop_back();
      if (pr.first < 0) continue;
      if (pr.second) rk[p[pr.first]]--;  // restore rank
      p[pr.first] = pr.first;
    }
  }
};

/*
 * ============================================================
 * Name: weighted DSU / rollback DSU
 * Complexity: wdsu.merge/find amortized O(alpha); rdsu.find O(log n) (no path
 *             compression), merge/rollback O(1)
 * Usage: wdsu: merges carrying relative relations (food chains / parity /
 *        difference-constraint style y = x + w; weights from any additive
 *        group: modular addition, xor, ...);
 *        rdsu: connectivity with rollback for offline edge deletion,
 *        segment-tree divide & conquer, parallel binary search
 * Interface: wdsu: merge(x, y, w) (constraint y = x + w; returns
 *        compatibility), rel(x, y) (val[y] - val[x] once same root);
 *        rdsu: merge(a, b), rollback(snapshot of hist.size())
 * Principle: wdsu path compression accumulates weights up to the root; on
 *        merge, the new root link weight follows d[rx] = w + d[y] - d[x];
 *        rdsu unions by rank (height O(log n)) and undoes via the stack
 * Notes: wdsu's weight group must support + and - (for xor groups replace +
 *        and - with ^); rdsu must not path-compress (it would break rollback)
 * ============================================================
 * Example (uncomment to compile):
 * signed main() {
 *   wdsu w;
 *   w.init(5);
 *   w.merge(1, 2, 5), w.merge(2, 3, -2);   // val2 = val1+5, val3 = val2-2
 *   w.find(1), w.find(3);                  // find first so paths compress (d is then the full distance to the root)
 *   cout << w.rel(1, 3) << '\n';           // 3（val3 - val1 = 5-2）
 *   cout << w.merge(1, 3, 3) << w.merge(1, 3, 4) << '\n';  // 1 0 (compatible / contradictory)
 *   rdsu r;
 *   r.init(5);
 *   size_t snap = r.hist.size();
 *   r.merge(1, 2), r.merge(2, 3);
 *   cout << (r.find(1) == r.find(3)) << '\n';  // 1
 *   r.rollback(snap);
 *   cout << (r.find(1) == r.find(3)) << '\n';  // 0
 * }
 */
