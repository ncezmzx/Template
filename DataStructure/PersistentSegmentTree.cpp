#include <bits/stdc++.h>
using namespace std;

// 主席树（可持久化权值线段树）：静态区间第 k 小 / 区间 <= x 计数
// rt[i] 为前缀 i 的版本，按值域建树，空间 = (n + q)·⌈log2 V⌉
constexpr int N = 2e5 + 9, SP = N * 20 + 9;
int n_, V, tot, rt[N], lc[SP], rc[SP], cnt_[SP];
vector<int> b_;  // 离散化值域（1..V）

int upd(int pre, int l, int r, int x) {  // pre 版本上位置 x 计数 +1
  int u = ++tot;
  lc[u] = lc[pre], rc[u] = rc[pre], cnt_[u] = cnt_[pre] + 1;
  if (l == r) return u;
  int m = l + r >> 1;
  x <= m ? lc[u] = upd(lc[pre], l, m, x) : rc[u] = upd(rc[pre], m + 1, r, x);
  return u;
}
void init(const vector<int>& a) {  // a[1..n]（1-indexed，传 n+1 长度含 a[0] 占位）
  n_ = (int)a.size() - 1;
  b_.assign(a.begin() + 1, a.end());
  sort(b_.begin(), b_.end()), b_.erase(unique(b_.begin(), b_.end()), b_.end());
  V = (int)b_.size(), tot = 0, rt[0] = 0;
  for (int i = 1; i <= n_; ++i) rt[i] = upd(rt[i - 1], 1, V, (int)(lower_bound(b_.begin(), b_.end(), a[i]) - b_.begin()) + 1);
}
int query_kth(int l, int r, int k) {  // 静态区间 [l, r]（1-indexed）第 k 小（返回原值）
  int u = rt[l - 1], v = rt[r], L = 1, R = V;
  while (L < R) {
    int m = L + R >> 1, c = cnt_[lc[v]] - cnt_[lc[u]];
    if (k <= c) u = lc[u], v = lc[v], R = m;
    else k -= c, u = rc[u], v = rc[v], L = m + 1;
  }
  return b_[L - 1];
}
int query_le(int l, int r, int x) {  // 区间 [l, r] 内值 <= x 的个数（原坐标系）
  int u = rt[l - 1], v = rt[r], L = 1, R = V, res = 0;
  while (L < R) {
    int m = L + R >> 1;
    if (x < b_[m]) {  // x 落在左半：右半全部 > x
      u = lc[u], v = lc[v], R = m;
    } else {  // 左半全部 <= x
      res += cnt_[lc[v]] - cnt_[lc[u]];
      u = rc[u], v = rc[v], L = m + 1;
    }
  }
  return res + (b_[L - 1] <= x ? cnt_[v] - cnt_[u] : 0);  // 叶子层（版本差叶计数 0/多次）
}

/*
 * ============================================================
 * 名称：主席树（可持久化线段树，静态区间第 k 小）
 * 复杂度：build O(n log n)；query O(log n)；空间 O((n)·log V)
 * 用途：在线询问静态数列的区间第 k 小 / 区间 rank / 区间内
 *       值 <= x 计数（比整体二分离线自由，比树套树省内存）
 * 接口：init(a)（a[0] 占位，a[1..n] 为数列，内部离散化）；
 *       query_kth(l, r, k) 区间第 k 小（原值）；
 *       query_le(l, r, x) 区间内 <= x 的个数
 * 原理：值域线段树按前缀版本化——rt[i] 在 rt[i-1] 基础上把 a[i]
 *       位置 +1（路径克隆，其余共享）；两版本相减即任意区间的
 *       值域分布，第 k 小沿值域树下行
 * 注意：SP = N·20（N = 2e5，log V ≈ 18），按数据量调整；
 *       位置 1-indexed；值需可离散化（重复值支持）
 * ============================================================
 * 使用示例（编译时取消注释；洛谷 P3834）：
 * signed main() {
 *   vector<int> a{0, 1, 5, 2, 4, 3};  // n = 5: {1,5,2,4,3}
 *   init(a);
 *   cout << query_kth(2, 4, 2) << '\n';  // 4（{5,2,4} 第 2 小）
 *   cout << query_kth(1, 5, 1) << '\n';  // 1
 *   cout << query_le(1, 5, 3) << '\n';   // 3（1,2,3）
 *   cout << query_le(2, 3, 2) << '\n';   // 1（2）
 * }
 */
