#include <bits/stdc++.h>
using namespace std;
#define int long long

// Manacher palindrome radii on the transformed string "&|c|c|...|?"
template <size_t N>
struct manacher {
  int p[2 * N];  // p[i] = radius at position i of t (1-indexed)
  string t;
  int m;
  // builds t from s, fills p; returns the longest palindromic substring length
  int build(const string& s) {
    t = "&|";
    for (char c : s) t += c, t += '|';
    t += '?';
    m = (int)t.size() - 1;
    int ans = 0;
    for (int i = 1, mid = 0, r = 0; i <= m; ++i) {
      p[i] = (i > r ? 1 : min(r - i + 1, p[2 * mid - i]));
      while (t[i + p[i]] == t[i - p[i]]) ++p[i];
      if (p[i] + i - 1 > r) mid = i, r = p[i] + i - 1;
      ans = max(ans, p[i]);
    }
    return ans - 1;
  }
};

/*
 * ============================================================
 * Name: Manacher palindromes
 * Complexity: O(n)
 * Usage: longest palindromic substring and palindrome statistics, wrapped as
 *        manacher<N>: build(s) constructs the transformed string
 *        "&|c|c|...|?" (sentinels make expansion safe), computes the radius
 *        array p (p[i] = palindrome radius at transformed position i), and
 *        returns the longest palindromic substring length (= max p[i] - 1)
 * Source: all.cpp lines 57102-57123 (wrapped into a struct; the radius
 *         update condition uses the standard p[i]+i-1 > r form)
 * ============================================================
 * Example (uncomment to compile):
 * static manacher<1000> mc;
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   string s;
 *   cin >> s;
 *   int ans = mc.build(s);
 *   cout << ans << '\n';                 // longest palindromic substring length
 *   return 0;
 * }
 * ============================================================
 * Original counting example (all.cpp 57112, 57115-57122; define md/n/k/pal/ans and qpow first):
 * needs: constexpr int md = 19930726;  int n, k, pal[N], ans = 1;
 * int qpow(int a, int b) { int r = 1; for (; b; b >>= 1, a = a * a % md) if (b & 1) r = r * a % md; return r; }
 * insert inside the radius loop: ++pal[p[i] - 1];
 * after the loop:
 * int s = 0;
 * for (int i = n; k && i >= 1; --i) {
 *   if (i % 2 == 0) continue;
 *   int t = min(k, s += pal[i]);
 *   k -= t;
 *   (ans *= qpow(i, t)) %= md;
 * }
 * if (k) cout << -1; else cout << ans;
 * ============================================================
 */
