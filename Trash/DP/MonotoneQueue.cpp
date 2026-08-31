#include <bits/stdc++.h>
using namespace std;

// monotone queue: sliding window maximum over a sequence, O(n)
// returns res[j] = max(a[j..j+y-1]) for j = 0..(int)a.size()-y
vector<int> window_max(const vector<int> &a, int y) {
  int m = (int)a.size();
  vector<int> res(max(0, m - y + 1));
  deque<int> q; // indices; front = current window maximum
  for (int j = 0; j < m; ++j) {
    while (!q.empty() && q.front() < j - y + 1) q.pop_front(); // expired
    while (!q.empty() && a[q.back()] < a[j]) q.pop_back();     // keep decreasing
    q.push_back(j);
    if (j >= y - 1) res[j - y + 1] = a[q.front()];
  }
  return res;
}

/*
 * ============================================================
 * Name: monotone queue (sliding window extrema, deque implementation)
 * Complexity: O(n) (each index enters/leaves the queue at most once)
 * Usage: min/max over a sliding window of length y; also the workhorse for
 *        monotone-queue-optimized DP (decision points slide with the window);
 *        wrapped as window_max(a, y); for minima flip the comparison
 * Invariant: the front always holds the current window's extremum index;
 *        before pushing, pop expired elements from the front and "dominated"
 *        (no better than the new element) elements from the back, keeping the
 *        queue monotone
 * Source: all.cpp 48897-48903 (originally the per-row window loop of a 2D
 *         sliding window problem, extracted here as a function); the
 *         array-based monotone queue (q[hd..tl]) follows below, from
 *         all.cpp 28770-28776 (slope-optimization source block)
 * ============================================================
 * Example (uncomment to compile): per-row window maxima of an n x m matrix
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   int n, m;
 *   cin >> n >> m;
 *   vector<vector<int>> a(n, vector<int>(m));
 *   for (int i = 0; i < n; ++i)
 *     for (int j = 0; j < m; ++j) cin >> a[i][j];
 *   int y;
 *   cin >> y;
 *   for (int i = 0; i < n; ++i) {
 *     auto res = window_max(a[i], y);
 *     for (int j = 0; j < (int)res.size(); ++j) cout << res[j] << " \n"[j + 1 == (int)res.size()];
 *   }
 *   return 0;
 * }
 * ============================================================
  *

 * ── Array-based monotone queue (q[hd..tl], from all.cpp 28770-28776, verbatim
 *    inside the slope-optimization block) ────────────────────────────────────
 * int q[N], hd, tl;
 * q[hd = tl = 1] = 0;                    // init: front/back at 1, decision point 0 in
 * for (int i = 1; i <= n; ++i) {
 *   while (hd < tl && <front-not-optimal condition>) ++hd;   // pop front (no longer optimal)
 *   int j = q[hd];                          // front = optimal decision point
 *   ... transition using j ...
 *   while (hd < tl && <back-breaks convexity/monotonicity>) --tl;  // pop back, keep monotone
 *   q[++tl] = i;                            // new decision point
 * }
 * For minima, the queue's indexed values are non-decreasing (front minimal);
 * flip the comparison signs accordingly.
 *
 * ── Other related forms ─────────────────────────────────────────────────────
 * 1) all.cpp 4936-4946: monotone-queue push wrapper (sliding window max,
 *    interactive-problem fragment):
 *    deque<int> q;
 *    void push(int x) { while (!q.empty() && a[q.back()] < a[x]) q.pop_back(); q.push_back(x); }
 *    // expiry pop: while (!q.empty() && q.front() < lft) q.pop_front();
 * 2) all.cpp 54351-54366: monotone-queue-optimized DP (minimal "penalty"
 *    count jumping at most k steps):
 *    q.clear(); q.push_back(1);
 *    for (int i = 2; i <= n; ++i) {
 *      while (!q.empty() && i - q.front() > k) q.pop_front();
 *      dp[i] = dp[q.front()] + (d[i] >= d[q.front()]);
 *      while (!q.empty() && ((dp[q.back()] == dp[i] && d[q.back()] < d[i]) || dp[q.back()] > dp[i])) q.pop_back();
 *      q.push_back(i);
 *    }
 */
