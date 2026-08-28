#include <bits/stdc++.h>
using namespace std;
#define int long long

// virtual tree: keep only key vertices + pairwise LCAs between queries
template <size_t N>
struct virtual_tree {
  int n, idx;
  int dep[N], fa[N][20], dfn[N];
  vector<int> es[N], se[N];  // es = original tree; se = virtual tree
  void dfs(int x, int ff) {  // preprocess dfn/dep/binary lifting
    fa[x][0] = ff, dep[x] = dep[ff] + 1, dfn[x] = ++idx;
    for (int i = 1; i < 20; ++i) fa[x][i] = fa[fa[x][i - 1]][i - 1];
    for (int y : es[x])
      if (y ^ ff) dfs(y, x);
  }
  int lca(int x, int y) {  // binary lifting
    for (int i = 19; ~i; --i) {
      if (dep[fa[x][i]] >= dep[y]) x = fa[x][i];
      if (dep[fa[y][i]] >= dep[x]) y = fa[y][i];
    }
    if (x == y) return x;
    for (int i = 19; ~i; --i)
      if (fa[x][i] != fa[y][i]) x = fa[x][i], y = fa[y][i];
    return fa[x][0];
  }
  // builds the virtual tree for key set v in se[]; returns all its vertices
  // (keys + LCAs + root 1) so callers can clear them afterwards
  vector<int> build(vector<int> v) {
    sort(v.begin(), v.end(), [&](int x, int y) -> bool { return dfn[x] < dfn[y]; });
    int m = (int)v.size();  // snapshot: the LCA loop must not chase the growing vector
    for (int i = 0; i + 1 < m; ++i) v.push_back(lca(v[i], v[i + 1]));  // adjacent LCAs
    v.push_back(1);                                                                // root
    sort(v.begin(), v.end(), [&](int x, int y) -> bool { return dfn[x] < dfn[y]; });
    v.erase(unique(v.begin(), v.end()), v.end());
    for (int i = 0; i + 1 < (int)v.size(); ++i) se[lca(v[i], v[i + 1])].push_back(v[i + 1]);  // virtual edges
    return v;
  }
};

/*
 * ============================================================
 * Name: virtual tree
 * Complexity: O(k log k) per query of k key vertices (sort + adjacent LCAs)
 * Usage: wrapped as virtual_tree<N>: after dfs(1, 0), each query calls
 *        build(keys) keeping only key vertices and their pairwise LCAs in the
 *        virtual adjacency se[]; run tree DP on se[] instead of the whole
 *        tree; clear with se[i].clear() for every returned vertex afterwards
 * Build rule (original): sort by dfn; add LCAs of adjacent vertices; add root
 *        1; deduplicate; then se[lca(v[i], v[i+1])].push_back(v[i+1]), i.e.
 *        each vertex's virtual parent is its LCA with the predecessor
 * Notes: the original always adds root 1; adapt if the root differs
 * Source: all.cpp lines 36532-36570 (dfs/lca), 36618-36634 (virtual tree
 *         build, kept verbatim); wrapped into a struct
 * ============================================================
 * Example (uncomment to compile):
 * static virtual_tree<100009> vt;
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   int n;
 *   cin >> n;
 *   for (int i = 1, x, y; i < n; ++i) {
 *     cin >> x >> y;
 *     vt.es[x].push_back(y), vt.es[y].push_back(x);
 *   }
 *   vt.dfs(1, 0);
 *   int q;
 *   cin >> q;
 *   while (q--) {
 *     int k;
 *     cin >> k;
 *     vector<int> v(k);
 *     for (int i = 0; i < k; ++i) cin >> v[i];
 *     v = vt.build(v);
 *     // ... tree DP over the virtual adjacency vt.se[] ...
 *     for (int i : v) vt.se[i].clear();   // clean up before the next query
 *   }
 * }
 * ============================================================
 */
