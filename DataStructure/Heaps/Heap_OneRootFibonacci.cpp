#include <bits/stdc++.h>
using namespace std;
#define int long long
constexpr int N = 1e6 + 9;
int a[N], fa[N], vl[N], frm[N], rk[N], sn[N], bt[N];
bool mk[N];
int newnode(int x, int i) { return vl[i] = x, i; }
int top(int x) { return vl[x]; }
void join(int& x, int y) {
  if (vl[x] > vl[y]) swap(x, y);
  if (int z = bt[y] = exchange(sn[frm[y] = fa[y] = x], y)) frm[z] = y;
}
void decrease_key(int& h, int p, int v) {
  int f = fa[p];
  if (vl[p] = v, !f || vl[f] <= v) return;
  int g = frm[p], z = bt[p];
  if (((sn[g] == p ? sn[g] : bt[g]) = z)) frm[z] = g;
  do rk[f] && --rk[f];
  while (!(mk[f] ^= true) && (f = fa[f]));
  frm[p] = fa[p] = bt[p] = 0, mk[p] = false, join(h, p);
}
void erase(int& h, int x) {
  decrease_key(h, x, LLONG_MIN);
  int mx = -1, y = sn[x];
  while (y) {
    int x = exchange(y, exchange(bt[y], 0));
    fa[x] = frm[x] = 0;
    while (int y = a[rk[x]]) join(x, y), a[rk[x]++] = 0;
    mx = max(mx, rk[x]), a[rk[x]] = x;
  }
  h = 0;
  for (int i = 0; i <= mx; ++i)
    if (int x = exchange(a[i], 0)) {
      if (!h)
        h = x;
      else
        join(h, x);
    }
}
/*
 * ============================================================
 * 名称：单根斐波那契堆（Fibonacci Heap one-root 写法，可并堆）
 * 复杂度：newnode/top/join O(1) 均摊；decrease_key O(1) 均摊；erase O(log n) 均摊
 * 用途：newnode/top/join/decrease_key/erase；小根堆
 * 来源：洛谷文章《对优先队列的爱》(luogu_blog_1_对优先队列的爱.md) 第 12 节，代码原样保留
 * 注意：堆永远只有一个根 h（join 直接把 y 挂到 x 的子表）；frm[] 记录"前驱兄弟"（list 用 bt 链），mk[] 为丢失标记；erase 先减到 LLONG_MIN 再拆出全部子树按 rk 桶合并重建根；h 为根节点引用（空堆为 0，但 join 未处理 x/y 为 0）；全局数组 N 按需调整
 * ============================================================
 */
