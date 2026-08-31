#include <bits/stdc++.h>
using namespace std;

// offline static range monoid products (cat-tree divide, topbit bucketing);
// queries are inclusive [l, r]
template <typename Mono, typename F>
void monoid_product(const std::vector<Mono> &vec, const std::vector<std::pair<int, int>> &query, F f) {
  const int n = vec.size();
  std::vector<std::vector<int>> buk(n);
  for (int id = 0; id < (int)query.size(); ++id) {
    int l = query[id].first, r = query[id].second;
    if (l == r) {
      f(id, vec[l]);
      continue;
    }
    int k = 31 - __builtin_clz(l ^ r); // topbit
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
 * Usage: static array, batch associative range products (sum / max / gcd /
 *        matrices).
 *        Queries are inclusive (l, r); l == r is answered with vec[l] directly;
 *        otherwise the callback receives f(id, product) and the answer is dp[l]
 *        * dp[r].
 * Principle: with k = topbit(l ^ r), mid = the top n-k bits of r splits l < mid
 *            <= r; dp stores prefix / suffix products outward from mid, O(log
 *            n) levels of O(n) scans
 * Source: user-provided code (structured bindings rewritten as .first/.second
 *         for pure C++14)
 * Notes: static only, and all queries must be known up front
 * ============================================================
 */
