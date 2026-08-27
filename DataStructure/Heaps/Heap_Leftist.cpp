#include <bits/stdc++.h>
using namespace std;
constexpr int N = 1e6 + 9;
int fa[N], vl[N], d[N], ls[N], rs[N];
int newnode(int x, int i) { return vl[i] = x, i; }
int top(int x) { return vl[x]; }
void join(int& x, int y) {
  if (!x || !y) return x |= y, void();
  if (vl[x] > vl[y]) swap(x, y);
  join(rs[x], y), fa[rs[x]] = x;
  if (d[ls[x]] > d[rs[x]]) swap(ls[x], rs[x]);
  d[x] = d[rs[x]] + 1;
}
void erase(int& h, int x) {
  int f = fa[x], l = ls[x], r = rs[x];
  int& s = f ? ls[f] == x ? ls[f] : rs[f] : h;
  join(s = l, r), fa[s] = f;
  while (f) {
    if (d[ls[f]] > d[rs[f]]) swap(ls[f], rs[f]);
    int nd = d[rs[f]] + 1;
    if (d[f] == nd) break;
    d[f] = nd, f = fa[f];
  }
}
void decrease_key(int& h, int p, int v) {
  erase(h, p), vl[p] = v, ls[p] = rs[p] = fa[p] = d[p] = 0, join(h, p);
}
/*
 * ============================================================
 * 名称：左偏树（Leftist Heap，可并堆，按 d 值合并）
 * 复杂度：join/top O(1)，merge/erase/decrease_key O(log n) 确定（最坏）
 * 用途：newnode/top/join/decrease_key/erase；小根堆
 * 来源：洛谷文章《对优先队列的爱》(luogu_blog_1_对优先队列的爱.md) 第 2 节，代码原样保留
 * 注意：全局数组 fa/vl/d/ls/rs，大小 N 按需调整；h 为堆顶节点引用，空堆用 0 表示（join 用 x|=y 处理空堆）；未处理 x==y 自合并；erase 后沿 fa 上溯维护 d
 * ============================================================
 */
