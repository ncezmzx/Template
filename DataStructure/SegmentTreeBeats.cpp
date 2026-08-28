#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int N = 5e5 + 9;
constexpr int INF = 4e18;
int n, m, a[N];

struct segbeats {
  struct node {
    int mx, mx2, mn, mn2, cmx, cmn, sum, tag;
  } t[N << 2];

  void up(int p) {
    t[p].sum = t[p << 1].sum + t[p << 1 | 1].sum;
    if (t[p << 1].mx > t[p << 1 | 1].mx) {
      t[p].mx = t[p << 1].mx, t[p].cmx = t[p << 1].cmx;
      t[p].mx2 = max(t[p << 1].mx2, t[p << 1 | 1].mx);
    } else if (t[p << 1].mx < t[p << 1 | 1].mx) {
      t[p].mx = t[p << 1 | 1].mx, t[p].cmx = t[p << 1 | 1].cmx;
      t[p].mx2 = max(t[p << 1].mx, t[p << 1 | 1].mx2);
    } else {
      t[p].mx = t[p << 1].mx, t[p].cmx = t[p << 1].cmx + t[p << 1 | 1].cmx;
      t[p].mx2 = max(t[p << 1].mx2, t[p << 1 | 1].mx2);
    }
    if (t[p << 1].mn < t[p << 1 | 1].mn) {
      t[p].mn = t[p << 1].mn, t[p].cmn = t[p << 1].cmn;
      t[p].mn2 = min(t[p << 1].mn2, t[p << 1 | 1].mn);
    } else if (t[p << 1].mn > t[p << 1 | 1].mn) {
      t[p].mn = t[p << 1 | 1].mn, t[p].cmn = t[p << 1 | 1].cmn;
      t[p].mn2 = min(t[p << 1].mn, t[p << 1 | 1].mn2);
    } else {
      t[p].mn = t[p << 1].mn, t[p].cmn = t[p << 1].cmn + t[p << 1 | 1].cmn;
      t[p].mn2 = min(t[p << 1].mn2, t[p << 1 | 1].mn2);
    }
  }

  void push_add(int p, int l, int r, int v) {
    t[p].sum += (r - l + 1) * v;
    t[p].mx += v, t[p].mn += v, t[p].tag += v;
    if (t[p].mx2 != -INF) t[p].mx2 += v;
    if (t[p].mn2 != INF) t[p].mn2 += v;
  }

  void push_min(int p, int v) {
    if (t[p].mx <= v) return;
    t[p].sum += (v - t[p].mx) * t[p].cmx;
    if (t[p].mn == t[p].mx) t[p].mn = v;
    if (t[p].mn2 == t[p].mx) t[p].mn2 = v;
    t[p].mx = v;
  }

  void push_max(int p, int v) {
    if (t[p].mn >= v) return;
    t[p].sum += (v - t[p].mn) * t[p].cmn;
    if (t[p].mx == t[p].mn) t[p].mx = v;
    if (t[p].mx2 == t[p].mn) t[p].mx2 = v;
    t[p].mn = v;
  }

  void pushdown(int p, int l, int r) {
    if (l == r) return;
    int m = (l + r) >> 1;
    if (t[p].tag) {
      push_add(p << 1, l, m, t[p].tag), push_add(p << 1 | 1, m + 1, r, t[p].tag);
      t[p].tag = 0;
    }
    push_max(p << 1, t[p].mn), push_min(p << 1, t[p].mx);
    push_max(p << 1 | 1, t[p].mn), push_min(p << 1 | 1, t[p].mx);
  }

  void build(int p, int l, int r) {
    t[p].tag = 0;
    if (l == r) {
      t[p].mx = t[p].mn = t[p].sum = a[l];
      t[p].mx2 = -INF, t[p].mn2 = INF;
      t[p].cmx = t[p].cmn = 1;
      return;
    }
    int m = (l + r) >> 1;
    build(p << 1, l, m), build(p << 1 | 1, m + 1, r);
    up(p);
  }

  void add(int p, int l, int r, int x, int y, int v) {
    if (x <= l && r <= y) return push_add(p, l, r, v), void();
    pushdown(p, l, r);
    int m = (l + r) >> 1;
    if (m >= x) add(p << 1, l, m, x, y, v);
    if (m < y) add(p << 1 | 1, m + 1, r, x, y, v);
    up(p);
  }

