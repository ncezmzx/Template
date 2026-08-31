#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int LOG = 30;

struct wavelet {
  vector<vector<int>> pref;
  vector<int> zc;
  vector<int> a;

  void build(const vector<int> &v) {
    a = v;
    int n = (int)v.size();
    pref.assign(LOG, vector<int>(n + 1, 0));
    zc.assign(LOG, 0);
    vector<int> cur = v, nxt(n);
    for (int lv = LOG - 1; lv >= 0; --lv) {
      int *P = pref[lv].data(); // hoist the row pointer, avoid per-element vector indexing
      int z = 0;
      for (int i = 0; i < n; ++i) {
        int b = (cur[i] >> lv & 1) == 0;
        P[i + 1] = P[i] + b, z += b;
      }
      zc[lv] = z;
      int p0 = 0, p1 = z;
      for (int i = 0; i < n; ++i) {
        if (cur[i] >> lv & 1) nxt[p1++] = cur[i];
        else nxt[p0++] = cur[i];
      }
      cur.swap(nxt); // swap instead of copy
    }
  }

  int kth(int l, int r, int k) const {
    int res = 0;
    for (int lv = LOG - 1; lv >= 0; --lv) {
      const int *P = pref[lv].data();
      int zl = P[l], zr = P[r];
      int zeros = zr - zl;
      if (k < zeros) l = zl, r = zr;
      else {
        k -= zeros;
        l = zc[lv] + (l - zl);
        r = zc[lv] + (r - zr);
        res |= 1 << lv;
      }
    }
    return res;
  }

  int rank_lt(int l, int r, int x) const {
    int res = 0;
    for (int lv = LOG - 1; lv >= 0; --lv) {
      const int *P = pref[lv].data();
      int zl = P[l], zr = P[r];
      if (x >> lv & 1) {
        res += zr - zl;
        l = zc[lv] + (l - zl);
        r = zc[lv] + (r - zr);
      }
      else l = zl, r = zr;
    }
    return res;
  }
};

struct dyn_wavelet {
  vector<wavelet> wm;
  vector<vector<int>> data;

  void clear() {
    wm.clear();
    data.clear();
  }
  int size() const {
    int s = 0;
    for (auto &d : data) s += (int)d.size();
    return s;
  }

  void insert(int x) {
    vector<int> v{x};
    int i = 0;
    while (i < (int)wm.size() && !wm[i].a.empty()) {
      v.insert(v.end(), data[i].begin(), data[i].end());
      wm[i] = wavelet();
      data[i].clear();
      ++i;
    }
    if (i == (int)wm.size()) wm.resize(i + 1), data.resize(i + 1);
    data[i] = v;
    wm[i].build(v);
  }

  bool erase(int x) {
    for (int i = 0; i < (int)wm.size(); ++i) {
      auto it = find(data[i].begin(), data[i].end(), x);
      if (it != data[i].end()) {
        data[i].erase(it);
        wm[i].build(data[i]);
        return true;
      }
    }
    return false;
  }

  int kth(int k) const {
    int res = 0;
    vector<int> l(wm.size(), 0), r(wm.size());
    for (int i = 0; i < (int)wm.size(); ++i) r[i] = (int)wm[i].a.size();
    for (int lv = LOG - 1; lv >= 0; --lv) {
      int zeros = 0;
      for (int i = 0; i < (int)wm.size(); ++i) {
        if (l[i] == r[i]) continue;
        zeros += wm[i].pref[lv][r[i]] - wm[i].pref[lv][l[i]];
      }
      if (k < zeros) {
        for (int i = 0; i < (int)wm.size(); ++i) {
          if (l[i] == r[i]) continue;
          int zl = wm[i].pref[lv][l[i]], zr = wm[i].pref[lv][r[i]];
          l[i] = zl, r[i] = zr;
        }
      }
      else {
        k -= zeros;
        res |= 1 << lv;
        for (int i = 0; i < (int)wm.size(); ++i) {
          if (l[i] == r[i]) continue;
          int zl = wm[i].pref[lv][l[i]], zr = wm[i].pref[lv][r[i]];
          l[i] = wm[i].zc[lv] + (l[i] - zl);
          r[i] = wm[i].zc[lv] + (r[i] - zr);
        }
      }
    }
    return res;
  }

  int rank_lt(int x) const {
    int res = 0;
    for (int i = 0; i < (int)wm.size(); ++i)
      if (!wm[i].a.empty()) res += wm[i].rank_lt(0, (int)wm[i].a.size(), x);
    return res;
  }

  int count(int x) const { return rank_lt(x + 1) - rank_lt(x); }
};

/*
 * ============================================================
 * Name: dynamic Wavelet Matrix via binary grouping (insertable value sequence)
 * Complexity: insert amortized O(log n * LOG); kth / rank_lt O(LOG * log n)
 * Usage: dynamically maintained insert-only multiset: insert(x); kth(k) global
 *        k-th smallest (0-based);
 *        rank_lt(x) global count of values < x; count(x) count of x; each block
 *        is a static Wavelet Matrix over [0, 2^LOG).
 * Principle: logarithmic method: blocks of sizes exactly 1, 2, 4, ...; an
 *            insert creates a size-1 block and repeatedly merges with an equal-
 *            sized block (binary carry), keeping O(log n) blocks; queries
 *            descend bitwise across all blocks in parallel (kth) or accumulate
 *            (rank_lt)
 * Notes: values must lie in [0, 2^LOG) (LOG = 30; shift negatives into range);
 *        erase is a simple O(sum of block sizes) rebuild, so prefer a
 *        persistent segment tree / balanced tree for deletion-heavy workloads
 * ============================================================
 */
