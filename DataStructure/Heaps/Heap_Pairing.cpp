#include <bits/stdc++.h>
using namespace std;
#define int long long
constexpr int N = 1e6 + 9;
int vl[N], sn[N], bt[N], fa[N];
int newnode(int x, int i) { return vl[i] = x, i; }
int top(int x) { return vl[x]; }
void join(int& x, int y) {
  if (!x || !y) return x |= y, void();
  if (vl[x] > vl[y]) swap(x, y);
  if ((bt[y] = exchange(sn[fa[y] = x], y))) fa[bt[y]] = y;
}
void decrease_key(int& h, int p, int v) {
  int f = fa[p], &s = f ? sn[f] == p ? sn[f] : bt[f] : h, t = bt[p];
  t && (fa[t] = f, bt[p] = 0), s = t, fa[p] = 0, vl[p] = v, join(h, p);
}
int mgsn(int x) {
  if (!x) return 0;
  int y = bt[x], z = mgsn(bt[y]);
  return join(x, y), join(z, x), z;
}
void erase(int& h, int x) {
  decrease_key(h, x, LLONG_MIN);
  h = mgsn(sn[h]), fa[h] = bt[h] = 0;
}
/*
 * ============================================================
 * 名称：配对堆（Pairing Heap，可并堆，mgsn 两两合并删除）
 * 复杂度：newnode/top/join O(1) 均摊；decrease_key/erase O(log n) 均摊
 * 用途：newnode/top/join/decrease_key/erase；小根堆
 * 来源：洛谷文章《对优先队列的爱》(luogu_blog_1_对优先队列的爱.md) 第 5 节，代码原样保留
 * 注意：该节代码未声明全局数组 vl/sn/bt/fa 与 N，已按惯例补全
 *       （constexpr int N = 1e6+9; int vl[N], sn[N], bt[N], fa[N];）；
 *       erase 用 LLONG_MIN 哨兵；h 为堆顶引用；未处理空堆/自合并边界
 * ============================================================
 */
