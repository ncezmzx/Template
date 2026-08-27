#include <bits/stdc++.h>
using namespace std;
// ============================================================
// 名称：迭代式线段树（自底向上 bottom-up，单点更新 + 区间查询）
// 复杂度：单点更新 O(log n)，区间查询 O(log n)
// 用途：单点赋值/修改 + 区间最值（此处为 pair 最大值；改求和/最小值只需改合并函数）
// 使用示例：
//   update(x, {v, x});   // 第 x 个位置置为 (v, x)
//   query(l, r);         // 区间 [l, r] 的最大 (值, 下标) 对
// 来源：all.cpp 第 14784-14803 行（sgt 数组版；同一块还有基于分块 blk 的 sgt1 变体）
// 调整说明：原块中 sgt1/blk 为分块特化，与 sgt 重复，此处只保留 sgt 版；
//           query 会先把 l/r 裁剪进 [1, n]，空区间返回 {-1,-1}。
// ============================================================
constexpr int N = 1e6 + 9;  // 按需调整
int n;
pair<int, int> sgt[N << 1];  // 叶子在 [n, 2n-1]，根为 1

// 单点更新：sgt[x] = y，然后自底向上重算祖先
void update(int x, const pair<int, int>& y) {
  sgt[x += n - 1] = y;
  for (x >>= 1; x; x >>= 1) sgt[x] = max(sgt[x << 1], sgt[x << 1 | 1]);
}
// 区间查询 [l, r] 最大值（l/r 越界自动裁剪）
pair<int, int> query(int l, int r) {
  l = max(1, l), r = min(r, n);
  if (l > r) return {-1, -1};
  pair<int, int> res;
  for (l += n - 1, r += n; l ^ r; l >>= 1, r >>= 1) {
    if (l & 1) res = max(res, sgt[l++]);
    if (r & 1) res = max(res, sgt[--r]);
  }
  return res;
}

#ifdef DEMO
signed main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  n = 5;
  update(1, {3, 1}), update(2, {7, 2}), update(3, {5, 3}), update(4, {9, 4}), update(5, {2, 5});
  auto p = query(1, 5);      // (9, 4)
  cout << p.first << ' ' << p.second << '\n';
  p = query(2, 3);           // (7, 2)
  cout << p.first << ' ' << p.second << '\n';
  update(4, {1, 4});         // 改后 [1,5] 最大为 (7,2)
  cout << query(1, 5).first << '\n';
  return 0;
}
#endif
