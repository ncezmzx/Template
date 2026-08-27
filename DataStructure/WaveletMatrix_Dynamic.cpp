#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int LOG = 30;

struct wavelet {
  vector<vector<int>> pref;
  vector<int> zc;
  vector<int> a;

  void build(const vector<int>& v) {
    a = v;
    int n = (int)v.size();
    pref.assign(LOG, vector<int>(n + 1, 0));
    zc.assign(LOG, 0);
    vector<int> cur = v, nxt(n);
    for (int lv = LOG - 1; lv >= 0; --lv) {
      int z = 0;
      for (int i = 0; i < n; ++i) {
        pref[lv][i + 1] = pref[lv][i] + ((cur[i] >> lv & 1) == 0);
        z += (cur[i] >> lv & 1) == 0;
      }
      zc[lv] = z;
      int p0 = 0, p1 = z;
      for (int i = 0; i < n; ++i) {
        if (cur[i] >> lv & 1) nxt[p1++] = cur[i];
        else nxt[p0++] = cur[i];
      }
      cur = nxt;
    }
  }

  int kth(int l, int r, int k) const {
    int res = 0;
    for (int lv = LOG - 1; lv >= 0; --lv) {
      int zl = pref[lv][l], zr = pref[lv][r];
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
      int zl = pref[lv][l], zr = pref[lv][r];
      if (x >> lv & 1) {
        res += zr - zl;
        l = zc[lv] + (l - zl);
        r = zc[lv] + (r - zr);
      } else l = zl, r = zr;
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
    for (auto& d : data) s += (int)d.size();
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
      } else {
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
 * 名称：基于二进制分组的动态 Wavelet Matrix（可插入的权值序列）
 * 复杂度：insert 均摊 O(log n * LOG)（二进制分组：小块不断合并重建，
 *         每个元素每轮至多参与 O(log n) 次重建）；kth/rank_lt O(LOG * log n)
 * 用途：动态维护一个多重集合（仅插入，可选 erase）：
 *       insert(x) 插入；kth(k) 全局第 k 小（0-based）；
 *       rank_lt(x) 全局 < x 的个数；count(x) 等于 x 的个数；
 *       每个"块"是一棵静态 Wavelet Matrix（值域 [0, 2^LOG)）
 * 原理：二进制分组（logarithmic method）：维护大小恰为 1,2,4,... 的块，
 *       插入时新建大小为 1 的块并不断与同大小块合并重建（二进制进位），
 *       保证任意时刻块数 O(log n)；查询对所有块并行按位下降（kth）或
 *       累加（rank_lt）
 * 注意：值域需在 [0, 2^LOG)（LOG = 30，负数请加偏移）；
 *       erase 为 O(Σ 块大小) 的简单实现（找到含 x 的块重建），
 *       删除频繁请改用可持久化线段树/平衡树
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   dyn_wavelet st;
 *   for (int x : {5, 3, 9, 1, 7, 5}) st.insert(x);
 *   cout << st.kth(0) << ' ' << st.kth(3) << '\n';   // 1 5
 *   cout << st.rank_lt(6) << '\n';                   // 4（1,3,5,5）
 *   cout << st.count(5) << '\n';                     // 2
 * }
 * ============================================================
 */
