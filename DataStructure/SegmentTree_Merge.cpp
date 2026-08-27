#include <bits/stdc++.h>
using namespace std;

// 线段树合并：动态开点权值线段树的合并（b 并入 a），配合树上统计
constexpr int N = 2e5 + 9, SP = N * 20 + 9;
int tot, lc[SP], rc[SP], sz[SP];

void init() { tot = 0; }
int new_node() {
  return ++tot, lc[tot] = rc[tot] = 0, sz[tot] = 0, tot;
}
void insert(int& u, int l, int r, int x, int w = 1) {  // 值 x 计数 += w
  if (!u) u = new_node();
  sz[u] += w;
  if (l == r) return;
  int m = l + r >> 1;
  x <= m ? insert(lc[u], l, m, x, w) : insert(rc[u], m + 1, r, x, w);
}
int merge(int a, int b, int l, int r) {  // b 并入 a 并返回新根（b 作废）
  if (!a || !b) return a + b;
  if (l == r) return sz[a] += sz[b], a;
  int m = l + r >> 1;
  lc[a] = merge(lc[a], lc[b], l, m);
  rc[a] = merge(rc[a], rc[b], m + 1, r);
  sz[a] = sz[lc[a]] + sz[rc[a]];
  return a;
}
int kth(int u, int l, int r, int k) {  // 第 k 小值（k 超过总数返回 -1）
  if (!u || k > sz[u]) return -1;
  while (l < r) {
    int m = l + r >> 1;
    if (k <= sz[lc[u]]) u = lc[u], r = m;
    else k -= sz[lc[u]], u = rc[u], l = m + 1;
  }
  return l;
}
int count_le(int u, int l, int r, int x) {  // 值 <= x 的个数
  if (!u || x < l) return 0;
  if (r <= x) return sz[u];
  int m = l + r >> 1;
  return count_le(lc[u], l, m, x) + (x > m ? count_le(rc[u], m + 1, r, x) : 0);
}

/*
 * ============================================================
 * 名称：线段树合并（动态开点权值线段树）
 * 复杂度：单次合并均摊 O(log V)（总代价 = 总节点量级）；
 *       insert O(log V)，kth / count_le O(log V)
 * 用途：多棵权值线段树的合并统计——树上启发式合并的替代、
 *       子树第 k 小、区间内某值域计数（每个位置/子树一棵树）
 * 接口：init() 清空；insert(root, 1, V, x[, w]) 插入；
 *       merge(a, b, 1, V) 把 b 并入 a；kth / count_le 查询
 * 原理：动态开点使空子树为 0，合并时双指针下递归，单侧为空直接
 *       挂另一侧 O(1)；合并两树的总代价不超过较小树的节点数，
 *       全局总代价 O((n + q) log V)
 * 注意：merge 后 b 树节点作废（不可再用）；值域 V 需先离散化；
 *       SP = 插入总次数 × log V，按需调整
 * ============================================================
 * 使用示例（编译时取消注释；两棵树合并查询）：
 * signed main() {
 *   init();
 *   int A = 0, B = 0;
 *   for (int x : {1, 3, 5, 5}) insert(A, 1, 10, x);
 *   for (int x : {2, 4, 5})    insert(B, 1, 10, x);
 *   A = merge(A, B, 1, 10);
 *   cout << kth(A, 1, 10, 5) << ' ' << kth(A, 1, 10, 7) << '\n';  // 5 -1（共 6 个数：1,2,3,4,5,5）
 *   cout << count_le(A, 1, 10, 3) << ' ' << count_le(A, 1, 10, 5) << '\n';  // 3 6
 * }
 */
