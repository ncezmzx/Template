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
void join(int x, list<int>& ndy) {
  int mx = -1;
  for (int h : nd[x]) mx = max(mx, rk[h]), a[rk[h]] = h;
  for (int h : ndy) {
    while (int y = a[rk[h]]) {
      if (vl[h] > vl[y]) swap(h, y);
      add(sn[fa[y] = h], y), a[rk[h]++] = 0;
    }
    mx = max(mx, rk[h]), a[rk[h]] = h;
  }
  nd[x].clear(), tp[x] = LLONG_MAX;
  for (int i = 0; i <= mx; ++i)
    if (int& h = a[i]) add(nd[x], h), cmin(tp[x], vl[h]), h = 0;
}
void join(int x, int y) { join(x, nd[y]); }
void decrease_key(int h, int p, int v) {
  cmin(tp[h], vl[p = pos[p]] = v);
  for (int f = fa[p]; f && vl[f] > v; f = fa[p = f])
    swap(msk[p], msk[f]), swap(pos[msk[p]], pos[msk[f]]), swap(vl[p], vl[f]);
}
void erase(int h, int x) {
  decrease_key(h, x, LLONG_MIN);
  for (int y : sn[x = pos[x]]) fa[y] = 0;
  nd[h].erase(it[x]), join(h, sn[x]);
}
/*
 * ============================================================
 * 名称：二项堆（Binomial Heap，无均摊常数的插入特化，可并堆）
 * 复杂度：newnode/top O(1)；join/erase/decrease_key O(log n) 最坏（join 无均摊常数）
 * 用途：newnode/top/join/decrease_key/erase；小根堆
 * 来源：洛谷文章《对优先队列的爱》(luogu_blog_1_对优先队列的爱.md) 第 4 节，代码原样保留
 * 注意：tp 用 LLONG_MAX 作空堆哨兵（已补 #define int long long，语义正确）；join(x, y) 后 y 的根表被清空；erase 用 decrease_key 降到 LLONG_MIN 再拆子树；未处理合并空堆/自合并的边界情况；全局数组 N 按需调整
 * ============================================================
 */
