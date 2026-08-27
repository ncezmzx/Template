#include <bits/stdc++.h>
using namespace std;

// CDQ 分治（三维偏序计数）：统计 i < j 且 a_i <= a_j, b_i <= b_j, c_i <= c_j 的点对数
// 也可改贡献方式求"每个点被多少点支配"等
constexpr int N = 2e5 + 9;
struct Node {
  int a, b, c;
};
Node v_[N], tmp_[N];
int bit_[N], C_;
void bit_add(int x, int w) {
  for (; x <= C_; x += x & -x) bit_[x] += w;
}
long long bit_sum(int x) {
  long long r = 0;
  for (; x; x -= x & -x) r += bit_[x];
  return r;
}
long long ans_;

void cdq(int l, int r) {  // [l, r)：v_ 已按 (a, 原序) 排序
  if (r - l <= 1) return;
  int m = (l + r) / 2;
  cdq(l, m), cdq(m, r);
  int p = l;
  for (int t = m; t < r; ++t) {  // 左半对右半的贡献：b、c 二维偏序
    while (p < m && v_[p].b <= v_[t].b) bit_add(v_[p].c, 1), ++p;
    ans_ += bit_sum(v_[t].c);
  }
  for (int t = l; t < p; ++t) bit_add(v_[t].c, -1);  // 还原
  int i = l, j = m, k = l;  // 按 b 归并（稳定，保序）
  while (i < m && j < r) tmp_[k++] = v_[i].b <= v_[j].b ? v_[i++] : v_[j++];
  while (i < m) tmp_[k++] = v_[i++];
  while (j < r) tmp_[k++] = v_[j++];
  copy(tmp_ + l, tmp_ + r, v_ + l);
}

// pts 每项 {a, b, c}（c 需在 [1, 1e9] 内，内部离散化）
long long count_3d(vector<array<int, 3>> pts) {
  int n = pts.size();
  vector<int> cs;
  for (auto& p : pts) cs.push_back(p[2]);
  sort(cs.begin(), cs.end()), cs.erase(unique(cs.begin(), cs.end()), cs.end());
  C_ = cs.size();
  for (int i = 0; i < n; ++i) v_[i] = {pts[i][0], pts[i][1], (int)(lower_bound(cs.begin(), cs.end(), pts[i][2]) - cs.begin()) + 1};
  // 按 (a, b, c) 全序排序：可比对 {u, v}（u 逐维 <= v）中 u 的字典序必 <= v，
  // 支配者恒排在被支配者之前（相等三元组任意序均可比），跨层只需判 b、c
  stable_sort(v_, v_ + n, [](const Node& x, const Node& y) {
    return x.a != y.a ? x.a < y.a : (x.b != y.b ? x.b < y.b : x.c < y.c);
  });
  ans_ = 0;
  cdq(0, n);
  return ans_;
}

/*
 * ============================================================
 * 名称：CDQ 分治（三维偏序）
 * 复杂度：O(n log² n)（分治层 × BIT）
 * 用途：统计三维偏序的可比点对数（经典：陌上花开 P3810：对
 *       每个点求支配它的点数，本题即其总和形态——无序对 {u,v}
 *       只要以 u≤v（逐维）可比便计入一次）；CDQ 思想可推广到
 *       "左半修改对右半询问贡献" 的一类整体二维问题
 * 接口：count_3d({{a, b, c}, ...}) 统计无序对 {u, v} 中
 *       min 逐维 <= max（可比）的对数（相等三元组按出现序计一次）
 * 原理：第一维按 (a, b, c) 全序排序（支配者必排在前）；第二维分治
 *       归并——归并时左半按 b 序插入 BIT（键为 c），右半查询前缀和
 *       （a 已由排序保证）；每层 O(n log n)，共 log n 层
 * 注意：统计"严格三维不同"需先去重相同三元组（组内组合数另算）；
 *       修改/询问混合型 CDQ 需把询问也作为元素放入 v_
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   cout << count_3d({{1, 1, 1}, {2, 2, 2}, {3, 3, 3}}) << '\n';   // 3（全部可比）
 *   cout << count_3d({{2, 1, 1}, {1, 2, 2}, {3, 3, 3}}) << '\n';   // 2（(2,1,1)<(3,3,3) 与 (1,2,2)<(3,3,3)；首两项 a 逆序不可配）
 *   cout << count_3d({{1, 1, 1}, {1, 1, 1}}) << '\n';              // 1（相等三元组按出现序计一次）
 * }
 */