  void chmin(int p, int l, int r, int x, int y, int v) {
    if (t[p].mx <= v) return;
    if (x <= l && r <= y && t[p].mx2 < v) return push_min(p, v), void();
    pushdown(p, l, r);
    int m = (l + r) >> 1;
    if (m >= x) chmin(p << 1, l, m, x, y, v);
    if (m < y) chmin(p << 1 | 1, m + 1, r, x, y, v);
    up(p);
  }

  void chmax(int p, int l, int r, int x, int y, int v) {
    if (t[p].mn >= v) return;
    if (x <= l && r <= y && t[p].mn2 > v) return push_max(p, v), void();
    pushdown(p, l, r);
    int m = (l + r) >> 1;
    if (m >= x) chmax(p << 1, l, m, x, y, v);
    if (m < y) chmax(p << 1 | 1, m + 1, r, x, y, v);
    up(p);
  }

  int qsum(int p, int l, int r, int x, int y) {
    if (x <= l && r <= y) return t[p].sum;
    pushdown(p, l, r);
    int m = (l + r) >> 1, res = 0;
    if (m >= x) res += qsum(p << 1, l, m, x, y);
    if (m < y) res += qsum(p << 1 | 1, m + 1, r, x, y);
    return res;
  }

  int qmax(int p, int l, int r, int x, int y) {
    if (x <= l && r <= y) return t[p].mx;
    pushdown(p, l, r);
    int m = (l + r) >> 1, res = -INF;
    if (m >= x) res = max(res, qmax(p << 1, l, m, x, y));
    if (m < y) res = max(res, qmax(p << 1 | 1, m + 1, r, x, y));
    return res;
  }

  int qmin(int p, int l, int r, int x, int y) {
    if (x <= l && r <= y) return t[p].mn;
    pushdown(p, l, r);
    int m = (l + r) >> 1, res = INF;
    if (m >= x) res = min(res, qmin(p << 1, l, m, x, y));
    if (m < y) res = min(res, qmin(p << 1 | 1, m + 1, r, x, y));
    return res;
  }
} st;

/*
 * ============================================================
 * Name: Segment Tree Beats
 * Complexity: range add / queries O(log n); range chmin/chmax amortized
 *             O((n + q) log n) (each chmin only decreases a node's "number
 *             of distinct values"; potential analysis bounds the total)
 * Usage: range add, range chmin (a[i] = min(a[i], v)), range chmax
 *        (a[i] = max(a[i], v)), range sum / max / min queries, wrapped as
 *        segbeats; typical problem: Luogu P6242 (historic extrema need
 *        extra his fields and pushdown logic; this template is the core
 *        skeleton without them)
 * Principle: each node keeps mx/mx2 (max, strict second max) and mn/mn2
 *        (min, strict second min) with counts cmx/cmn; chmin over a segment
 *        whose max > v but second max < v only changes the maxima (O(1)
 *        tagging); otherwise recurse. tag is the range-add lazy tag;
 *        pushdown passes the parent's mx/mn down as chmin/chmax hints
 * Notes: INF must exceed the value range; all-equal segments have mx2 = -INF,
 *        mn2 = INF (sentinels); the whole-segment chmin condition is
 *        mx2 < v (strict, so the second max never leaks past v)
 * Usage pattern: build(1, 1, n); then st.add / st.chmin / st.chmax / st.qsum / st.qmax / st.qmin
 * ============================================================
 * Example (uncomment to compile; initial a = {3, 1, 4, 1, 5}):
 * signed main() {
 *   n = 5, a[1] = 3, a[2] = 1, a[3] = 4, a[4] = 1, a[5] = 5;
 *   st.build(1, 1, n);
 *   st.chmin(1, 1, n, 1, 5, 3);            // chmin 3 everywhere -> {3,1,3,1,3}
 *   cout << st.qsum(1, 1, n, 1, 5) << '\n'; // 11
 *   cout << st.qmax(1, 1, n, 1, 5) << ' ' << st.qmin(1, 1, n, 1, 5) << '\n'; // 3 1
 *   st.chmax(1, 1, n, 1, 5, 2);            // chmax 2 everywhere -> {3,2,3,2,3}
 *   cout << st.qsum(1, 1, n, 1, 5) << '\n'; // 13
 *   st.chmin(1, 1, n, 2, 4, 2);            // chmin 2 on [2,4] -> {3,2,2,2,3}
 *   cout << st.qsum(1, 1, n, 1, 5) << '\n'; // 12
 * }
 * ============================================================
 */
