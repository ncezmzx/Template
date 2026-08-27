#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int N = 1e5 + 9;
int ch[N][2], fa[N], sz[N], val[N], tg[N], rt, tot;

void up(int x) { sz[x] = 1 + sz[ch[x][0]] + sz[ch[x][1]]; }

void apply(int x) { swap(ch[x][0], ch[x][1]), tg[x] ^= 1; }

void down(int x) {
  if (tg[x]) apply(ch[x][0]), apply(ch[x][1]), tg[x] = 0;
}

void rotate(int x) {
  int y = fa[x], z = fa[y], k = ch[y][1] == x;
  if (z) ch[z][ch[z][1] == y] = x;
  fa[x] = z;
  ch[y][k] = ch[x][k ^ 1];
  if (ch[x][k ^ 1]) fa[ch[x][k ^ 1]] = y;
  ch[x][k ^ 1] = y, fa[y] = x;
  up(y), up(x);
}

void splay(int x, int goal) {
  while (fa[x] != goal) {
    int y = fa[x], z = fa[y];
    if (z != goal) rotate((ch[y][1] == x) ^ (ch[z][1] == y) ? x : y);
    rotate(x);
  }
  if (!goal) rt = x;
}

int build(int l, int r) {
  if (l > r) return 0;
  int m = (l + r) >> 1, x = ++tot;
  val[x] = m, sz[x] = 1;
  if (l < m) ch[x][0] = build(l, m - 1), fa[ch[x][0]] = x;
  if (m < r) ch[x][1] = build(m + 1, r), fa[ch[x][1]] = x;
  up(x);
  return x;
}

int kth(int k) {
  int x = rt;
  while (true) {
    down(x);
    if (sz[ch[x][0]] >= k) x = ch[x][0];
    else if (k == sz[ch[x][0]] + 1) return x;
    else k -= sz[ch[x][0]] + 1, x = ch[x][1];
  }
}

void reverse(int l, int r) {
  int L = kth(l), R = kth(r + 2);
  splay(L, 0), splay(R, L);
  apply(ch[R][0]);
}

void print(int x) {
  if (!x) return;
  down(x);
  print(ch[x][0]), cout << val[x] << ' ', print(ch[x][1]);
}

/*
 * ============================================================
 * 名称：Splay 伸展树（文艺平衡树：区间反转/序列操作）
 * 复杂度：均摊 O(log n) 每次操作
 * 用途：序列维护：区间反转、区间插入删除、区间平移、可持久化不可行但实现直观；
 *       把区间 [l, r] 的两侧端点 kth 后 splay 到根与根的右子，区间即根的
 *       右儿子的左子树，打懒标记或整段操作即可
 * 原理：每次访问把节点旋转到根（splay），摊还分析保证总复杂度 O((n+q) log n)
 * 注意：与 BalancedTree_Treap.cpp、BalancedTree_WBST_*.cpp 为同算法（平衡树）
 *       的不同实现；本文件为序列版（含哨兵：build 时 1..n 外包 0 与 n+1 两个哨兵
 *       可避免 kth 越界——示例里直接建 0..n+1，kth 对应位置 +1）
 * 用法：rt = build(0, n + 1); reverse(l, r) 区间反转（l、r 为实际位置）
 * ============================================================
 * 使用示例（编译时取消注释；区间反转）：
 * signed main() {
 *   int n, m;
 *   cin >> n >> m;
 *   rt = build(0, n + 1);
 *   while (m--) {
 *     int l, r;
 *     cin >> l >> r;
 *     reverse(l, r);
 *   }
 *   print(rt);
 * }
 * ============================================================
 */
