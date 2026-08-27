#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int N = 1e5 + 9;
int ch[N][2], sz[N], val[N], pr[N], tg[N], tot, rt;
mt19937 rng(random_device{}());

int node(int v) {
  int x = ++tot;
  val[x] = v, sz[x] = 1, pr[x] = rng();
  return x;
}

void up(int x) { sz[x] = 1 + sz[ch[x][0]] + sz[ch[x][1]]; }

void apply(int x) { swap(ch[x][0], ch[x][1]), tg[x] ^= 1; }

void down(int x) {
  if (tg[x]) apply(ch[x][0]), apply(ch[x][1]), tg[x] = 0;
}

void split(int x, int k, int& a, int& b) {
  if (!x) return a = b = 0, void();
  down(x);
  if (sz[ch[x][0]] >= k) split(ch[x][0], k, a, ch[x][0]), b = x, up(x);
  else split(ch[x][1], k - sz[ch[x][0]] - 1, ch[x][1], b), a = x, up(x);
}

void split_v(int x, int v, int& a, int& b) {
  if (!x) return a = b = 0, void();
  down(x);
  if (val[x] < v) split_v(ch[x][1], v, ch[x][1], b), a = x, up(x);
  else split_v(ch[x][0], v, a, ch[x][0]), b = x, up(x);
}

int merge(int a, int b) {
  if (!a || !b) return a + b;
  if (pr[a] < pr[b]) return down(a), ch[a][1] = merge(ch[a][1], b), up(a), a;
  return down(b), ch[b][0] = merge(a, ch[b][0]), up(b), b;
}

void insert(int& x, int v) {
  int a, b;
  split_v(x, v, a, b);
  x = merge(merge(a, node(v)), b);
}

void erase(int& x, int v) {
  int a, b, c;
  split_v(x, v, a, b);
  split_v(b, v + 1, c, b);
  x = merge(a, b);
}

int kth(int x, int k) {
  while (true) {
    down(x);
    if (sz[ch[x][0]] >= k) x = ch[x][0];
    else if (k == sz[ch[x][0]] + 1) return val[x];
    else k -= sz[ch[x][0]] + 1, x = ch[x][1];
  }
}

void dfs(int x) {
  if (!x) return;
  down(x);
  dfs(ch[x][0]), cout << val[x] << ' ', dfs(ch[x][1]);
}

/*
 * ============================================================
 * 名称：无旋 Treap（fhq-Treap，split/merge）
 * 复杂度：单次操作均摊 O(log n)
 * 用途：既可按值（有序集合）也可按大小（序列）分裂：
 *       - 集合版：split_v(x, v, a, b) 把树拆成 < v 与 >= v 两棵，配合 merge 实现
 *         insert/erase/kth/前驱后继；本文件已给出按值 insert/erase/kth
 *       - 序列版：split(x, k, a, b) 按大小拆前 k 个；apply 打反转懒标记即可
 *         做区间翻转（文艺平衡树），见注释示例
 * 原理：每个节点带随机优先级 pr，merge 按堆性质合并保证期望平衡；
 *       split/merge 是唯一基本操作，天然支持可持久化（split/merge 时复制节点）
 * 注意：与 BalancedTree_Splay.cpp、BalancedTree_WBST_*.cpp 为同算法（平衡树）
 *       的不同实现，按需选用；多组数据需重置 tot、rt
 * 用法：rt = merge(a, b)；insert(rt, v)；erase(rt, v)；kth(rt, k)
 *       （erase 删除**所有**等于 v 的节点，set 语义；multiset 需自行 split 摘一个）
 * ============================================================
 * 使用示例（编译时取消注释；序列区间反转）：
 * signed main() {
 *   int n, m;
 *   cin >> n >> m;
 *   for (int i = 1; i <= n; ++i) rt = merge(rt, node(i));
 *   while (m--) {
 *     int l, r, a, b, c;
 *     cin >> l >> r;
 *     split(rt, l - 1, a, b), split(b, r - l + 1, b, c);
 *     apply(b);
 *     rt = merge(merge(a, b), c);
 *   }
 *   dfs(rt);
 * }
 * ============================================================
 */
