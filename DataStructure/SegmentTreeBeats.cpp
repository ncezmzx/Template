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
 * 名称：Segment Tree Beats（势能线段树 / 吉老师线段树）
 * 复杂度：区间加/查询 O(log n)；区间 chmin/chmax 均摊 O((n + q) log n)
 *         （每次 chmin 只让节点"不同取值的种数"减少，势能分析保证总代价）
 * 用途：支持 区间加 add、区间取 min（a[i] = min(a[i], v)）、区间取 max
 *       （a[i] = max(a[i], v)）、区间和/最大值/最小值查询；
 *       典型题：洛谷 P6242（含历史最值时需另加 his 字段与对应 pushdown 逻辑，
 *       本模板为不含历史最值的核心骨架）
 * 原理：每个节点维护 mx/mx2（最大、严格次大）与 mn/mn2（最小、严格次小）
 *       及出现次数 cmx/cmn；chmin 时若整段最大值 > v 且次大值 < v，则可
 *       只改最大值（O(1) 打标记）；否则递归。tag 为区间加懒标记，
 *       pushdown 时把父节点的 mx/mn 作为 chmin/chmax 提示下传
 * 注意：INF 需大于值域；值全相等时 mx2 = -INF、mn2 = INF（哨兵）；
 *       chmin 的整段条件为 mx2 < v（严格小于，保证次大值不越界）
 * 用法：build(1, 1, n)；之后 st.add / st.chmin / st.chmax / st.qsum / st.qmax / st.qmin
 * ============================================================
 * 使用示例（编译时取消注释；初值 a = {3, 1, 4, 1, 5}）：
 * signed main() {
 *   n = 5, a[1] = 3, a[2] = 1, a[3] = 4, a[4] = 1, a[5] = 5;
 *   st.build(1, 1, n);
 *   st.chmin(1, 1, n, 1, 5, 3);            // 全体取 min 3 → {3,1,3,1,3}
 *   cout << st.qsum(1, 1, n, 1, 5) << '\n'; // 11
 *   cout << st.qmax(1, 1, n, 1, 5) << ' ' << st.qmin(1, 1, n, 1, 5) << '\n'; // 3 1
 *   st.chmax(1, 1, n, 1, 5, 2);            // 全体取 max 2 → {3,2,3,2,3}
 *   cout << st.qsum(1, 1, n, 1, 5) << '\n'; // 13
 *   st.chmin(1, 1, n, 2, 4, 2);            // [2,4] 取 min 2 → {3,2,2,2,3}
 *   cout << st.qsum(1, 1, n, 1, 5) << '\n'; // 12
 * }
 * ============================================================
 */
