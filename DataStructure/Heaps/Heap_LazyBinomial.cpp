#include <bits/stdc++.h>
using namespace std;
#define int long long
constexpr int N = 1e6 + 9;
int a[N], fa[N], vl[N], tp[N], rk[N], pos[N], msk[N];
list<int> nd[N], sn[N];
list<int>::iterator it[N];
void add(list<int>& x, int y) { it[y] = x.insert(x.end(), y); }
void cmin(int& x, int y) { x > y && (x = y); }
int newnode(int x, int i) {
  return add(nd[i], i), tp[i] = vl[i] = x, pos[i] = msk[i] = i;
}
int top(int x) { return tp[x]; }
void join(int x, int y) {
  cmin(tp[x], tp[y]), nd[x].splice(nd[x].end(), nd[y]);
}
void decrease_key(int h, int p, int v) {
  cmin(tp[h], vl[p = pos[p]] = v);
  for (int f = fa[p]; f && vl[f] > v; f = fa[p = f])
    swap(msk[p], msk[f]), swap(pos[msk[p]], pos[msk[f]]), swap(vl[p], vl[f]);
}
void erase(int h, int x) {
  decrease_key(h, x, LLONG_MIN);
  for (int y : sn[x = pos[x]]) add(nd[h], y), fa[y] = 0;
  nd[h].erase(it[x]), sn[x].clear();
  int mx = -1;
  for (int x : nd[h]) {
    while (int y = a[rk[x]]) {
      if (vl[x] > vl[y]) swap(x, y);
      add(sn[fa[y] = x], y), a[rk[x]++] = 0;
    }
    mx = max(mx, rk[x]), a[rk[x]] = x;
  }
  nd[h].clear(), tp[h] = LLONG_MAX;
  for (int i = 0; i <= mx; ++i)
    if (int& x = a[i]) add(nd[h], x), cmin(tp[h], vl[x]), x = 0;
}
/*
 * ============================================================
 * 名称：懒二项堆（Lazy Binomial Heap，可并堆）
 * 复杂度：newnode/top/join O(1) 均摊；decrease_key/erase O(log n) 均摊
 * 用途：newnode/top/join/decrease_key/erase；小根堆
 * 来源：洛谷文章《对优先队列的爱》(luogu_blog_1_对优先队列的爱.md) 第 11 节，代码原样保留
 * 注意：join 只合并根表（惰性，不立刻维护二项树）；decrease_key 沿 fa 上溯冒泡交换 msk/pos/vl；erase 用 LLONG_MIN 哨兵再整桶重排；已补 #define int long long，tp 用 LLONG_MAX 哨兵语义正确；未处理合并空堆/自合并；全局数组 N 按需调整
 * ============================================================
 */
