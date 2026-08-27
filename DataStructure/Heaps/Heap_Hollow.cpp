#include <bits/stdc++.h>
using namespace std;
#define int long long
constexpr int N = 2e6 + 9;
struct {
  int vl, sn, bt, af, rk, ps;
} nd[N];
int pos[N], a[N], tot;
int newnode(int w, int p) {
  return nd[++tot] = {w, 0, 0, 0, 0, p}, pos[p] = tot;
}
void join(int& u, int v) {
  if (nd[u].vl > nd[v].vl) swap(u, v);
  nd[v].bt = exchange(nd[u].sn, v);
}
void merge(int& u, int v) {
  if (!u)
    u = v;
  else if (v)
    join(u, v);
}
int top(int u) { return nd[u].vl; }
void decrease_key(int& u, int x, int v) {
  int y = pos[x];
  if (u == y) return nd[u].vl = v, void();
  int p = newnode(v, x);
  nd[y].ps = 0, nd[p].rk = max(nd[y].rk - 2, 0ll);
  return nd[p].sn = y, nd[y].af = p, join(u, p);
}
void erase(int& u, int x) {
  int y = exchange(pos[x], 0), mx = 0;
  if (nd[y].ps = 0, nd[u].ps) return;
  nd[u].bt = 0;
  while (u) {
    int w = nd[u].sn, v = exchange(u, nd[u].bt);
    while (w) {
      int z = exchange(w, nd[w].bt);
      if (!nd[z].ps) {
        if (int& f = nd[z].af)
          (exchange(f, 0) == v ? w : nd[z].bt) = 0;
        else
          nd[z].bt = exchange(u, z);
      } else {
        int t = nd[z].rk;
        while (int& s = a[t]) join(z, exchange(s, 0)), nd[z].rk = ++t;
        a[t] = z, mx = max(mx, t);
      }
    }
  }
  for (int i = 0; i <= mx; ++i) merge(u, a[i]);
  memset(a, 0, (mx + 1) * sizeof(int));
}
/*
 * ============================================================
 * 名称：hollow heap（奉先堆，可并堆）
 * 复杂度：newnode/top/join O(1) 均摊；decrease_key O(1) 均摊（期望/均摊）；erase O(log n) 均摊
 * 用途：newnode/top/join/decrease_key/erase；小根堆（节点存于结构体数组 nd[]，pos[] 做 id->节点 映射）
 * 来源：洛谷文章《对优先队列的爱》(luogu_blog_1_对优先队列的爱.md) 第 8 节，代码原样保留
 * 注意：节点池 nd[N]（N=2e6），tot 递增分配不回收（newnode 每次新建节点）；ps 标记 hollow（被减键失效的旧节点）；vl 为 int 但 rk 用 0ll 比较（int 截断无碍）；decrease_key 若 u==y 直接改根值；全局数组大小按需调整
 * ============================================================
 */
