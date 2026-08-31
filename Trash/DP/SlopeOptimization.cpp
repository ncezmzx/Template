#include <bits/stdc++.h>
using namespace std;
#define int long long
using d64 = long double;

// slope-optimized DP (convex hull trick, monotone queue) + WQS binary search:
// split a[1..n] into exactly k segments minimizing sum (segment sum + 1)^2
template <size_t N> struct slope_opt_wqs {
  constexpr static d64 eps = 1e-9;
  int n, k, a[N], s[N], f[N], g[N], q[N], hd, tl;
  d64 x(int i) { return s[i]; }
  d64 y(int i) { return f[i] + s[i] * s[i]; }
  d64 slope(int i, int j) { return (y(j) - y(i)) / (x(j) - x(i)); }
  // one Kruskal-style pass with penalty x per segment;
  // returns {cost with penalty, number of segments used}
  pair<int, int> calc(int x) {
    for (int i = 0; i <= n; ++i) f[i] = g[i] = 0;
    hd = tl = 1, q[1] = 0;
    for (int i = 1; i <= n; ++i) {
      while (hd < tl && slope(q[hd], q[hd + 1]) < eps + 2 * (s[i] + 1)) ++hd;
      int j = q[hd];
      g[i] = g[j] + 1, f[i] = f[j] + (s[i] - s[j] + 1) * (s[i] - s[j] + 1) - x;
      while (hd < tl && slope(q[tl - 1], q[tl]) + eps > slope(q[tl], i)) --tl;
      q[++tl] = i;
    }
    return {f[n], g[n]};
  }
  // WQS binary search over the penalty until g[n] == k, then restore
  int solve() {
    int l = -2e16, r = 2e16;
    while (l < r) {
      int m = (l + r) >> 1;
      if (calc(m).second >= k) r = m;
      else l = m + 1;
    }
    return calc(l).first + l * k;
  }
};

/*
 * ============================================================
 * Name: slope-optimized DP (convex hull trick CHT, monotone queue) + WQS binary search
 * Complexity: one calc(x) pass O(n) (each point enters/leaves the queue once),
 *             WQS binary search O(log V) passes, total O(n log V)
 * Usage: segmented DP of the form f[i] = min_j { f[j] + w(j,i) } with w
 *        satisfying the quadrangle inequality, wrapped as slope_opt_wqs<N>;
 *        WQS removes the "exactly k segments" constraint: subtract a penalty
 *        x per segment inside calc, binary-search x until the optimal segment
 *        count g[n] hits k, answer = calc(l).first + l * k.
 *        The example problem: split a[1..n] into exactly k segments
 *        minimizing sum (segment sum + 1)^2.
 * Source: all.cpp 28756-28807 (extracted verbatim into a struct; the
 *         commented-out cerr debugging kept out). Variants: two other
 *         slope-optimization forms follow below (all.cpp 26430-26480
 *         fractional calc; all.cpp 54539-54610 classic y/x slopes).
 * ============================================================
 * Example (uncomment to compile):
 * static slope_opt_wqs<100009> so;
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   cin >> so.n >> so.k;
 *   for (int i = 1; i <= so.n; ++i) cin >> so.a[i], so.s[i] = so.s[i - 1] + so.a[i];
 *   cout << so.solve();
 * }
 * ============================================================
  *

 * ── Variant 1 (all.cpp 26430-26480, fractional slope optimization) ─────────
 * Grouping/mean-type problems: calc(l, j) compares the "fraction value" of two
 * candidate endpoints directly, combined with cmax updates (the original used
 * cmax rather than direct assignment):
 *
 * int n, k, p, h[N], q[N], hd, tl, H, s[N];
 * pair<short, bool> g[N][N];
 * d64 f[N], F[N];
 * d64 x(int i) { return i - 1; }
 * d64 y(int i) { return F[i] - s[i]; }
 * d64 slope(int i, int j) {
 *   return (y(j) - y(i)) / (x(j) - x(i));
 * }
 * d64 calc(int l, int j) {
 *   return (F[l] - s[l] + s[j]) / (j - l + 1);
 * }
 * // main-loop skeleton:
 * //   q[hd = tl = 1] = 0;
 * //   for (int j = 1; j <= n; ++j) {
 * //     while (hd < tl && calc(q[hd], j) < calc(q[hd + 1], j)) ++hd;
 * //     g[i][j] = {j - 1, 0}, f[j] = f[j - 1];
 * //     if (cmax(f[j], calc(q[hd], j))) g[i][j] = {q[hd], 1};
 * //     while (hd < tl && slope(q[tl - 1], q[tl]) + eps <= slope(q[tl], j)) --tl;
 * //     q[++tl] = j;
 * //   }
 * // helper used by the original (all.cpp 26427):
 * //   template<class T> bool cmax(T& x, const T &y) { if (x <= y) return x = y, true; return false; }
 *
 * ── Variant 2 (all.cpp 54539-54610, classic y/x slope + monotone queue) ────
 * 2a. static hull over m slope queries (54539-54560, handles vertical lines
 *     where x is equal):
 *
 * double slope(int x, int y) {
 *   return a[y].x == a[x].x ? (a[y].y < a[x].y ? -inf : inf) : 1. * (a[y].y - a[x].y) / (a[y].x - a[x].x);
 * }
 * // lower hull: for (int i = 1; i <= n; ++i) {
 * //               while (l < r && slope(q[r - 1], q[r]) <= slope(q[r], i)) --r;
 * //               q[++r] = i;
 * //             }
 * // sort queries by slope, then: while (l < r && slope(q[l], q[l + 1]) >= b[i].k) ++l;
 * //                             ans[b[i].i] = -b[i].k * a[q[l]].x + a[q[l]].y;
 *
 * 2b. decision-monotone DP form (54571-54604, getx/gety wrap the coordinates,
 *     slopes monotone in b[i].w):
 *
 * int getx(int x) { return -b[x + 1].h; }
 * int gety(int x) { return dp[x]; }
 * double slope(int x, int y) {
 *   return 1. * (gety(y) - gety(x)) / (getx(y) - getx(x));
 * }
 * // main loop:
 * //   q[++r] = 0;
 * //   for (int i = 1; i <= m; ++i) {
 * //     while (l < r && slope(q[l], q[l + 1]) <= b[i].w) ++l;
 * //     dp[i] = b[i].w * b[q[l] + 1].h + dp[q[l]];
 * //     while (l < r && slope(q[r - 1], q[r]) >= slope(q[r], i)) --r;
 * //     q[++r] = i;
 * //   }
 */
