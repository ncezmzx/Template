#include <bits/stdc++.h>
using namespace std;

// rollback Mo (deletion-free Mo): max distance between equal values in range
// for statistics supporting add but not delete (mode / max distance / historic extrema)
constexpr int N = 2e5 + 9;
int a[N], first[N], last[N];        // first/last occurrence of each value in the window
int sv[N], sf[N], stk_top;          // rollback stack for left extensions (value / old first)
int ts[N], tmpfirst[N], timer;      // timestamps for in-block brute force
int cur;

inline void radd(int i) {           // add on the right (i increasing, permanent)
  int v = a[i];
  if (first[v] == 0) first[v] = last[v] = i;
  else last[v] = i, cur = max(cur, last[v] - first[v]);
}
inline void ladd(int i) {           // add on the left (i decreasing, temporary, rollback-able)
  int v = a[i];
  sv[++stk_top] = v, sf[stk_top] = first[v];
  if (first[v] == 0) first[v] = last[v] = i;
  else first[v] = i, cur = max(cur, last[v] - first[v]);
}
inline void lrollback() {           // undo left additions in reverse order
  for (; stk_top; --stk_top) first[sv[stk_top]] = sf[stk_top];
}

struct Qry {
  int l, r, id;
};
// val[1..n] (val[0] unused); qs = {{l, r}, ...} (1-indexed)
vector<int> rollback_mo(int n, const vector<int>& val, const vector<array<int, 2>>& qs) {
  // coordinate-compress to [1, V]
  vector<int> b(val.begin() + 1, val.end());
  sort(b.begin(), b.end()), b.erase(unique(b.begin(), b.end()), b.end());
  for (int i = 1; i <= n; ++i)
    a[i] = (int)(lower_bound(b.begin(), b.end(), val[i]) - b.begin()) + 1;
  int V = b.size(), q = qs.size();
  int B = max(1, (int)(n / max(1.0, sqrt((double)q))));  // block length n/sqrt(q)
  vector<Qry> Q(q);
  for (int i = 0; i < q; ++i) Q[i] = {qs[i][0], qs[i][1], i};
  sort(Q.begin(), Q.end(), [&](const Qry& x, const Qry& y) {
    int bx = x.l / B, by = y.l / B;
    if (bx != by) return bx < by;
    return x.r < y.r;  // inside a block r only grows (odd/even trick not applicable)
  });
  vector<int> ans(q);
  for (int i = 0; i < q;) {
    int b = Q[i].l / B;
    int R = min(n, (b + 1) * B);  // right end of the block
    for (int v = 1; v <= V; ++v) first[v] = last[v] = 0;
    cur = 0;
    int r = R;
    for (; i < q && Q[i].l / B == b; ++i) {
      if (Q[i].r <= R) {  // query fully inside the block: brute force
        int best = 0, t = ++timer;
        for (int p = Q[i].l; p <= Q[i].r; ++p) {
          int v = a[p];
          if (ts[v] != t) ts[v] = t, tmpfirst[v] = p;
          else best = max(best, p - tmpfirst[v]);
        }
        ans[Q[i].id] = best;
      } else {
        while (r < Q[i].r) radd(++r);
        int saved = cur;
        stk_top = 0;
        int l = R + 1;
        while (l > Q[i].l) ladd(--l);
        ans[Q[i].id] = cur;
        lrollback();
        cur = saved;
      }
    }
  }
  return ans;
}

/*
 * ============================================================
 * Name: rollback Mo (deletion-free Mo)
 * Complexity: O(n sqrt q + q sqrt n) (block size n / sqrt q)
 * Usage: offline range queries for statistics that are O(1) to add but hard
 *        or impossible to remove in O(1): the example is the maximum
 *        distance between equal values in range (Luogu P5906); the same
 *        scheme does range-mode counts, etc.
 * Interface: rollback_mo(n, val[1..n], {{l, r}, ...}) -> per-query farthest equal-value distance
 * Principle: group by l's block; inside a block the right pointer only moves
 *        right (additions kept permanently); per query the left pointer
 *        extends temporarily leftward from the block's right end and rolls
 *        back in reverse after answering — no deletion ever needed
 * Notes: r increases monotonically within a block, so odd/even ordering does
 *        not apply; values are compressed automatically; empty windows return
 *        0; left additions only touch first[] (reverse-order rollback keeps
 *        it correct)
 * Source: OI-Wiki "Rollback Mo" (https://oi-wiki.org/misc/rollback-mo/)
 * ============================================================
 * Example (uncomment to compile):
 * signed main() {
 *   vector<int> val{0, 1, 2, 1, 3, 2};  // n = 5: {1,2,1,3,2}
 *   auto r = rollback_mo(5, val, {{1, 3}, {2, 5}, {1, 5}});
 *   for (int x : r) cout << x << ' ';  // 2 (the 1s, distance 2) 3 (the 2s, distance 3) 3
 *   cout << '\n';
 * }
 * ============================================================
 */
