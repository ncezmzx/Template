#include <bits/stdc++.h>
using namespace std;
#define int long long

// KMP prefix function / failure table (strings are 1-indexed: s = ' ' + s)
constexpr int N = 1e6 + 9;
int n, nxt[N]; // nxt[i] = longest border length of s[1..i] (failure jump)
string s;      // pattern; use as s = ' ' + s

// build nxt (s is 1-indexed with s[0] a placeholder; n = |s| - 1)
void build_nxt() {
  for (int j = 0, i = 2; i <= n; ++i) {
    while (j && s[j + 1] != s[i]) j = nxt[j];
    j = nxt[i] = j + (s[j + 1] == s[i]);
  }
}

// match: t = pattern (1-indexed, nxt built on the global s == t), s = text;
// returns the number of occurrences of t in the text
int match(const string &t, const string &s) {
  int m = t.size() - 1, M = s.size() - 1, cnt = 0;
  for (int i = 1, j = 0; i <= M; ++i) {
    while (j && t[j + 1] != s[i]) j = nxt[j];
    if ((j += (t[j + 1] == s[i])) == m) ++cnt; // full pattern hit (could record i instead)
  }
  return cnt;
}

// ========== variant: KMP automaton to[i][c] (all.cpp 6476-6499, commented) ==========
// The original was over binary strings ('0'/'1'), to[N][2]; for other alphabets
// replace 2 by the alphabet size. After building fail, to[i][c] is the next
// state from i on character c, so matching is O(1) per step:
//
// int fail[N], to[N][2];
// string s;                       // 1-indexed
// s = ' ' + s;
// for (int i = 2, j = 0; i <= n; ++i) {
//   while (j && s[j + 1] != s[i]) j = fail[j];
//   fail[i] = j += s[j + 1] == s[i];
// }
// for (int i = 0; i <= n; ++i) {
//   for (int j = 0; j < 2; ++j) to[i][j] = to[fail[i]][j];
//   if (i < n) to[i][s[i + 1] - '0'] = i + 1;
// }
// // matching: j = 0; for (char c : text) { j = to[j][c - '0']; if (j == n) { hit; } }
// // note: the original dep[]/f[] were problem-specific statistics, omitted.

/*
 * ============================================================
 * Name: KMP prefix function / failure table (nxt)
 * Complexity: build O(n), matching O(n + m)
 * Usage: pattern matching, minimal period, border counting; kept as free
 *        functions on globals n/s/nxt: n = |pattern|, s = ' ' + pattern,
 *        build_nxt(), then match(pattern, ' ' + text) counts occurrences
 * Source: all.cpp lines 57134-57137 (build nxt)
 *         all.cpp lines 11199-11202 (matching loop)
 *         all.cpp lines 6476-6499 (KMP automaton variant, commented)
 * ============================================================
 * Example (uncomment to compile):
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   string txt, pat;
 *   cin >> txt >> pat;             // text first, then pattern
 *   n = pat.size(), s = ' ' + pat; // nxt is built on the global s (pattern)
 *   build_nxt();
 *   cout << match(s, ' ' + txt) << '\n';  // occurrences of pattern in text
 *   return 0;
 * }
 * ============================================================
 */
