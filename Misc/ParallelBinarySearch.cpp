#include <bits/stdc++.h>
using namespace std;
#define int long long

// 整体二分（Parallel Binary Search）：离线静态区间第 k 小
// 对"答案值域"整体二分，每层用 BIT 数区间内 ≤ mid 的个数分流询问
constexpr int N = 2e5 + 9;
int bit[N], n_;
inline void bit_add(int i, int v) {
  for (; i <= n_; i += i & -i) bit[i] += v;
}
inline int bit_sum(int i) {
  int s = 0;
  for (; i > 0; i -= i & -i) s += bit[i];
  return s;
}

struct Q {
  int l, r, k, id;
};
// a[1..n] 原数列；qs = {{l, r, k}, ...}（1-indexed）；返回按输入顺序的第 k 小原值
vector<int> parallel_kth(int n, const vector<int>& a, const vector<array<int, 3>>& qs) {
  n_ = n;
  memset(bit, 0, (n + 1) * sizeof(int));
  vector<int> vals(a.begin() + 1, a.end());
  sort(vals.begin(), vals.end());
  vals.erase(unique(vals.begin(), vals.end()), vals.end());
  int V = vals.size();
  vector<vector<int>> pos(V + 1);  // 离散值 i 的所有位置
  for (int i = 1; i <= n; ++i)
    pos[(int)(lower_bound(vals.begin(), vals.end(), a[i]) - vals.begin()) + 1].push_back(i);
  vector<Q> cur;
  for (int i = 0; i < (int)qs.size(); ++i) cur.push_back({qs[i][0], qs[i][1], qs[i][2], i});
  vector<int> ans(qs.size());
  function<void(int, int, vector<Q>&)> solve = [&](int L, int R, vector<Q>& q) {
    if (q.empty()) return;
    if (L == R) {
      for (auto& x : q) ans[x.id] = vals[L - 1];
      return;
    }
    int mid = L + R >> 1;
    for (int i = L; i <= mid; ++i)  // 加入值域 [L, mid] 的位置
      for (int p : pos[i]) bit_add(p, 1);
    vector<Q> lq, rq;
    for (auto& x : q) {
      int c = bit_sum(x.r) - bit_sum(x.l - 1);  // 区间内值 ≤ mid 的个数
      if (c >= x.k) lq.push_back(x);
      else rq.push_back({x.l, x.r, x.k - c, x.id});
    }
    for (int i = L; i <= mid; ++i)  // 回滚 BIT，保证进入下层时干净
      for (int p : pos[i]) bit_add(p, -1);
    solve(L, mid, lq);
    solve(mid + 1, R, rq);
  };
  solve(1, V, cur);
  return ans;
}

/*
 * ============================================================
 * 名称：整体二分（Parallel Binary Search）
 * 复杂度：O((n + q) log² n)
 * 用途：一类"多组询问、答案单调可二分、可离线"的问题，把所有询问
 *       一起二分答案：经典是静态区间第 k 小（替代树套树/主席树），
 *       也可做"带修第 k 小""二维第 k 小"等（把修改也当作事件二分）
 * 接口：parallel_kth(n, a[1..n], {{l, r, k}, ...}) → 每问区间第 k 小
 * 原理：在答案值域 [L, R] 上递归；每层把值 ≤ mid 的位置加入 BIT，
 *       对每个询问数出 [l, r] 内 ≤ mid 的个数 c：c ≥ k 归左，否则
 *       k -= c 归右；归右前撤销本层 BIT 修改
 * 注意：值需可离散化；k 合法（1 ≤ k ≤ r-l+1）；多测注意重设 bit
 * 来源：OI-Wiki《整体二分》（https://oi-wiki.org/misc/parallel-binsearch/）
 * ============================================================
 * 使用示例（编译时取消注释；洛谷 P3834）：
 * signed main() {
 *   vector<int> a{0, 1, 5, 2, 4, 3};  // n = 5: {1,5,2,4,3}
 *   auto r = parallel_kth(5, a, {{2, 4, 2}, {1, 5, 1}, {1, 5, 5}});
 *   for (int x : r) cout << x << ' ';  // 4 1 5
 *   cout << '\n';
 * }
 * ============================================================
 */
