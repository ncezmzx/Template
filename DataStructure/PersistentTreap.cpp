#include <bits/stdc++.h>
using namespace std;

// 可持久化平衡树（fhq-Treap 全持久化）：插入/删除返回新根，历史版本随时可查
constexpr int N = 4e6 + 9;  // ≈ (n + m) · 2 log n
int lc[N], rc[N], sz[N], pri[N], tot;
long long val[N];
unsigned long long sd = 88172645463325252ull;  // 固定随机种子（可换 chrono 抗卡）
unsigned long long rng() {
  unsigned long long x = (sd += 0x9e3779b97f4a7c15ull);
  x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ull;
  x = (x ^ (x >> 27)) * 0x94d049bb133111ebull;
  return x ^ (x >> 31);
}
int newnode(long long x) {
  int u = ++tot;
  lc[u] = rc[u] = 0, sz[u] = 1, val[u] = x, pri[u] = (int)(rng() & 0x3fffffff);
  return u;
}
inline void upd(int u) { sz[u] = sz[lc[u]] + sz[rc[u]] + 1; }
// 按值分裂：≤ k 归 a，> k 归 b（沿途克隆，不改动原树）
void split(int u, long long k, int& a, int& b) {
  if (!u) {
    a = b = 0;
    return;
  }
  int v = ++tot;
  lc[v] = lc[u], rc[v] = rc[u], val[v] = val[u], pri[v] = pri[u], sz[v] = sz[u];
  if (val[v] <= k) split(rc[v], k, rc[v], b), a = v;
  else split(lc[v], k, a, lc[v]), b = v;
  upd(v);
}
int merge(int a, int b) {
  if (!a || !b) return a | b;
  int u = ++tot;
  if (pri[a] < pri[b]) {
    lc[u] = lc[a], rc[u] = rc[a], val[u] = val[a], pri[u] = pri[a], sz[u] = sz[a];
    rc[u] = merge(rc[a], b);
  } else {
    lc[u] = lc[b], rc[u] = rc[b], val[u] = val[b], pri[u] = pri[b], sz[u] = sz[b];
    lc[u] = merge(a, lc[b]);
  }
  upd(u);
  return u;
}
int insert(int rt, long long x) {  // 插一个 x，返回新根（多重集）
  int a, b;
  split(rt, x, a, b);
  return merge(merge(a, newnode(x)), b);
}
int erase(int rt, long long x) {  // 删一个 x（若无则树不变），返回新根
  int a, b, c;
  split(rt, x, a, b);
  split(a, x - 1, a, c);
  c = merge(lc[c], rc[c]);  // 去掉 c 的根（恰一个 x）
  return merge(merge(a, c), b);
}
long long kth(int rt, int k) {  // 第 k 小（1-indexed），越界返回 LLONG_MIN
  int u = rt;
  while (u) {
    if (k <= sz[lc[u]]) u = lc[u];
    else if (k == sz[lc[u]] + 1) return val[u];
    else k -= sz[lc[u]] + 1, u = rc[u];
  }
  return LLONG_MIN;
}
int rnk(int rt, long long x) {  // < x 的元素个数（不分配节点）
  int u = rt, res = 0;
  while (u) {
    if (val[u] < x) res += sz[lc[u]] + 1, u = rc[u];
    else u = lc[u];
  }
  return res;
}
long long pre(int rt, long long x) {  // 严格前驱，无返回 LLONG_MIN
  int u = rt;
  long long res = LLONG_MIN;
  while (u) {
    if (val[u] < x) res = val[u], u = rc[u];
    else u = lc[u];
  }
  return res;
}
long long nxt(int rt, long long x) {  // 严格后继，无返回 LLONG_MAX
  int u = rt;
  long long res = LLONG_MAX;
  while (u) {
    if (val[u] > x) res = val[u], u = lc[u];
    else u = rc[u];
  }
  return res;
}

/*
 * ============================================================
 * 名称：可持久化平衡树（fhq-Treap 全持久化）
 * 复杂度：插入/删除 O(log n)（沿途克隆）；kth/rank/前驱/后继 O(log n)
 * 用途：需要同时保留多个历史版本的有序多重集（经典：洛谷 P3835
 *       可持久化平衡树），也支持"回退到版本 k"
 * 接口：insert(rt, x) / erase(rt, x) → 新根；kth(rt, k)、rnk(rt, x)、
 *       pre(rt, x)、nxt(rt, x) 为纯查询（不分配节点）
 * 原理：split/merge 全程"写时复制"——每访问一个节点先克隆，历史根
 *       指向的结构永不被修改；随机优先级保平衡
 * 注意：节点值域 1..tot；N ≈ (n+m)·2log n（1e5 次操作约 4e6）；删除
 *       仅去一个同值节点；kth 越界返回 LLONG_MIN
 * 来源：OI-Wiki《可持久化平衡树》（https://oi-wiki.org/ds/persistent-balanced/）
 * ============================================================
 * 使用示例（编译时取消注释；维护版本并查询第 k 小）：
 * signed main() {
 *   vector<int> ver{0};  // ver[i] = 第 i 个版本（0 为空树）
 *   ver.push_back(insert(ver[0], 2));
 *   ver.push_back(insert(ver[1], 1));
 *   ver.push_back(insert(ver[2], 2));  // 多重集 {1,2,2}
 *   cout << kth(ver[3], 2) << '\n';    // 2
 *   cout << rnk(ver[3], 2) << '\n';    // 1（<2 的个数）
 *   cout << kth(ver[1], 1) << '\n';    // 2（历史版本仍可用）
 *   cout << nxt(ver[3], 1) << '\n';    // 2
 * }
 * ============================================================
 */
