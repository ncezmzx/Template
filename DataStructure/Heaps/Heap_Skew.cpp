#include <bits/stdc++.h>
using namespace std;
#define int long long
constexpr int N = 1e6 + 9;
int fa[N], vl[N], ls[N], rs[N];
int newnode(int x, int i) { return vl[i] = x, i; }
int top(int x) { return vl[x]; }
void join(int& x, int y) {
  if (!x || !y) return x |= y, void();
  if (vl[x] > vl[y]) swap(x, y);
  join(rs[x], y), fa[rs[x]] = x, swap(ls[x], rs[x]);
}
void decrease_key(int& h, int p, int v) {
  int f = fa[p], &s = f ? ls[f] == p ? ls[f] : rs[f] : h;
  s = fa[p] = 0, vl[p] = v, join(h, p);
}
void erase(int& h, int x) {
  decrease_key(h, x, LLONG_MIN);
  int l = ls[h], r = rs[h];
  join(h = l, r), fa[h] = 0;
}
/*
 * ============================================================
 * 名称：斜堆（Skew Heap，可并堆，合并时交换左右子树）
 * 复杂度：newnode/top O(1)；merge/erase/decrease_key O(log n) 均摊
 * 用途：newnode/top/join/decrease_key/erase；小根堆
 * 来源：洛谷文章《对优先队列的爱》(luogu_blog_1_对优先队列的爱.md) 第 3 节，代码原样保留
 * 注意：已补 #define int long long，erase 用 LLONG_MIN 哨兵语义正确；h 为堆顶引用，空堆为 0；未处理 x==y 自合并；未声明 d 数组（无 rank 信息）
 * ============================================================
 */
