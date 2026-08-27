#include <bits/stdc++.h>
using namespace std;

// 最小圆覆盖（增量法，期望 O(n)）
struct P {
  double x, y;
};
P operator-(P a, P b) { return {a.x - b.x, a.y - b.y}; }
double dist2(P a, P b) { P d = a - b; return d.x * d.x + d.y * d.y; }
double dist(P a, P b) { return sqrt(dist2(a, b)); }

struct C {
  P c;
  double r;
};
C circle2(P a, P b) { return {(P){(a.x + b.x) / 2, (a.y + b.y) / 2}, dist(a, b) / 2}; }
P circumcenter(P a, P b, P c) {  // 三角形外接圆圆心（三点不共线）
  double a1 = b.x - a.x, b1 = b.y - a.y, a2 = c.x - a.x, b2 = c.y - a.y;
  double d1 = a1 * a1 + b1 * b1, d2 = a2 * a2 + b2 * b2, d = 2 * (a1 * b2 - a2 * b1);
  return {(P){a.x + (d1 * b2 - d2 * b1) / d, a.y + (a1 * d2 - a2 * d1) / d}};
}
C circle3(P a, P b, P c) {
  P o = circumcenter(a, b, c);
  return {o, dist(o, a)};
}
bool outside(P p, const C& c) { return dist2(p, c.c) > c.r * c.r + 1e-7; }

C min_circle(vector<P> p) {  // 随机增量：期望 O(n)
  int n = p.size();
  if (n == 0) return {{0, 0}, -1};
  shuffle(p.begin(), p.end(), mt19937(0x5f5f));
  C c = {p[0], 0};
  for (int i = 1; i < n; ++i)
    if (outside(p[i], c)) {
      c = {p[i], 0};
      for (int j = 0; j < i; ++j)
        if (outside(p[j], c)) {
          c = circle2(p[i], p[j]);
          for (int k = 0; k < j; ++k)
            if (outside(p[k], c)) c = circle3(p[i], p[j], p[k]);
        }
    }
  return c;
}

/*
 * ============================================================
 * 名称：最小圆覆盖（随机增量法）
 * 复杂度：期望 O(n)（随机打乱后三层增量为调和级数级）
 * 用途：覆盖平面上 n 个点的最小圆（圆心半径）；
 *       附 circle2/circle3（两点/三点定圆）可单独复用
 * 原理：增量构造——固定前 i-1 个点的最小圆，若 p[i] 在圆外则
 *       p[i] 必在新的圆边界上；同理固定两层，三点定圆。
 *       打乱后期望三层循环均为 O(当前规模) 的几何衰减
 * 注意：eps 判定（1e-7）按坐标量级调整；三点共线时 circle3
 *       不被调用到（外层 outside 保证）；n = 0 返回 r = -1
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   vector<P> p{{0, 0}, {0, 2}, {2, 0}, {2, 2}};
 *   C c = min_circle(p);
 *   cout << fixed << setprecision(6);
 *   cout << c.c.x << ' ' << c.c.y << ' ' << c.r << '\n';  // 1.000000 1.000000 1.414214
 *   p.push_back({10, 10});
 *   c = min_circle(p);
 *   cout << c.c.x << ' ' << c.c.y << ' ' << c.r << '\n';  // 5.000000 5.000000 7.071068（(0,0) 与 (10,10) 为直径）
 * }
 */
