#include <bits/stdc++.h>
using namespace std;
#define int long long


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

vector<int> parallel_kth(int n, const vector<int> &a, const vector<array<int, 3>> &qs) {
  n_ = n;
  memset(bit, 0, (n + 1) * sizeof(int));
  vector<int> vals(a.begin() + 1, a.end());
  sort(vals.begin(), vals.end());
  vals.erase(unique(vals.begin(), vals.end()), vals.end());
  int V = vals.size();
  vector<vector<int>> pos(V + 1);
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
    for (int i = L; i <= mid; ++i)
      for (int p : pos[i]) bit_add(p, 1);
    vector<Q> lq, rq;
    for (auto &x : q) {
      int c = bit_sum(x.r) - bit_sum(x.l - 1);
      if (c >= x.k) lq.push_back(x);
      else rq.push_back({x.l, x.r, x.k - c, x.id});
    }
    for (int i = L; i <= mid; ++i)
      for (int p : pos[i]) bit_add(p, -1);
    solve(L, mid, lq);
    solve(mid + 1, R, rq);
  };
  solve(1, V, cur);
  return ans;
}

