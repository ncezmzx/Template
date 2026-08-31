#include <bits/stdc++.h>
using namespace std;
#define int long long

// parallel binary search: offline static range k-th smallest
// binary-search the answer space; each level partitions queries with a BIT count of values <= mid
constexpr int N = 2e5 + 9;
int bit[N], n_;
inline void bit_add(int i, int v) {
  for (; i <= n_; i += i & -i) bit[i] += v;
}
inline int bit_sum(int i) {
  int s = 0;
  for (; i > 0; i -= i & -i) s += bit[i];
  return s;
}

struct Q {
  int l, r, k, id;
};
// a[1..n] (1-indexed); qs = {{l, r, k}, ...}; returns k-th smallest values in input order
vector<int> parallel_kth(int n, const vector<int> &a, const vector<array<int, 3>> &qs) {
  n_ = n;
  memset(bit, 0, (n + 1) * sizeof(int));
  vector<int> vals(a.begin() + 1, a.end());
  sort(vals.begin(), vals.end());
  vals.erase(unique(vals.begin(), vals.end()), vals.end());
  int V = vals.size();
  vector<vector<int>> pos(V + 1); // all positions of compressed value i
  for (int i = 1; i <= n; ++i) pos[(int)(lower_bound(vals.begin(), vals.end(), a[i]) - vals.begin()) + 1].push_back(i);
  vector<Q> cur;
  for (int i = 0; i < (int)qs.size(); ++i) cur.push_back({qs[i][0], qs[i][1], qs[i][2], i});
  vector<int> ans(qs.size());
  function<void(int, int, vector<Q> &)> solve = [&](int L, int R, vector<Q> &q) {
    if (q.empty()) return;
    if (L == R) {
      for (auto &x : q) ans[x.id] = vals[L - 1];
      return;
    }
    int mid = L + R >> 1;
    for (int i = L; i <= mid; ++i) // insert positions with values in [L, mid]
      for (int p : pos[i]) bit_add(p, 1);
    vector<Q> lq, rq;
    for (auto &x : q) {
      int c = bit_sum(x.r) - bit_sum(x.l - 1); // count of values <= mid in range
      if (c >= x.k) lq.push_back(x);
      else rq.push_back({x.l, x.r, x.k - c, x.id});
    }
    for (int i = L; i <= mid; ++i) // roll back the BIT for the next level
      for (int p : pos[i]) bit_add(p, -1);
    solve(L, mid, lq);
    solve(mid + 1, R, rq);
  };
  solve(1, V, cur);
  return ans;
}

/*
 * ============================================================
 * Name: parallel binary search
 * Complexity: O((n + q) log^2 n)
 * Usage: problems with many queries whose answers are monotone / bisectable and
 *        offline-able, so all answers are binary-searched together; the classic
 *        is the static range k-th smallest
 *        (replacing tree-of-trees / persistent segment trees), also k-th
 *        smallest with updates and 2D k-th smallest (treat updates as events
 *        too).
 *        parallel_kth(n, a[1..n], {{l, r, k}, ...}) returns the k-th smallest
 *        per query.
 * Interface: parallel_kth(n, a[1..n], {{l, r, k}, ...}) -> k-th smallest per query
 * Principle: recurse over the answer value domain [L, R]; each level inserts
 *            the positions with values <= mid into a BIT, and per query the
 *            count c of values <= mid inside [l, r] decides: c >= k goes left,
 *            otherwise k -= c goes right; roll back this level's BIT before
 *            recursing right
 * Notes: values must be compressible; k must be valid (1 <= k <= r-l+1); reset
 *        the BIT between test cases
 * Source: OI-Wiki "Parallel binary search" (https://oi-wiki.org/misc/parallel-binsearch/)
 * ============================================================
 */
