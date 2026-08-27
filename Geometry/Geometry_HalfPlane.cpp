#include <bits/stdc++.h>
using namespace std;

// 半平面交（S&I 算法）：半平面 = 有向直线 p + t·v 的左侧；O(n log n)
const double EPS = 1e-9;
struct P {
  double x, y;
};
P operator+(P a, P b) { return {a.x + b.x, a.y + b.y}; }
P operator-(P a, P b) { return {a.x - b.x, a.y - b.y}; }
P operator*(P a, double k) { return {a.x * k, a.y * k}; }
double cross(P a, P b) { return a.x * b.y - a.y * b.x; }

struct HP {
  P p, v;  // 半平面：沿 p → p + v 方向行进的左侧
  double ang;
  HP(P p_ = {0, 0}, P v_ = {1, 0}) : p(p_), v(v_), ang(atan2(v_.y, v_.x)) {}
};
bool onleft(const HP& l, const P& t) { return cross(l.v, t - l.p) > 0; }
P line_inter(const HP& a, const HP& b) {  // 两直线交点（需不平行）
  double t = cross(b.p - a.p, b.v) / cross(a.v, b.v);
  return a.p + a.v * t;
}

// 返回交区域多边形（顶点按 CCW）；可能为空（区域为空 / 无界未加框）
// 注意：无界时应先加入足够大的矩形框（四条半平面）
vector<P> hpi(vector<HP> ls) {
  sort(ls.begin(), ls.end(), [](const HP& a, const HP& b) { return a.ang < b.ang; });
  int n = ls.size(), first = 0, last = 0;
  vector<HP> q(n);
  vector<P> p(n);
  q[0] = ls[0];
  for (int i = 1; i < n; i++) {
    while (first < last && !onleft(ls[i], p[last - 1])) --last;
    while (first < last && !onleft(ls[i], p[first])) ++first;
    q[++last] = ls[i];
    if (last - first >= 1 && fabs(cross(q[last].v, q[last - 1].v)) < EPS) {  // 平行去冗余
      --last;
      if (onleft(q[last], ls[i].p)) q[last] = ls[i];  // 保留更紧的
    }
    if (last - first >= 1) p[last - 1] = line_inter(q[last - 1], q[last]);
  }
  while (first < last && !onleft(q[first], p[last - 1])) --last;
  if (last - first <= 1) return {};
  p[last] = line_inter(q[last], q[first]);
  return vector<P>(p.begin() + first, p.begin() + last + 1);
}

double area(const vector<P>& p) {  // 多边形面积（无符号）
  double s = 0;
  for (int i = 1, n = p.size(); i + 1 < n; ++i) s += cross(p[i] - p[0], p[i + 1] - p[0]);
  return fabs(s) / 2;
}

/*
 * ============================================================
 * 名称：半平面交（S&I / 增量单调队列算法）
 * 复杂度：O(n log n)（排序主导）
 * 用途：求 n 个半平面的公共区域（凸多边形，可能为空）：
 *       典型题：多边形内核、线性规划可行域、二维决策单调问题
 * 接口：HP(p, v)：沿 p→p+v 左侧的半平面；hpi(ls) 返回 CCW 顶点集；
 *       area(poly) 求面积
 * 原理：按极角排序后单调双端队列维护候选边——新半平面不断弹出
 *       位于其右侧的旧交点（队尾/队首），平行同向保留更紧者，
 *       平行异向导致空区域；最后环形裁剪队尾
 * 注意：区域可能无界时先加大矩形框；EPS 视坐标量级调整；
 *       边界上的点是否保留由 onleft 的严格性决定（当前严格 > 0）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   // 正方形 [0,1]² 的四条内侧半平面 + 对角线 x+y >= 0.5? 取 x + y <= 1.8 的左下侧：
 *   vector<HP> ls;
 *   ls.push_back({{0, 0}, {1, 0}});   // y >= 0（沿 +x 左侧）
 *   ls.push_back({{1, 0}, {0, 1}});   // x <= 1（沿 +y 左侧）
 *   ls.push_back({{1, 1}, {-1, 0}});  // y <= 1（沿 -x 左侧）
 *   ls.push_back({{0, 1}, {0, -1}});  // x >= 0（沿 -y 左侧）
 *   auto poly = hpi(ls);
 *   cout << poly.size() << ' ' << area(poly) << '\n';  // 4 1
 *   ls.push_back({{0.5, 0.5}, {1, 1}});  // 再切掉右上角（x + y <= ... 对角切）
 *   auto poly2 = hpi(ls);
 *   cout << poly2.size() << ' ' << area(poly2) << '\n'; // 3 0.5（保留 y > x 一侧的小三角）
 * }
 */
