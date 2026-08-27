#include <bits/stdc++.h>
using namespace std;
constexpr int N = 1e5 + 9;
int n, a[N], sgt[N << 2];

void build(int u, int l, int r) {
  if (l == r) return sgt[u] = a[l], void();
  int m = (l + r) >> 1;
  build(u << 1, l, m), build(u << 1 | 1, m + 1, r);
  sgt[u] = __gcd(sgt[u << 1], sgt[u << 1 | 1]);
}
void update(int u, int l, int r, int x, int y) {
  if (l == r) return sgt[u] = y, void();
  int m = (l + r) >> 1;
  if (m >= x) update(u << 1, l, m, x, y);
  else update(u << 1 | 1, m + 1, r, x, y);
  sgt[u] = __gcd(sgt[u << 1], sgt[u << 1 | 1]);
}
int query(int u, int l, int r, int x) {
  if (r <= x) return sgt[u];
  int m = (l + r) >> 1, ans = query(u << 1, l, m, x);
  if (m < x) ans = __gcd(ans, query(u << 1 | 1, m + 1, r, x));
  return ans;
}
/*
 * ============================================================
 * 名称：线段树维护 gcd
 * 复杂度：build O(n)，单点修改 O(log n)，查询 O(log n)
 * 用途：单点修改 + 区间 gcd 查询；query(1,1,n,x) 实为前缀 gcd（= gcd(a[1..x])）
 * 来源：all.cpp 第 53170-53191 行（匿名 namespace 内，原题 "gcd" 一题；注释已统一移至文件尾部）
 * 调整说明：原块中 st/ed/xr 等为分块异或结构，属题目特化，已省略；
 *           query 语义实为前缀 gcd（原任务描述为"从 x 到右端"，与代码不符，已按代码注释）
 * ============================================================
 * 使用示例（编译时取消注释）：
 *   signed main() {
 *     cin.tie(nullptr)->sync_with_stdio(false);
 *     n = 5, a[1] = 12, a[2] = 18, a[3] = 6, a[4] = 24, a[5] = 9;
 *     build(1, 1, n);
 *     cout << query(1, 1, n, 3) << '\n';   // gcd(12,18,6) = 6
 *     update(1, 1, n, 2, 10);              // a[2] = 10
 *     cout << query(1, 1, n, 4) << '\n';   // gcd(12,10,6,24) = 2
 *     return 0;
 *   }
 * ============================================================
 */