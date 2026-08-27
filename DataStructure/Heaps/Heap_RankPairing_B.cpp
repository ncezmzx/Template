#include <bits/stdc++.h>
using namespace std;
#define int long long
constexpr int N = 1e6 + 9;
int a[N], fa[N], vl[N], tp[N], rk[N];
list<int> nd[N], sn[N];
list<int>::iterator it[N];
void add(list<int>& x, int y) { it[y] = x.insert(x.end(), y); }
void cmin(int& x, int y) { x > y && (x = y); }
int newnode(int x, int i) { return add(nd[i], i), tp[i] = vl[i] = x, i; }
int top(int x) { return tp[x]; }
void join(int x, int y) {
  cmin(tp[x], tp[y]), nd[x].splice(nd[x].end(), nd[y]);
}
int getls(int x) { return sn[x].empty() ? 0 : sn[x].front(); }
int getrs(int x) {
  int f = fa[x];
  if (!f || it[x] == prev(sn[f].end())) return 0;
  return *next(it[x]);
}
int getrk(int x) { return x ? rk[x] : -1; }
int getfa(int x) {
  int f = fa[x];
  if (!f || it[x] == sn[f].begin()) return f;
  return *prev(it[x]);
}
void decrease_key(int h, int p, int v) {
  cmin(tp[h], vl[p] = v);
  int f = getfa(p);
  if (!f || vl[f] <= v) return;
  sn[f].erase(it[p]), add(nd[h], p), fa[p] = 0, rk[p] = getrk(getls(p)) + 1;
  while (f) {
    int rkf = getrk(f), rkl = getrk(getls(f)), rkr = getrk(getrs(f));
    if (rkf == rkl || (rkf == rkl + 1 && rkr <= rkl + 1)) break;
    rk[f] = rkr <= rkl + 1 ? rkl + 1 : rkl, f = getfa(f);
  }
}
void erase(int h, int x) {
  decrease_key(h, x, LLONG_MIN);
  for (int y : sn[x]) add(nd[h], y), fa[y] = 0;
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
 * 名称：赋级配对堆乙类（Rank-Pairing Heap，多叉树写法，可并堆）
 * 复杂度：newnode/top/join O(1) 均摊；decrease_key/erase O(log n) 均摊
 * 用途：newnode/top/join/decrease_key/erase；小根堆
 * 来源：洛谷文章《对优先队列的爱》(luogu_blog_1_对优先队列的爱.md) 第 6 节（乙类 + 多叉树写法），代码原样保留
 * 注意：子表 sn[] 用 std::list 存，getls/getrs/getfa 借助 it[] 迭代器与 list 顺序实现左/右兄弟；已补 #define int long long，tp 用 LLONG_MAX 哨兵语义正确；已知问题：erase 重建（nd[h].clear() 后重插）在部分操作序列下偶发错误/崩溃（疑似迭代器失效），使用时多测验证；未处理合并空堆/自合并；全局数组 N 按需调整
 * ============================================================
 */
