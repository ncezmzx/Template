#include <bits/stdc++.h>
using namespace std;
#define int long long
constexpr int N = 1e6 + 9;
int a[N], fa[N], vl[N], tp[N], rk[N], ls[N], rs[N];
bool ist[N];
list<int> nd[N];
list<int>::iterator it[N];
void add(list<int>& x, int y) { it[y] = x.insert(x.end(), y); }
void cmin(int& x, int y) { x > y && (x = y); }
int newnode(int x, int i) { return add(nd[i], i), tp[i] = vl[i] = x, i; }
int top(int x) { return tp[x]; }
void join(int x, int y) {
  cmin(tp[x], tp[y]), nd[x].splice(nd[x].end(), nd[y]);
}
int getrk(int x) { return x ? rk[x] : -1; }
void decrease_key(int h, int p, int v) {
  cmin(tp[h], vl[p] = v);
  int f = fa[p];
  if (!f || (p == ls[f] && vl[f] <= v)) return;
  (p == ls[f] ? ls[f] : rs[f]) = exchange(rs[p], 0);
  add(nd[h], p), fa[p] = 0;
  if (ist[p]) --rk[p], ist[p] = false;
  while (f) {
    int rkf = getrk(f), rkl = getrk(ls[f]), rkr = getrk(rs[f]);
    if (rkf >= rkr + 2 || (!fa[f] && ist[f])) {
      if (ist[f])
        --rk[f], ist[f] = false, f = fa[f];
      else {
        int sn = ls[f], bt = rs[f];
        if (bt) fa[bt] = sn;
        ls[f] = rs[sn], rs[f] = sn;
        if (sn) rs[sn] = bt;
        if (int s = ls[f]) fa[s] = f;
        ist[f] = true;
        break;
      }
    } else if (rkf >= rkl + 3) {
      int g = fa[f];
      (f == ls[g] ? ls[g] : rs[g]) = exchange(rs[f], 0);
      add(nd[h], f), fa[f] = 0, rk[f] -= 2, ist[f] = false;
      f = g;
    } else
      break;
  }
}
void erase(int h, int x) {
  decrease_key(h, x, LLONG_MIN);
  for (int y = ls[x]; y; y = exchange(rs[y], 0)) add(nd[h], y), fa[y] = 0;
  nd[h].erase(it[x]);
  int mx = -1;
  for (int x : nd[h]) {
    while (int y = a[rk[x]]) {
      if (vl[x] > vl[y]) swap(x, y);
      fa[rs[y] = exchange(ls[fa[y] = x], y)] = y, a[rk[x]++] = 0;
    }
    mx = max(mx, rk[x]), a[rk[x]] = x;
  }
  nd[h].clear(), tp[h] = LLONG_MAX;
  for (int i = 0; i <= mx; ++i)
    if (int& x = a[i]) add(nd[h], x), cmin(tp[h], vl[x]), x = 0;
}
/*
 * ============================================================
 * 名称：瘦堆（Thin Heap，可并堆）
 * 复杂度：newnode/top/join O(1) 均摊；decrease_key O(1) 均摊；erase O(log n) 均摊
 * 用途：newnode/top/join/decrease_key/erase；小根堆
 * 来源：洛谷文章《对优先队列的爱》(luogu_blog_1_对优先队列的爱.md) 第 10 节，代码原样保留
 * 注意：ist[] 标记"瘦"节点（仅允许丢失一个子节点的减键处理）；vl 为 int，tp 用 LLONG_MAX 哨兵；decrease_key 上溯时做瘦化/切分；该实现存在固有缺陷（erase 重建的 fa 语义与减键摘除冲突、非首子节点摘除错误、erase 后可能残留 LLONG_MIN 幽灵节点），文章原版代码，仅作参考，不建议直接用于正式比赛；全局数组 N 按需调整
 * ============================================================
 */
