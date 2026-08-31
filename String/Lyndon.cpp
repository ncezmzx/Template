#include <bits/stdc++.h>
using namespace std;

// Lyndon factorization (Duval): s = w1 w2 ... wk, Lyndon words with w1 >= w2 >= ... >= wk
vector<pair<int, int>> duval(const string &s) { // factor ranges [a, b)
  int n = (int)s.size(), i = 0;
  vector<pair<int, int>> res;
  while (i < n) {
    int j = i + 1, k = i;
    while (j < n && s[k] <= s[j]) {
      if (s[k] < s[j]) k = i; // period broken, restart comparison
      else ++k;
      ++j;
    }
    while (i <= k) { // one factor per period length
      res.push_back({i, i + j - k});
      i += j - k;
    }
  }
  return res;
}

// lexicographically smallest rotation start index; O(n)
int min_representation(const string &s) {
  int n = (int)s.size(), i = 0, j = 1, k = 0;
  while (i < n && j < n && k < n) {
    int a = s[(i + k) % n], b = s[(j + k) % n];
    if (a == b) ++k;
    else {
      if (a > b) i += k + 1; // starts i..i+k cannot be minimal
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
 * Usage: duval(s) splits s into lexicographically non-increasing Lyndon factors
 *        (runs, distinct-substring counting, k-th smallest substrings, suffix-
 *        array helpers);
 *        min_representation(s) gives the start index of the lexicographically
 *        smallest cyclic shift (cyclic isomorphism, period problems).
 * Principle: Duval maintains a pre-factor [i, j) that is almost Lyndon plus an
 *            inner comparison pointer k: s[k] < s[j] extends the period, s[k] >
 *            s[j] cuts off one period-length Lyndon factor; the minimal
 *            representation races two starts i / j with a comparison pointer k
 * Notes: duval returns [a, b) ranges that concatenate back to the original;
 *        guard the empty string for min_representation
 * ============================================================
 */
