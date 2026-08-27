#include <bits/stdc++.h>
using namespace std;
#define int long long
constexpr int N = 1e6 + 9, M = N << 1;
int ivl[M], bel[M], vl[M];
vector<vector<int>> buf;
void cmin(int& x, int y) { x > y && (x = y); }
struct eraseable_heap {
  priority_queue<int> pq, dl;
  void push(int x, int i) { pq.push(ivl[i] = x); }
  int top() {
    while (!dl.empty() && !pq.empty() && pq.top() == dl.top()) pq.pop(), dl.pop();
    return pq.empty() ? LLONG_MAX : pq.top();
  }
  void erase(int x) {
    if (dl.push(ivl[x]), (dl.size() << 1) >= pq.size()) {
      vector<int> ret;
      while (true) {
        int t = top();
        if (t == LLONG_MAX) break;
        ret.push_back(t), pq.pop();
      }
      for (int x : ret) pq.push(x);
    }
  }
};
struct fastpush_heap {
  eraseable_heap pq;
  vector<int> nbuf;
  int sz, tp;
  fastpush_heap() : sz(0), tp(LLONG_MAX) {}
  static int qmin(vector<int>& v) {
    int x = LLONG_MAX;
    for (int y : v) cmin(x, vl[y]);
    return x;
  }
  static void erase(vector<int>& v, int t) {
    v.erase(find(v.begin(), v.end(), t));
  }
  void push(int x, int i) {
    cmin(tp, vl[i] = x), bel[i] = -1;
    nbuf.push_back(i);
    if ((1 << ((int)nbuf.size() >> 1)) > ++sz) {
      int t = buf.size();
      for (int x : nbuf) bel[x] = t;
      pq.push(qmin(nbuf), t);
      buf.push_back(nbuf), nbuf.clear();
    }
  }
  int top() const { return tp; }
  void modify(int x, int v) {
    int t = bel[x];
    vl[x] = v;
    if (t == -1) {
      if (v == LLONG_MAX) erase(nbuf, x);
    } else {
      pq.erase(t);
      if (v == LLONG_MAX) erase(buf[t], x);
      if (!buf[t].empty()) pq.push(qmin(buf[t]), t);
    }
    tp = min(pq.top(), qmin(nbuf)), sz -= v == LLONG_MAX;
  }
  int size() const { return sz; }
};
fastpush_heap pq[N];
int fa[N], rk[N];
int newnode(int x, int i) { return pq[i].push(x, i), i; }
int top(int x) { return pq[x].top(); }
void join(int& x, int y) {
  if (rk[x] < rk[y]) swap(x, y);
  if (rk[x] == rk[y]) ++rk[x];
  pq[x].push(pq[y].top(), y + N), fa[y] = x;
}
void decrease_key(int h, int p, int v) {
  for (int x = p; int& f = fa[x]; x = exchange(f, h)) {
    if (f == h) {
      pq[f].modify(x + N, pq[x].top());
      break;
    }
    pq[f].modify(x + N, LLONG_MAX);
    if (pq[x].size()) pq[h].push(pq[x].top(), x + N);
  }
  if (pq[p].modify(p, v), p != h) pq[h].modify(p + N, pq[p].top());
}
void erase(int h, int x) { decrease_key(h, x, LLONG_MAX); }
/*
 * ============================================================
 * 名称：STL 二叉堆扩展（binary(STL)(extended)，可并堆，基于 priority_queue 惰性删除）
 * 复杂度：push/top O(1) 均摊（top 惰性弹 dl 后 O(1)）；modify/erase O(log n) 均摊；size O(1)
 * 用途：结构体含 push/top/modify/erase/size（heap-array 用法：fastpush_heap pq[N]，节点 i 的键存 vl[i]）；newnode/top/join/decrease_key/erase；小根堆
 * 来源：洛谷文章《对优先队列的爱》(luogu_blog_1_对优先队列的爱.md) 第 9 节，代码原样保留
 * 注意：decrease_key/erase 用 LLONG_MAX 惰性删除；join 把 y 的堆顶作为哨兵节点 y+N 插入 x 的 pq（编号需 < M）；内部有 eraseable_heap（pq+dl 双堆）与 fastpush_heap（分块 nbuf/buf）两层；已补 #define int long long 使键与哨兵语义一致（原文用 int 存 LLONG_MAX 会截断）；全局数组 M 按需调整；已知问题：decrease_key 的 fa 链上提在部分操作序列下会丢失堆内最小值贡献（top 偏大），需自行排查
 * ============================================================
 */
