#include <bits/stdc++.h>
using namespace std;
#define int long long

// 二维基础：整点向量运算（全部 long long 精确无误差）
struct P {
  int x, y;
};
P operator-(P a, P b) { return {a.x - b.x, a.y - b.y}; }
bool operator<(P a, P b) { return a.x != b.x ? a.x < b.x : a.y < b.y; }
bool operator==(P a, P b) { return a.x == b.x && a.y == b.y; }
int cross(P a, P b) { return a.x * b.y - a.y * b.x; }  // 叉积
int dot(P a, P b) { return a.x * b.x + a.y * b.y; }     // 点积
int cr(P a, P b, P c) { return cross(b - a, c - a); }   // ab × ac（>0 c 在 ab 左侧）

bool on_seg(P a, P b, P p) {  // p 是否在线段 ab 上（含端点）
  return cr(a, b, p) == 0 && dot(p - a, p - b) <= 0;
}
bool seg_inter(P a, P b, P c, P d) {  // 线段 ab 与 cd 是否相交（含端点接触）
  int d1 = cr(a, b, c), d2 = cr(a, b, d), d3 = cr(c, d, a), d4 = cr(c, d, b);
  if (d1 * d2 < 0 && d3 * d4 < 0) return true;  // 规范相交
  return (!d1 && on_seg(a, b, c)) || (!d2 && on_seg(a, b, d)) || (!d3 && on_seg(c, d, a)) || (!d4 && on_seg(c, d, b));
}
int area2(const vector<P>& p) {  // 多边形有向面积 ×2（逆时针为正）
  int s = 0;
  for (int i = 0, n = p.size(); i < n; ++i) s += cross(p[i], p[(i + 1) % n]);
  return s;
}
int pip(const vector<P>& p, P q) {  // 点 q 与多边形关系：0 外 / 1 边界 / 2 内（射线法，多边形任意简单形状）
  int cnt = 0, n = p.size();
  for (int i = 0; i < n; ++i) {
    P a = p[i], b = p[(i + 1) % n];
    if (on_seg(a, b, q)) return 1;
    if ((a.y > q.y) != (b.y > q.y)) {  // 边跨射线 y = q.y
      int d = cross(b - a, q - a);
      if (b.y > a.y ? d > 0 : d < 0) ++cnt;  // 交点在射线右侧
    }
  }
  return cnt & 1 ? 2 : 0;
}

// Andrew 凸包（严格凸，共线中间点剔除；全共线时输出两端点；CCW，首尾不重复）
vector<P> convex_hull(vector<P> p) {
  sort(p.begin(), p.end());
  p.erase(unique(p.begin(), p.end()), p.end());
  int m = p.size(), k = 0;
  if (m <= 2) return p;
  vector<P> h(2 * m + 1);
  for (int i = 0; i < m; ++i) {  // 下凸壳
    while (k > 1 && cr(h[k - 2], h[k - 1], p[i]) <= 0) --k;
    h[k++] = p[i];
  }
  for (int i = m - 2, t = k; i >= 0; --i) {  // 上凸壳
    while (k > t && cr(h[k - 2], h[k - 1], p[i]) <= 0) --k;
    h[k++] = p[i];
  }
  h.resize(k - 1);  // 最后一个点与 h[0] 重复
  if ((int)h.size() == 2 && h[0] == h[1]) h.resize(1);
  return h;
}

// 旋转卡壳：凸包直径（最远点对），返回距离平方；h 需为 convex_hull 结果
long long diameter2(const vector<P>& h) {
  int n = h.size();
  if (n == 1) return 0;
  if (n == 2) { P d = h[0] - h[1]; return dot(d, d); }
  long long res = 0;
  for (int i = 0, j = 2; i < n; ++i) {  // 枚举边 i→i+1，j 为对踵点候选
    P e = h[(i + 1) % n] - h[i];
    while (abs(cross(e, h[(j + 1) % n] - h[i])) > abs(cross(e, h[j] - h[i]))) j = (j + 1) % n;  // 面积最大
    P d1 = h[i] - h[j], d2 = h[(i + 1) % n] - h[j];
    res = max({res, dot(d1, d1), dot(d2, d2)});
  }
  return res;
}

// 平面最近点对（分治），返回距离平方；O(n log n)
constexpr int CN = 1e6 + 9;
P tmp_[CN];
long long d2_(P a, P b) { P d = a - b; return dot(d, d); }
long long closest2(P* a, int l, int r) {  // [l, r)，a 已按 (x, y) 排序
  if (r - l <= 1) return LLONG_MAX;
  if (r - l == 2) {  // 基例也要按 y 排好（父层归并的前提）
    if (a[l].y > a[l + 1].y) swap(a[l], a[l + 1]);
    return d2_(a[l], a[l + 1]);
  }
  int m = (l + r) / 2;
  int xm = a[m].x;
  long long d = min(closest2(a, l, m), closest2(a, m, r));
  inplace_merge(a + l, a + m, a + r, [](P u, P v) { return u.y != v.y ? u.y < v.y : u.x < v.x; });
  int c = 0;
  for (int i = l; i < r; ++i)
    if ((a[i].x - xm) * (a[i].x - xm) < d) tmp_[c++] = a[i];  // 中带
  for (int i = 0; i < c; ++i)  // 每个 tmp 点只需向后检查均摊 O(1) 个
    for (int j = i + 1; j < c && (tmp_[j].y - tmp_[i].y) * (tmp_[j].y - tmp_[i].y) < d; ++j)
      d = min(d, d2_(tmp_[i], tmp_[j]));
  return d;
}
long long closest_pair2(vector<P> p) {
  sort(p.begin(), p.end());
  return closest2(p.data(), 0, p.size());
}

/*
 * ============================================================
 * 名称：二维计算几何基础（整点，全部精确运算）
 * 复杂度：凸包 O(n log n)；旋转卡壳 O(n)；最近点对 O(n log n)
 * 用途：cross/dot/cr（叉积点积与方向判定）；on_seg/seg_inter（线段
 *       判交，含端点）；area2（多边形有向面积×2）；pip（点在多边形内，
 *       支持任意简单多边形）；convex_hull（Andrew 单调链，严格凸）；
 *       diameter2（旋转卡壳直径²）；closest_pair2（最近点对距离²）
 * 原理：全部用 long long 精确计算，无浮点误差（坐标需 |x|,|y| < 2^31
 *       量级以保证叉积不溢出）；凸包单调链两次扫描；旋转卡壳枚举边，
 *       对踵点随边单调移动；最近点对按 x 分治 + 按 y 归并 + 中带扫描
 * 注意：返回值为长度/面积的平方（避免开方误差）；凸包全共线时返回
 *       两端点（size=2）；坐标重复点已自动去重
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   vector<P> p{{0, 0}, {0, 2}, {2, 0}, {2, 2}, {1, 1}};
 *   auto h = convex_hull(p);                     // 正方形四角
 *   cout << h.size() << ' ' << area2(h) << '\n'; // 4 8（面积 4×2）
 *   cout << diameter2(h) << '\n';                // 8（对角线² = (2² + 2²)）
 *   cout << pip(h, {1, 1}) << pip(h, {3, 1}) << '\n';  // 2（内）0（外）
 *   cout << seg_inter({0, 0}, {2, 2}, {0, 2}, {2, 0}) << '\n';  // 1（对角线相交）
 *   vector<P> q{{0, 0}, {3, 4}};
 *   cout << closest_pair2(q) << '\n';            // 25
 * }
 */
