#include <bits/stdc++.h>
using namespace std;
// ============================================================
// 名称：线段树（区间加 + 区间和，带懒标记 lazy tag）
// 复杂度：build/update/query 均 O(log n)
// 用途：区间加（range add）、区间求和（range sum）、find 定位前缀和 >= k 的位置
// 使用示例：
//   update(1, 1, n, l, r, z);   // a[l..r] += z
//   query(1, 1, n, l, r);       // sum(a[l..r])
//   find(1, 1, n, k);           // 第一个前缀和 >= k 的下标
// 来源：all.cpp 第 48453-48484 行（range-add + range-sum 版，含 find）
// 调整说明：原代码块无 build（sgt/tg 为全局零初始化，直接 update 即可）；
//           原指定行 12453-12489 为区间加 + 区间 max 版，如需最值可参照。
// ============================================================
constexpr int N = 3e5 + 9;  // 按需调整
int n;
long long sgt[N << 2], tg[N << 2];  // sgt: 区间和, tg: 懒标记

// 区间加：整段打懒标记
void apply(int u, int l, int r, long long x) { sgt[u] += (r - l + 1) * x, tg[u] += x; }
// 下传懒标记
void down(int u, int l, int r) {
  if (tg[u]) {
    int m = (l + r) >> 1;
    apply(u << 1, l, m, tg[u]), apply(u << 1 | 1, m + 1, r, tg[u]);
    tg[u] = 0;
  }
}
// 区间加：a[x..y] += z
void update(int u, int l, int r, int x, int y, int z) {
  if (x <= l && r <= y) return apply(u, l, r, z);
  int m = (l + r) >> 1;
  down(u, l, r);
  if (m >= x) update(u << 1, l, m, x, y, z);
  if (m < y) update(u << 1 | 1, m + 1, r, x, y, z);
  sgt[u] = sgt[u << 1] + sgt[u << 1 | 1];
}
// 找第一个前缀和 >= k 的下标（要求全为非负？原用法为权重定位，按需使用）
int find(int u, int l, int r, long long k) {
  if (l == r) return l;
  int m = (l + r) >> 1;
  down(u, l, r);
  if (sgt[u << 1] >= k) return find(u << 1, l, m, k);
  else return find(u << 1 | 1, m + 1, r, k - sgt[u << 1]);
}
// 区间求和：sum(a[x..y])
long long query(int u, int l, int r, int x, int y) {
  if (x <= l && r <= y) return sgt[u];
  int m = (l + r) >> 1;
  long long res = 0;
  down(u, l, r);
  if (m >= x) res += query(u << 1, l, m, x, y);
  if (m < y) res += query(u << 1 | 1, m + 1, r, x, y);
  return res;
}

#ifdef DEMO
signed main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  n = 5;
  update(1, 1, n, 2, 4, 3);            // a = {0,3,3,3,0}
  cout << query(1, 1, n, 1, 5) << '\n'; // 9
  cout << query(1, 1, n, 3, 3) << '\n'; // 3
  cout << find(1, 1, n, 7) << '\n';     // 前缀和 >= 7 的位置 -> 4
  return 0;
}
#endif
