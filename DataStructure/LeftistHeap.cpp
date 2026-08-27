#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int N = 1e5 + 9;
int val[N], dis[N], ch[N][2], fa[N], tot;

int node(int v) {
  int x = ++tot;
  val[x] = v, dis[x] = 1, fa[x] = x;
  return x;
}

int merge(int a, int b) {
  if (!a) return b;
  if (!b) return a;
  if (val[a] > val[b]) swap(a, b);
  ch[a][1] = merge(ch[a][1], b);
  if (ch[a][1]) fa[ch[a][1]] = a;
  if (dis[ch[a][0]] < dis[ch[a][1]]) swap(ch[a][0], ch[a][1]);
  dis[a] = dis[ch[a][1]] + 1;
  return a;
}

int find(int x) {
  while (fa[x] != x) x = fa[x];
  return x;
}

int pop(int x) {
  int l = ch[x][0], r = ch[x][1];
  ch[x][0] = ch[x][1] = 0, dis[x] = 1;
  if (l) fa[l] = l;
  if (r) fa[r] = r;
  return merge(l, r);
}

/*
 * ============================================================
 * 名称：左偏树（Leftist Heap，可并堆，小根堆）
 * 复杂度：merge O(log n)，pop/建堆均摊 O(log n)；find 沿 fa 上跳
 * 用途：可合并的优先队列：O(log n) 合并两堆、删除堆顶；
 *       常配合并查集思想做"每个集合的堆"（如最小生成树变体、贪心题）
 * 原理：维护 dist（到最近空儿子的距离），始终把较重的右链换到左边
 *       （dis[左] >= dis[右]），保证每次 merge 只沿一条右链递归，长度 O(log n)
 * 注意：find 不做路径压缩（堆合并后 fa 即堆树结构，压缩会破坏 pop 的
 *       父子关系）；与二叉堆（STL priority_queue，不可合并）互补；
 *       堆顶是 val 最小的节点（小根堆，取反即大根堆）
 * 用法：两堆合并：int h = merge(h1, h2)（h1、h2 为堆根）；
 *       删除根 x 所在堆的堆顶：h = pop(find(x)) 或直接 h = pop(h)
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   int n;
 *   cin >> n;
 *   vector<int> h;
 *   for (int i = 1, x; i <= n; ++i) {
 *     cin >> x;
 *     h.push_back(node(x));
 *   }
 *   while (h.size() > 1) {
 *     sort(h.begin(), h.end(), [&](int a, int b) { return val[a] > val[b]; });
 *     int a = h.back(); h.pop_back();
 *     int b = h.back(); h.pop_back();
 *     h.push_back(merge(a, b));
 *   }
 *   cout << val[h[0]] << '\n';
 * }
 * ============================================================
 */
