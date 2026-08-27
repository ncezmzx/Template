#include <bits/stdc++.h>
using namespace std;

// 莫队算法：离线区间询问，指针跳动均摊 O((n+q)√n)
// 普通版示例：区间不同数个数；带修版：单点改值 + 区间不同数个数
constexpr int N = 5e5 + 9, MQ = 5e5 + 9;
int a[N], cnt_[N], cur, ans[MQ];

struct Qry {
  int l, r, t, id;
};
inline void mo_add(int i) { cur += cnt_[a[i]]++ == 0; }
inline void mo_del(int i) { cur -= --cnt_[a[i]] == 0; }

// 普通莫队：a[1..n]，询问 [l, r]（1-indexed）→ 区间不同数个数
vector<int> mo_distinct(int n, const vector<int>& val, vector<array<int, 2>> qs) {
  int q = qs.size(), B = max(1, (int)(n / max(1.0, sqrt((double)q))));
  vector<Qry> Q(q);
  for (int i = 0; i < q; ++i) Q[i] = {qs[i][0], qs[i][1], 0, i};
  sort(Q.begin(), Q.end(), [&](const Qry& x, const Qry& y) {
    int bx = x.l / B, by = y.l / B;
    if (bx != by) return bx < by;
    return bx & 1 ? x.r > y.r : x.r < y.r;  // 奇偶块优化
  });
  for (int i = 1; i <= n; ++i) a[i] = val[i], cnt_[a[i]] = 0;
  cur = 0;
  int l = 1, r = 0;
  vector<int> res(q);
  for (auto& Q_ : Q) {
    while (l > Q_.l) mo_add(--l);
    while (r < Q_.r) mo_add(++r);
    while (l < Q_.l) mo_del(l++);
    while (r > Q_.r) mo_del(r--);
    res[Q_.id] = cur;
  }
  return res;
}

struct Upd {
  int pos, val;
};
// 带修莫队：单点改值（ups 按时间顺序）+ 区间不同数个数
vector<int> mo_distinct_modify(int n, const vector<int>& val, const vector<array<int, 2>> qs, const vector<array<int, 2>> ups) {
  int q = qs.size(), t = ups.size();
  vector<array<int, 2>> up(ups);  // 可变副本（时间指针回退需要改写）
  int B = max(1, (int)pow(n, 2.0 / 3));
  vector<Qry> Q(q);
  for (int i = 0; i < q; ++i) Q[i] = {qs[i][0], qs[i][1], t, i};  // t = 询问前的修改数（本接口假设全部修改生效）
  sort(Q.begin(), Q.end(), [&](const Qry& x, const Qry& y) {
    int bx = x.l / B, by = y.l / B;
    if (bx != by) return bx < by;
    int rx = x.r / B, ry = y.r / B;
    if (rx != ry) return rx < ry;
    return x.t < y.t;
  });
  for (int i = 1; i <= n; ++i) a[i] = val[i];
  for (int i = 0; i < N; ++i) cnt_[i] = 0;
  cur = 0;
  int l = 1, r = 0, now = 0;
  vector<int> res(q);
  for (auto& Q_ : Q) {
    while (now < Q_.t) {  // 时间指针前移：应用第 now 个修改
      int p = up[now][0];
      if (l <= p && p <= r) mo_del(p), swap(a[p], up[now][1]), mo_add(p);
      else swap(a[p], up[now][1]);
      ++now;
    }
    while (now > Q_.t) {
      --now;
      int p = up[now][0];
      if (l <= p && p <= r) mo_del(p), swap(a[p], up[now][1]), mo_add(p);
      else swap(a[p], up[now][1]);
    }
    while (l > Q_.l) mo_add(--l);
    while (r < Q_.r) mo_add(++r);
    while (l < Q_.l) mo_del(l++);
    while (r > Q_.r) mo_del(r--);
    res[Q_.id] = cur;
  }
  return res;
}

/*
 * ============================================================
 * 名称：莫队算法（普通 + 带修改）
 * 复杂度：普通 O((n+q)√n)；带修 O(n^{5/3})（块长 n^{2/3}）
 * 用途：离线区间统计（无修改强行在线代价高、区间可 O(1) 增删）：
 *       示例为区间不同数个数；改 mo_add/mo_del 适配众数、
 *       区间 [x,y] 值域计数、异或和种数等
 * 接口：mo_distinct(n, val[1..n], {{l, r}, ...})；
 *       mo_distinct_modify(n, val, qs, ups)（ups = {pos, val}，
 *       假设全部修改在询问前生效——如需交错时刻，把每个询问的
 *       t 设为其前面的修改数即可）
 * 原理：询问按 (l 块, r 块/奇偶, t) 排序，三指针（l, r, 时间）
 *       跳动；增删 O(1) 时总移动量按排序最优分块达到均摊下界
 * 注意：add/del 需支持任意顺序调用（本示例 cnt 判空安全）；
 *       值域超 N 需先离散化；奇偶块优化可省一半 r 移动
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   vector<int> val{0, 1, 2, 1, 3, 2};  // n = 5: {1,2,1,3,2}
 *   auto res = mo_distinct(5, val, {{1, 3}, {2, 5}, {4, 4}});
 *   for (int x : res) cout << x << ' ';  // 2 3 1（{1,2} {2,1,3} {3}）
 *   cout << '\n';
 *   auto res2 = mo_distinct_modify(5, val, {{1, 5}}, {{2, 5}});  // a[2] = 5 → {1,5,1,3,2}
 *   for (int x : res2) cout << x << ' ';  // 4
 *   cout << '\n';
 * }
 */
