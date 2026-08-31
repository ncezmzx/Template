#include <bits/stdc++.h>
using namespace std;
#define int long long

// digit DP (binary form): count numbers in [0, x] with exactly k one-bits
struct digit_dp_binary {
  int k, dp[100][100][2], c, a[100];
  // fill from high bit to low: p = current bit, q = ones still to place,
  // top = all higher bits were tight (limits this bit by up)
  int dfs(int p, int q, int top) {
    if (q < 0) return 0;
    if (!p) return q == 0;
    auto &ret = dp[p][q][top];
    if (~ret) return ret;
    ret = 0;
    for (int i = 0, up = top ? a[p] : 1; i <= up; ++i) ret += dfs(p - 1, q - i, top && i == up);
    return ret;
  }
  int sol(int x) { // count in [0, x]; memoization only for non-tight states
    memset(dp, -1, sizeof dp);
    for (c = 0; x; x >>= 1) a[++c] = x & 1; // a[1] = lowest bit
    return dfs(c, k, 1);
  }
};

/*
 * ============================================================
 * Name: digit DP (binary version; count numbers in [0, x] with exactly k one-bits)
 * Complexity: O(#bits * k * 2) (memoized dp[p][q][top], p ~ 60, q <= k)
 * Usage: generic digit-DP skeleton, wrapped as digit_dp_binary:
 *       dfs(p, q, top) fills bits high-to-low (p = current bit, q = remaining
 *       state, top = still tight, which bounds the next digit by up);
 *       sol(x) splits x into bits a[1..c] and returns the count; memoization
 *       applies only to non-tight (top == 0) states. Here it counts
 *       "exactly k ones in 1..x"; combine with binary search / inclusion-
 *       exclusion for "numbers in [l, r] satisfying some digit property".
 * Source: all.cpp 2602-2631 (extracted verbatim). A decimal digit-DP variant
 *         (leading-zero pre flag + aggregated sums) follows below
 *         (all.cpp 50486-50519).
 * ============================================================
 * Example (uncomment to compile; input "m k" -> least x with >= m numbers in (x, 2x] having exactly k one-bits):
 * digit_dp_binary dd;
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   int m;
 *   cin >> m >> dd.k;
 *   auto chk = [&](int x) { return dd.sol(2 * x) - dd.sol(x) >= m; };
 *   int l = 1, r = 1e18;
 *   while (l < r) {
 *     int mid = (l + r) >> 1;
 *     if (chk(mid)) r = mid;
 *     else l = mid + 1;
 *   }
 *   cout << l;
 *   return 0;
 * }
 * ============================================================
  *

 * ── Decimal digit-DP variant (all.cpp 50486-50519, with leading-zero pre flag
 *    and aggregated sums) ─────────────────────────────────────────────────────
 * Decimal digit extraction uses a[++c] = x % 10; besides top the state has
 * pre (still all leading zeros); memoize when !top && !pre; the return struct
 * p0/p1/p2 aggregates count / sum / sum-of-squares.
 *
 * int a[20], c, pw[20];
 * struct info { int p0, p1, p2; info() = default; } dp[20][10][1 << 10];
 * info dfs(int p, bool top, bool pre, int S) {
 *   if (!p) return {__builtin_popcount(S) == s, 0, 0};
 *   if (!top && !pre && ~dp[p][s][S].p0) return dp[p][s][S];
 *   info ret = {0, 0, 0};
 *   int up = (top ? a[p] : 9);
 *   for (int i = 0; i <= up; ++i) {
 *     int nS = prg(S, i);
 *     auto nw = dfs(p - 1, top && (i == up), pre && (i == 0), (pre && (i == 0)) ? 0 : nS);
 *     int d = i * pw[p - 1] % md;
 *     (ret.p0 += nw.p0) %= md;
 *     (ret.p1 += nw.p1 + nw.p0 * d) %= md;
 *     (ret.p2 += nw.p2 + 2 * nw.p1 * d + d * d % md * nw.p0) %= md;
 *   }
 *   if (!top && !pre) dp[p][s][S] = ret;
 *   return ret;
 * }
 * int f(int x) {
 *   for (c = 0; x; x /= 10) a[++c] = x % 10;
 *   auto nw = dfs(c, true, true, 0);
 *   ...
 * }
 */
