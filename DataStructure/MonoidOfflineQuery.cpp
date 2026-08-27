#include <bits/stdc++.h>
using namespace std;

// 离线静态区间半群乘积（猫树分治）：所有查询按最高分层位分桶，
// 每层一次双向扩展 O(n)，总 O((n + q) log n)，单次查询 O(1) 回调
template <typename Mono, typename F>
void monoid_product(const std::vector<Mono> &vec,
                    const std::vector<std::pair<int, int>> &query, F f) {
  const int n = vec.size();
  std::vector<std::vector<int>> buk(n);
  for (int id = 0; id < (int)query.size(); ++id) {
    int l = query[id].first, r = query[id].second;
    if (l == r) {
      continue;
    }
    int k = 31 - __builtin_clz(l ^ r);   // topbit
    buk[r >> k << k].push_back(id);
  }

  std::vector<Mono> dp(n);
  for (int mid = 1; mid < n; ++mid) {
    int min = mid, max = mid;
    for (int id : buk[mid]) {
      int l = query[id].first, r = query[id].second;
      min = std::min(min, l);
      max = std::max(max, r);
    }
    dp[mid - 1] = vec[mid - 1];
    for (int i = mid - 2; i >= min; --i) {
      dp[i] = vec[i] * dp[i + 1];
    }
    dp[mid] = vec[mid];
    for (int i = mid + 1; i <= max; ++i) {
      dp[i] = dp[i - 1] * vec[i];
    }
    for (int id : buk[mid]) {
      int l = query[id].first, r = query[id].second;
      f(dp[l], dp[r], id);   // 答案 = dp[l] * dp[r]（查询半开 [l, r) 时为 dp[l..mid)·dp[mid..r)）
    }
  }
}

/*
 * ============================================================
 * 名称：离线静态区间半群乘积（猫树分治 / 按 topbit 分桶）
 * 复杂度：O((n + q) log n) 总耗时，单查询 O(1)；空间 O(n + q)
 * 用途：静态数组、离线批量回答区间可结合乘积（和 / 最值 / gcd / 矩阵等）。
 *       查询 (l, r) 为闭区间且 l < r（l == r 直接取 vec[l]，不进回调）；
 *       回调 f(dp[l], dp[r], id) 中答案 = dp[l] * dp[r]
 * 原理：设 k = topbit(l ^ r)，mid = r 的前 n-k 位 → l < mid <= r；
 *       dp 从 mid 向两侧前缀/后缀积，答案 = dp[l] * dp[r]；
 *       mid 只取 2 的幂，共 O(log n) 层，每层一次 O(n) 扫描
 * 来源：用户提供代码（结构化绑定改写为 .first/.second 以纯 C++14 编译）
 * ============================================================
 * 使用示例（编译时取消注释；区间最小值）：
 * struct Mono { long long v; };
 * Mono operator*(Mono a, Mono b) { return Mono{min(a.v, b.v)}; }
 * signed main() {
 *   vector<Mono> a{{5}, {2}, {7}, {1}, {9}, {4}};
 *   vector<pair<int,int>> qs{{0, 3}, {1, 4}, {2, 5}, {0, 5}};
 *   long long ans[4];
 *   monoid_product(a, qs, [&](Mono L, Mono R, int id) {
 *     ans[id] = (L * R).v;
 *   });
 *   // ans = {1, 1, 4, 1}
 * }
 * ============================================================
 */
