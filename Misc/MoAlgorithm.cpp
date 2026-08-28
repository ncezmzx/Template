#include <bits/stdc++.h>
using namespace std;

// Mo's algorithm: offline range queries, amortized O((n+q) sqrt n) pointer moves
// plain version: distinct values in range; modify version: point updates + distinct values
constexpr int N = 5e5 + 9, MQ = 5e5 + 9;
int a[N], cnt_[N], cur, ans[MQ];

struct Qry {
  int l, r, t, id;
};
inline void mo_add(int i) { cur += cnt_[a[i]]++ == 0; }
inline void mo_del(int i) { cur -= --cnt_[a[i]] == 0; }

// plain Mo: a[1..n], queries [l, r] (1-indexed) -> number of distinct values
vector<int> mo_distinct(int n, const vector<int>& val, vector<array<int, 2>> qs) {
  int q = qs.size(), B = max(1, (int)(n / max(1.0, sqrt((double)q))));
  vector<Qry> Q(q);
  for (int i = 0; i < q; ++i) Q[i] = {qs[i][0], qs[i][1], 0, i};
  sort(Q.begin(), Q.end(), [&](const Qry& x, const Qry& y) {
    int bx = x.l / B, by = y.l / B;
    if (bx != by) return bx < by;
    return bx & 1 ? x.r > y.r : x.r < y.r;  // odd/even block optimization
  });
  for (int i = 1; i <= n; ++i) a[i] = val[i], cnt_[a[i]] = 0;
  cur = 0;
  int l = 1, r = 0;
  vector<int> res(q);
  for (auto& Q_ : Q) {
    while (l > Q_.l) mo_add(--l);
    while (r < Q_.r) mo_add(++r);
    while (l < Q_.l) mo_del(l++);
    while (r > Q_.r) mo_del(r--);
    res[Q_.id] = cur;
  }
  return res;
}

struct Upd {
  int pos, val;
};
// Mo with updates: point assignments (ups in time order) + distinct values in range
vector<int> mo_distinct_modify(int n, const vector<int>& val, const vector<array<int, 2>> qs, const vector<array<int, 2>> ups) {
  int q = qs.size(), t = ups.size();
  vector<array<int, 2>> up(ups);  // mutable copy (time-pointer rollback rewrites it)
  int B = max(1, (int)pow(n, 2.0 / 3));
  vector<Qry> Q(q);
  for (int i = 0; i < q; ++i) Q[i] = {qs[i][0], qs[i][1], t, i};  // t = updates before query (here: all applied)
  sort(Q.begin(), Q.end(), [&](const Qry& x, const Qry& y) {
    int bx = x.l / B, by = y.l / B;
    if (bx != by) return bx < by;
    int rx = x.r / B, ry = y.r / B;
    if (rx != ry) return rx < ry;
    return x.t < y.t;
  });
  for (int i = 1; i <= n; ++i) a[i] = val[i];
  for (int i = 0; i < N; ++i) cnt_[i] = 0;
  cur = 0;
  int l = 1, r = 0, now = 0;
  vector<int> res(q);
  for (auto& Q_ : Q) {
    while (now < Q_.t) {  // advance time pointer: apply update now
      int p = up[now][0];
      if (l <= p && p <= r) mo_del(p), swap(a[p], up[now][1]), mo_add(p);
      else swap(a[p], up[now][1]);
      ++now;
    }
    while (now > Q_.t) {
      --now;
      int p = up[now][0];
      if (l <= p && p <= r) mo_del(p), swap(a[p], up[now][1]), mo_add(p);
      else swap(a[p], up[now][1]);
    }
    while (l > Q_.l) mo_add(--l);
    while (r < Q_.r) mo_add(++r);
    while (l < Q_.l) mo_del(l++);
    while (r > Q_.r) mo_del(r--);
    res[Q_.id] = cur;
  }
  return res;
}

/*
 * ============================================================
 * Name: Mo's algorithm (plain + with updates)
 * Complexity: plain O((n+q) sqrt n); with updates O(n^{5/3}) (block size n^{2/3})
 * Usage: offline range statistics (forcing online without updates is
 *        expensive; ranges support O(1) add/remove): the example counts
 *        distinct values in range; adapt mo_add/mo_del for modes, value-range
 *        counting on [x,y], xor-sum kinds, etc.
 * Interface: mo_distinct(n, val[1..n], {{l, r}, ...});
 *        mo_distinct_modify(n, val, qs, ups) (ups = {pos, val}; assumes all
 *        updates happen before all queries — for interleaved timelines, set
 *        each query's t to the number of updates preceding it)
 * Principle: queries sorted by (l-block, r-block/odd-even, t); three
 *        pointers (l, r, time) move; with O(1) add/remove the total movement
 *        under optimal blocking reaches the amortized lower bound
 * Notes: add/del must be safe to call in any order (the example's cnt checks
 *        are); compress values exceeding N first; odd/even blocking saves
 *        half the r movement
 * ============================================================
 * Example (uncomment to compile):
 * signed main() {
 *   vector<int> val{0, 1, 2, 1, 3, 2};  // n = 5: {1,2,1,3,2}
 *   auto res = mo_distinct(5, val, {{1, 3}, {2, 5}, {4, 4}});
 *   for (int x : res) cout << x << ' ';  // 2 3 1（{1,2} {2,1,3} {3}）
 *   cout << '\n';
 *   auto res2 = mo_distinct_modify(5, val, {{1, 5}}, {{2, 5}});  // a[2] = 5 → {1,5,1,3,2}
 *   for (int x : res2) cout << x << ' ';  // 4
 *   cout << '\n';
 * }
 */
