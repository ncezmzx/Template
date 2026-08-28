#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int N = 1e5 + 9;
int n, col[N], cnt[N], sz[N], son[N], big[N], mx, sum, ans[N];
vector<int> es[N];

void dfs(int x, int ff) {
  sz[x] = 1;
  for (int y : es[x])
    if (y != ff) {
      dfs(y, x), sz[x] += sz[y];
      if (sz[y] > sz[son[x]]) son[x] = y;
    }
}

void add(int x, int ff, int d) {
  int c = col[x];
  cnt[c] += d;
  if (d > 0) {
    if (cnt[c] > mx) mx = cnt[c], sum = c;
    else if (cnt[c] == mx) sum += c;
  }
  for (int y : es[x])
    if (y != ff && !big[y]) add(y, x, d);
}

void dfs2(int x, int ff, bool keep) {
  for (int y : es[x])
    if (y != ff && y != son[x]) dfs2(y, x, false);
  if (son[x]) dfs2(son[x], x, true), big[son[x]] = 1;
  add(x, ff, 1);
  ans[x] = sum;
  if (son[x]) big[son[x]] = 0;
  if (!keep) add(x, ff, -1), mx = 0, sum = 0;
}

/*
 * ============================================================
 * Name: DSU on tree (small-to-large subtree statistics)
 * Complexity: O(n log n)
 * Usage: subtree queries on a static tree, e.g.:
 *        1) sum of the most frequent color ids in each subtree (implemented
 *           here; CF 600E original problem);
 *        2) distinct colors in a subtree / k-th smallest / depth-related
 *           statistics, etc.
 *        Core idea: keep the heavy child's information, brute-force recompute
 *        the light children — O(n log n) total
 * Principle: first dfs finds heavy children; dfs2 processes all light
 *        children first (do not keep), then the heavy child (keep), then
 *        brute-force adds the light subtrees back, giving the complete
 *        statistic for the current subtree; add(x, ff, -1) clears a subtree
 * Notes: the statistics inside add (mx/sum) are problem-specific; cnt[] is
 *        indexed by color id; mx/sum reset on clear (handled in the !keep branch)
 * Usage pattern: dfs(1, 0); dfs2(1, 0, true); ans[x] is x's subtree answer
 * ============================================================
 * Example (uncomment to compile):
 * signed main() {
 *   cin >> n;
 *   for (int i = 1; i <= n; ++i) cin >> col[i];
 *   for (int i = 1, u, v; i < n; ++i) {
 *     cin >> u >> v;
 *     es[u].push_back(v), es[v].push_back(u);
 *   }
 *   dfs(1, 0), dfs2(1, 0, true);
 *   for (int i = 1; i <= n; ++i) cout << ans[i] << " \n"[i == n];
 * }
 * ============================================================
 */
