#include <bits/stdc++.h>
using namespace std;
constexpr int N = 1e6 + 9;
int fa[N], vl[N], sz[N], ls[N], rs[N];
void cmin(int& x, int y) { x > y && (x = y); }
int newnode(int x, int i) { return vl[i] = x, sz[i] = 1, i; }
int top(int x) { return vl[x]; }
void join(int& x, int y) {
  if (!x || !y) return x |= y, void();
  if (vl[x] > vl[y]) swap(x, y);
  int& s = sz[ls[x]] > sz[rs[x]] ? rs[x] : ls[x];
  sz[x] += sz[y], join(s, y), fa[s] = x;
}
void erase(int& h, int x) {
  int f = fa[x], l = ls[x], r = rs[x];
  int& s = f ? ls[f] == x ? ls[f] : rs[f] : h;
  join(s = l, r), fa[s] = f;
}
void decrease_key(int& h, int p, int v) {
  erase(h, p), vl[p] = v, ls[p] = rs[p] = fa[p] = 0, sz[p] = 1, join(h, p);
}
/*
 * ============================================================
 * 名称：重左偏（Weighted Leftist Heap，按子树大小 sz 合并，可并堆）
 * 复杂度：newnode/top O(1)；merge/erase/decrease_key O(log n) 确定（按 sz 保证对数深度）
 * 用途：newnode/top/join/decrease_key/erase；小根堆
 * 来源：洛谷文章《对优先队列的爱》(luogu_blog_1_对优先队列的爱.md) 第 15 节，代码原样保留
 * 注意：与普通左偏树不同，按子树大小 sz 选合并侧（无 d 数组）；join 用 x|=y 处理空堆；未处理 x==y 自合并；erase/decrease_key 用引用参数 h 维护堆顶；全局数组 N 按需调整
 * ============================================================
 */
