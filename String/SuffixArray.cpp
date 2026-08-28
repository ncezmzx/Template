#include <bits/stdc++.h>
using namespace std;
#define int long long

// suffix array (doubling + radix sort) with height array; 1-indexed
template <size_t N>
struct suffix_array {
  int n, sw;  // sw = second-key offset of current round (-1 = none)
  int a[N], rk[N], sa[N], id[N], buc[N], h[N];
  bool samerank(int x, int y) {  // inline second-key compare, avoids pair construction
    if (rk[x] != rk[y]) return false;
    if (sw < 0) return true;
    int px = x + sw <= n ? rk[x + sw] : 0, py = y + sw <= n ? rk[y + sw] : 0;
    return px == py;
  }
  void bucsort(int m) {
    memset(buc + 1, 0, m * sizeof(int));
    for (int i = 1; i <= n; ++i) ++buc[rk[i]];
    for (int i = 1; i <= m; ++i) buc[i] += buc[i - 1];
    for (int i = n; i >= 1; --i) sa[buc[rk[id[i]]]--] = id[i];  // id is sorted by second key
    id[sa[1]] = 1;
    for (int i = 2; i <= n; ++i)
      id[sa[i]] = id[sa[i - 1]] + !samerank(sa[i - 1], sa[i]);
    memcpy(rk + 1, id + 1, n * sizeof(int));
  }
  // requires a[1..n] filled with values in [1, n] and a[n+1] = 0 (sentinel)
  void build() {
    for (int i = 1; i <= n; ++i) rk[i] = a[i], sa[i] = id[i] = i;
    sw = -1;
    bucsort(n);
    for (int w = 1, p = 0, m = rk[sa[n]]; w < n; w <<= 1, m = rk[sa[n]], p = 0) {
      for (int j = n - w + 1; j <= n; ++j) id[++p] = j;
      for (int j = 1; j <= n; ++j)
        if (sa[j] > w) id[++p] = sa[j] - w;
      sw = w;
      bucsort(m);
      if (rk[sa[n]] == n) break;
    }
  }
  void build_height() {
    for (int i = 1, k = 0; i <= n; ++i) {
      if (k) --k;
      while (a[i + k] == a[sa[rk[i] - 1] + k]) ++k;
      h[rk[i]] = k;
    }
  }
};

/*
 * ============================================================
 * Name: suffix array SA (doubling + radix sort) with the height array
 * Complexity: O(n log n) to build SA, O(n) for height
 * Usage: suffix sorting, LCP queries, distinct-substring counting, substring
 *        occurrence statistics. Wrapped as suffix_array<N>: fill sa.n and
 *        sa.a[1..n], then build() (doubling + radix sort) and
 *        build_height(); results in sa.sa / sa.rk / sa.h.
 *        Note: a[1..n] must be integers >= 1 (map strings through a
 *        compression first); the first bucsort has bucket cap n, so initial
 *        values must be <= n; sentinel a[n+1] = 0 (global arrays default to
 *        0; the height loop relies on it to stop comparing).
 * Source: all.cpp lines 27981-28028 (wrapped into a struct, logic unchanged)
 * ============================================================
 * Example (uncomment to compile; map the string to an integer sequence):
 * static suffix_array<200> sfx;
 * signed main() {
 *   string str = "banana";
 *   int n = str.size();
 *   sfx.n = n;
 *   // map chars to integers >= 1; first bucsort has bucket cap n, so compress:
 *   vector<int> val(256, 0);
 *   for (int i = 0; i < n; ++i) val[str[i]] = 1;
 *   int tot = 0;
 *   for (int c = 0; c < 256; ++c) if (val[c]) val[c] = ++tot;
 *   for (int i = 1; i <= n; ++i) sfx.a[i] = val[str[i - 1]];
 *   sfx.a[n + 1] = 0;                  // sentinel (0 by default, written explicitly for safety)
 *   sfx.build();
 *   sfx.build_height();
 *   for (int i = 1; i <= n; ++i)
 *     cout << sfx.sa[i] << ' ' << sfx.h[i] << ' ' << str.substr(sfx.sa[i] - 1) << '\n';
 *   // distinct substring count = n * (n + 1) / 2 - sum(h[2..n])
 * }
 */
