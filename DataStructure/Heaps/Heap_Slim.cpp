#include <bits/stdc++.h>
using namespace std;
#define int long long
constexpr int N = 1e6 + 9;
int fa[N], vl[N], tp[N], sn[N], bt[N];
list<int> nd[N];
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
  if (!f) return;
  (p == sn[f] ? sn[f] : bt[f]) = exchange(bt[p], 0);
  add(nd[h], p), fa[p] = bt[p] = 0;
}
int merge(int x, int y) {
  return fa[bt[y] = exchange(sn[fa[y] = x], y)] = y, x;
}
void erase(int h, int x) {
  decrease_key(h, x, LLONG_MIN);
  for (int y = sn[x]; y; y = exchange(bt[y], 0)) add(nd[h], y), fa[y] = 0;
  nd[h].erase(it[x]);
  if (nd[h].empty()) return;
  auto it = nd[h].begin();
  while (true)
    if (it == prev(nd[h].end()) || vl[*it] >= vl[*next(it)])
      if (it != prev(nd[h].end()) &&
          (it == nd[h].begin() || vl[*prev(it)] < vl[*next(it)]))
        *it = merge(*next(it), *it), nd[h].erase(next(it));
      else if (it != nd[h].begin())
        *it = merge(*prev(it), *it), nd[h].erase(prev(it));
      else
        break;
    else
      ++it;
  ::it[*it] = it, tp[h] = vl[*it];
}
/*
 * ============================================================
 * 名称：纤细堆（Slim Heap，可并堆）
 * 复杂度：newnode/top/join O(1) 均摊；decrease_key O(1) 均摊；erase O(log n) 均摊（两两 merge 收敛根数）
 * 用途：newnode/top/join/decrease_key/erase；小根堆
 * 来源：洛谷文章《对优先队列的爱》(luogu_blog_1_对优先队列的爱.md) 第 14 节，代码原样保留
 * 注意：已补 #define int long long 使 LLONG_MIN 哨兵语义正确（原文用 int 会截断为 0）；
 *       已知缺陷（原文如此）：decrease_key 摘除非首子节点 p 时
 *       `(p == sn[f] ? sn[f] : bt[f]) = exchange(bt[p], 0)` 未更新 p 的前驱兄弟的
 *       bt（无前驱指针），会把父节点的 bt 写成 bt[p]，破坏兄弟链，被删节点可能
 *       残留在树中；仅当被减键节点恒为首子节点时才正确，使用时请留意；
 *       erase 在根表 nd[h] 上反复两两 merge 直到只剩一个根，借全局 ::it[] 更新迭代器；
 *       未处理合并空堆/自合并；全局数组 N 按需调整
 * ============================================================
 */
