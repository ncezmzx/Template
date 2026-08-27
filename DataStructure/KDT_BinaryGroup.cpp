#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int N = 1e5 + 9;
struct pt {
  int x, y, w;
} p[N];

struct kdnode {
  int l, r, x, y, w, mnx, mny, mxx, mxy, sum;
} t[N];

int tot, dim;
bool cmp(const pt& a, const pt& b) {
  if (dim == 0) return a.x < b.x;
  return a.y < b.y;
}

int build(int l, int r) {
  if (l > r) return 0;
  int mid = (l + r) >> 1;
  dim = __lg(r - l + 1) & 1;
  nth_element(p + l, p + mid, p + r + 1, cmp);
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

int rect_query(int u, int x1, int y1, int x2, int y2) {
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

long long box_dist(int u, int x, int y) {
  if (!u) return 0x3f3f3f3f3f3f3f3f;
  long long dx = 0, dy = 0;
  if (x < t[u].mnx) dx = t[u].mnx - x;
  if (x > t[u].mxx) dx = x - t[u].mxx;
  if (y < t[u].mny) dy = t[u].mny - y;
  if (y > t[u].mxy) dy = y - t[u].mxy;
  return dx * dx + dy * dy;
}

long long nearest(int u, int x, int y) {
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

/*
 * ============================================================
 * 名称：K-D Tree（二维，矩形权值和查询 + 最近点查询）
 * 复杂度：build O(n log n)（按层交替维度 nth_element 分割）；
 *         rect_query / nearest 期望 O(sqrt(n))（OI 常规数据，构造数据最坏 O(n)）
 * 用途：二维带权点集的矩形和查询、最近点距离（欧氏距离平方）；
 *       配合二进制分组可支持动态插入（见注释示例）
 * 原理：按维度交替以中位数分割（dim = 层数奇偶），节点维护包围盒
 *       （mnx/mny/mxx/mxy）与子树权和；矩形查询：包围盒相交剪枝、
 *       完全包含直接返回子树和；最近点：按包围盒距离剪枝优先搜索
 * 注意：build 前把点放入全局数组 p[l..r]（1-based），tot 清零；
 *       nearest 包含查询点自身（在点集中时返回 0），排除自身需特判
 * ============================================================
 * 使用示例（编译时取消注释；矩形权和）：
 * signed main() {
 *   int n;
 *   cin >> n;
 *   for (int i = 1; i <= n; ++i) cin >> p[i].x >> p[i].y >> p[i].w;
 *   int root = build(1, n);
 *   int q;
 *   cin >> q;
 *   while (q--) {
 *     int x1, y1, x2, y2;
 *     cin >> x1 >> y1 >> x2 >> y2;
 *     cout << rect_query(root, x1, y1, x2, y2) << '\n';
 *   }
 * }
 * // 二进制分组动态插入（均摊 O(log n * sqrt n)）：
 * // struct dyn_kdt {
 * //   vector<vector<pt>> g;
 * //   vector<int> rt;
 * //   void insert(pt q) {
 * //     vector<pt> v{q};
 * //     int i = 0;
 * //     while (i < (int)g.size() && !g[i].empty()) {
 * //       v.insert(v.end(), g[i].begin(), g[i].end());
 * //       g[i].clear(), rt[i] = 0;
 * //       ++i;
 * //     }
 * //     if (i == (int)g.size()) g.resize(i + 1), rt.resize(i + 1);
 * //     g[i] = v, tot = 0;
 * //     for (int j = 0; j < (int)v.size(); ++j) p[j + 1] = v[j];
 * //     rt[i] = build(1, (int)v.size());
 * //   }
 * //   int rect(int x1, int y1, int x2, int y2) {
 * //     int s = 0;
 * //     for (int i = 0; i < (int)rt.size(); ++i) s += rect_query(rt[i], x1, y1, x2, y2);
 * //     return s;
 * //   }
 * // };
 * ============================================================
 */
