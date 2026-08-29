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
 * Usage: subtree queries on a static tree: sum of the most frequent color ids
 *        in each subtree (implemented here, CF 600E), distinct colors in a
 *        subtree, k-th smallest, depth-related statistics, and friends.
 *        Keep the heavy child's information and brute-force recompute the light
 *        children. Call pattern: dfs(1, 0); dfs2(1, 0, true); ans[x] is x's
 *        subtree answer.
 * Principle: the first dfs finds the heavy children; dfs2 processes all light
 *            children first (do not keep), then the heavy child (keep), then
 *            brute-force adds the light subtrees back, giving the complete
 *            statistic; add(x, ff, -1) clears a subtree
 * Notes: the statistics maintained inside add (mx / sum) are problem-specific;
 *        cnt[] is indexed by color id; mx / sum reset on clear (handled in the
 *        !keep branch)
 * ============================================================
 */
