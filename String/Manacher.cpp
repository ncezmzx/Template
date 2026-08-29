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
 * Usage: `manacher<N>`: longest palindromic substring and palindrome
 *        statistics.
 *        build(s) constructs the transformed string "&|c|c|...|?" (sentinels
 *        make expansion safe), computes the radius array p
 *        (p[i] = palindrome radius at transformed position i), and returns the
 *        longest palindromic substring length (max p[i] - 1).
 * Source: all.cpp lines 57102-57123 (wrapped into a struct; the radius
 *         update condition uses the standard p[i]+i-1 > r form)
 * ============================================================
 */
