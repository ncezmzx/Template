#include <bits/stdc++.h>
using namespace std;
#define int long long

// K-D Tree (2D): weighted rectangle-sum queries + nearest-neighbor queries
template <size_t N>
struct kdtree {
  struct pt {
    int x, y, w;
  } p[N];
  struct kdnode {
    int l, r, x, y, w, mnx, mny, mxx, mxy, sum;
  } t[N];
  int tot, dim;
  static bool cmpx(const pt& a, const pt& b) { return a.x < b.x; }
  static bool cmpy(const pt& a, const pt& b) { return a.y < b.y; }
  int build(int l, int r) {  // points in p[l..r]; alternating split dimension
    if (l > r) return 0;
    int mid = (l + r) >> 1;
    dim = __lg(r - l + 1) & 1;
    // comparator with no global dependency (avoids reading dim per comparison)
    nth_element(p + l, p + mid, p + r + 1, dim ? cmpy : cmpx);
    int u = ++tot;
    t[u].x = p[mid].x, t[u].y = p[mid].y, t[u].w = p[mid].w;
    t[u].l = build(l, mid - 1);
    t[u].r = build(mid + 1, r);
    t[u].mnx = t[u].mxx = t[u].x;
    t[u].mny = t[u].mxy = t[u].y;
    if (t[u].l) {
      t[u].mnx = min(t[u].mnx, t[t[u].l].mnx), t[u].mxx = max(t[u].mxx, t[t[u].l].mxx);
      t[u].mny = min(t[u].mny, t[t[u].l].mny), t[u].mxy = max(t[u].mxy, t[t[u].l].mxy);
    }
    if (t[u].r) {
      t[u].mnx = min(t[u].mnx, t[t[u].r].mnx), t[u].mxx = max(t[u].mxx, t[t[u].r].mxx);
      t[u].mny = min(t[u].mny, t[t[u].r].mny), t[u].mxy = max(t[u].mxy, t[t[u].r].mxy);
    }
    t[u].sum = t[u].w + t[t[u].l].sum + t[t[u].r].sum;
    return u;
  }
  int rect_query(int u, int x1, int y1, int x2, int y2) {  // sum of weights in rectangle
    if (!u) return 0;
    if (t[u].mxx < x1 || t[u].mnx > x2 || t[u].mxy < y1 || t[u].mny > y2) return 0;
    if (x1 <= t[u].mnx && t[u].mxx <= x2 && y1 <= t[u].mny && t[u].mxy <= y2)
      return t[u].sum;
    int res = 0;
    if (x1 <= t[u].x && t[u].x <= x2 && y1 <= t[u].y && t[u].y <= y2) res += t[u].w;
    res += rect_query(t[u].l, x1, y1, x2, y2);
    res += rect_query(t[u].r, x1, y1, x2, y2);
    return res;
  }
  long long box_dist(int u, int x, int y) {  // squared distance point -> bounding box
    if (!u) return 0x3f3f3f3f3f3f3f3f;
    long long dx = 0, dy = 0;
    if (x < t[u].mnx) dx = t[u].mnx - x;
    if (x > t[u].mxx) dx = x - t[u].mxx;
    if (y < t[u].mny) dy = t[u].mny - y;
    if (y > t[u].mxy) dy = y - t[u].mxy;
    return dx * dx + dy * dy;
  }
  long long nearest(int u, int x, int y) {  // min squared distance to any point
    if (!u) return 0x3f3f3f3f3f3f3f3f;
    long long res = (long long)(t[u].x - x) * (t[u].x - x) + (long long)(t[u].y - y) * (t[u].y - y);
    long long dl = box_dist(t[u].l, x, y), dr = box_dist(t[u].r, x, y);
    if (dl < dr) {
      if (dl < res) res = min(res, nearest(t[u].l, x, y));
      if (dr < res) res = min(res, nearest(t[u].r, x, y));
    } else {
      if (dr < res) res = min(res, nearest(t[u].r, x, y));
      if (dl < res) res = min(res, nearest(t[u].l, x, y));
    }
    return res;
  }
};

/*
 * ============================================================
 * Name: K-D Tree (2D, rectangle weight-sum query + nearest-point query)
 * Complexity: build O(n log n) (nth_element splits, alternating dimension per
 *             level); rect_query / nearest expected O(sqrt n) on typical OI
 *             data (adversarial data worst-case O(n))
 * Usage: 2D weighted point set: rectangle-sum queries and nearest-point
 *        distance (squared Euclidean), wrapped as kdtree<N>; combine with
 *        binary grouping for dynamic inserts (sketch below)
 * Principle: median split with alternating dimensions (dim = level parity);
 *        nodes keep a bounding box (mnx/mny/mxx/mxy) and subtree weight sum;
 *        rectangle query prunes by box intersection and returns the subtree
 *        sum on full containment; nearest searches the closer box first
 * Notes: put points in p[1..n] before build(1, n) (and clear tot);
 *        nearest includes the query point itself (returns 0 when it is in the
 *        set); special-case if you need to exclude it
 * ============================================================
 * Example (uncomment to compile; rectangle weight sums):
 * static kdtree<100009> kd;
 * signed main() {
 *   int n;
 *   cin >> n;
 *   for (int i = 1; i <= n; ++i) cin >> kd.p[i].x >> kd.p[i].y >> kd.p[i].w;
 *   int root = kd.build(1, n);
 *   int q;
 *   cin >> q;
 *   while (q--) {
 *     int x1, y1, x2, y2;
 *     cin >> x1 >> y1 >> x2 >> y2;
 *     cout << kd.rect_query(root, x1, y1, x2, y2) << '\n';
 *   }
 * }
 * // binary-grouping dynamic inserts (amortized O(log n * sqrt n)):
 * // struct dyn_kdt {
 * //   vector<vector<kdtree<100009>::pt>> g;
 * //   vector<int> rt;
 * //   kdtree<100009> kd;
 * //   void insert(kdtree<100009>::pt q) {
 * //     vector<kdtree<100009>::pt> v{q};
 * //     int i = 0;
 * //     while (i < (int)g.size() && !g[i].empty()) {
 * //       v.insert(v.end(), g[i].begin(), g[i].end());
 * //       g[i].clear(), rt[i] = 0;
 * //       ++i;
 * //     }
 * //     if (i == (int)g.size()) g.resize(i + 1), rt.resize(i + 1);
 * //     g[i] = v, kd.tot = 0;
 * //     for (int j = 0; j < (int)v.size(); ++j) kd.p[j + 1] = v[j];
 * //     rt[i] = kd.build(1, (int)v.size());
 * //   }
 * //   int rect(int x1, int y1, int x2, int y2) {
 * //     int s = 0;
 * //     for (int i = 0; i < (int)rt.size(); ++i) s += kd.rect_query(rt[i], x1, y1, x2, y2);
 * //     return s;
 * //   }
 * // };
 * ============================================================
 */
