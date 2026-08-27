#include <bits/stdc++.h>
using namespace std;
#define int long long
constexpr int N = 1e6 + 9;
int a[N], fa[N], vl[N], tp[N], dg[N];
bool mk[N];
list<int> nd[N], sn[N];
list<int>::iterator it[N];
void add(list<int>& x, int y) { it[y] = x.insert(x.end(), y); }
void cmin(int& x, int y) { x > y && (x = y); }
int newnode(int x, int i) { return add(nd[i], i), tp[i] = vl[i] = x, i; }
int top(int x) { return tp[x]; }
void join(int x, int y) {
  cmin(tp[x], tp[y]), nd[x].splice(nd[x].end(), nd[y]);
}
void decrease_key(int h, int p, int v) {
  cmin(tp[h], vl[p] = v);
  int f = fa[p];
  if (!f || vl[f] <= v) return;
  do sn[f].erase(it[p]), --dg[f], add(nd[h], p), fa[p] = 0;
  while ((mk[p = f] ^= true) && (f = fa[p]));
}
void erase(int h, int x) {
  decrease_key(h, x, LLONG_MIN);
  for (int y : sn[x]) add(nd[h], y), fa[y] = 0;
  nd[h].erase(it[x]), sn[x].clear();
  int mx = -1;
  for (int x : nd[h]) {
    while (int y = a[dg[x]]) {
      if (vl[x] > vl[y]) swap(x, y);
      add(sn[fa[y] = x], y), a[dg[x]++] = 0;
    }
    mx = max(mx, dg[x]), a[dg[x]] = x;
  }
  nd[h].clear(), tp[h] = LLONG_MAX;
  for (int i = 0; i <= mx; ++i)
    if (int& x = a[i]) add(nd[h], x), cmin(tp[h], vl[x]), x = 0;
}
/*
 * ============================================================
 * 名称：斐波那契堆（Fibonacci Heap，可并堆）
 * 复杂度：newnode/top/join O(1) 均摊；decrease_key O(1) 均摊；erase O(log n) 均摊
 * 用途：newnode/top/join/decrease_key/erase；小根堆
 * 来源：洛谷文章《对优先队列的爱》(luogu_blog_1_对优先队列的爱.md) 第 7 节，代码原样保留
 * 注意：mk[] 为"丢失子节点"标记（文章闲话：decrease_key 中 while 行加不加 ! 均正确，代码保留原文带 ! 的写法）；vl 为 int，tp 用 LLONG_MAX 哨兵；未处理合并空堆/自合并；全局数组 N 按需调整
 * ============================================================
 */
