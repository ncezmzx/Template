#include <bits/stdc++.h>
using namespace std;

// Lyndon factorization (Duval): s = w1 w2 ... wk, Lyndon words with w1 >= w2 >= ... >= wk
vector<pair<int, int>> duval(const string& s) {  // factor ranges [a, b)
  int n = (int)s.size(), i = 0;
  vector<pair<int, int>> res;
  while (i < n) {
    int j = i + 1, k = i;
    while (j < n && s[k] <= s[j]) {
      if (s[k] < s[j]) k = i;  // period broken, restart comparison
      else ++k;
      ++j;
    }
    while (i <= k) {  // one factor per period length
      res.push_back({i, i + j - k});
      i += j - k;
    }
  }
  return res;
}

// lexicographically smallest rotation start index; O(n)
int min_representation(const string& s) {
  int n = (int)s.size(), i = 0, j = 1, k = 0;
  while (i < n && j < n && k < n) {
    int a = s[(i + k) % n], b = s[(j + k) % n];
    if (a == b) ++k;
    else {
      if (a > b) i += k + 1;  // starts i..i+k cannot be minimal
      else j += k + 1;
      if (i == j) ++j;
      k = 0;
    }
  }
  return min(i, j) % n;
}

/*
 * ============================================================
 * Name: Lyndon factorization (Duval) + minimal representation
 * Complexity: both O(n), tiny constants
 * Usage: duval(s) splits s into lexicographically non-increasing Lyndon
 *        factors — for runs / distinct-substring counting, k-th smallest
 *        substrings, suffix-array helpers; min_representation(s) gives the
 *        lexicographically smallest cyclic shift's start index (cyclic
 *        isomorphism / period problems)
 * Principle: Duval maintains a "pre-factor" [i, j) that is almost Lyndon and
 *        an inner comparison pointer k: s[k] < s[j] extends the period, >
 *        cuts off one period-length Lyndon factor; minimal representation
 *        races two starts i/j with comparison pointer k
 * Notes: duval returns [a, b) ranges concatenating back to the original;
 *        O(n) factors; guard the empty string for minimal representation
 * ============================================================
 * Example (uncomment to compile):
 * signed main() {
 *   auto f = duval("ababa");                // "ab" | "ab" | "a" (non-increasing Lyndon factors)
 *   for (auto& p : f) cout << p.first << ',' << p.second << ' ';
 *   cout << '\n';                           // 0,2 2,4 4,5
 *   cout << min_representation("abab") << '\n';  // 0 (rotations {abab, baba}; abab is minimal)
 *   cout << min_representation("baab") << '\n';  // 1 (start 1 in s+s gives "aabb", minimal)
 * }
 */
