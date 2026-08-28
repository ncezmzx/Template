#include <bits/stdc++.h>
using namespace std;

// offline static range monoid products (cat-tree divide, topbit bucketing);
// queries are inclusive [l, r]
template <typename Mono, typename F>
void monoid_product(const std::vector<Mono> &vec,
                    const std::vector<std::pair<int, int>> &query, F f) {
  const int n = vec.size();
  std::vector<std::vector<int>> buk(n);
  for (int id = 0; id < (int)query.size(); ++id) {
    int l = query[id].first, r = query[id].second;
    if (l == r) {
      f(id, vec[l]);
      continue;
    }
    int k = 31 - __builtin_clz(l ^ r);   // topbit
    buk[r >> k << k].push_back(id);
  }

  std::vector<Mono> dp(n);
  for (int mid = 1; mid < n; ++mid) {
    int min = mid, max = mid;
    for (int id : buk[mid]) min = std::min(min, query[id].first), max = std::max(max, query[id].second);
    dp[mid - 1] = vec[mid - 1];
    for (int i = mid - 2; i >= min; --i) dp[i] = vec[i] * dp[i + 1];
    dp[mid] = vec[mid];
    for (int i = mid + 1; i <= max; ++i) dp[i] = dp[i - 1] * vec[i];
    for (int id : buk[mid]) {
      int l = query[id].first, r = query[id].second;
      f(id, dp[l] * dp[r]);
    }
  }
}

/*
 * ============================================================
 * Name: offline static range monoid products (cat-tree divide / topbit bucketing)
 * Complexity: O((n + q) log n) total, O(1) per query; space O(n + q)
 * Usage: static array, batch-answered associative range products (sum / max /
 *        gcd / matrices, ...). Queries are inclusive (l, r); l == r is
 *        answered with vec[l] directly; otherwise the callback receives the
 *        product: f(id, product), answer = dp[l] * dp[r]
 * Principle: with k = topbit(l ^ r) and mid = r's top n-k bits, l < mid <= r;
 *        dp stores prefix/suffix products outward from mid; mid only takes
 *        powers of two, giving O(log n) levels, one O(n) scan each
 * Source: user-provided code (structured bindings rewritten as .first/.second
 *         for pure C++14)
 * ============================================================
 * Example (uncomment to compile; range minimum):
 * struct Mono { long long v; };
 * Mono operator*(Mono a, Mono b) { return Mono{min(a.v, b.v)}; }
 * signed main() {
 *   vector<Mono> a{{5}, {2}, {7}, {1}, {9}, {4}};
 *   vector<pair<int,int>> qs{{0, 3}, {1, 4}, {2, 5}, {0, 5}};
 *   long long ans[4];
 *   monoid_product(a, qs, [&](int id, Mono res) {
 *     ans[id] = res.v;
 *   });
 *   for (int i = 0; i < 4; ++i) cout << ans[i] << " \n"[i == 3];
 *   // ans = {1, 1, 4, 1}
 * }
 * ============================================================
 */
