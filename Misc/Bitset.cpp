#include <bits/stdc++.h>
using namespace std;
#define int long long
constexpr int N = 3e5 + 9;
string s, t;
bitset<N> a[26];
/*
 * ============================================================
 * Name: bitset trick — wildcard string matching (26 per-character bitsets + AND + shift)
 * Complexity: O(|S| * |T| / 64) (each ans &= a[c] and ans <<= 1 costs O(|T|/64))
 * Usage: all match starts when the pattern t contains '*' (matching any one
 *        character); bitsets maintain "is each position still possible" in
 *        parallel: one bitset a[c] per character c; for each character of s,
 *        ans &= a[c] then shift left by one. The demo input is lowercase
 *        letters and '*' only.
 * Source: all.cpp lines 33335-33356 (kept verbatim, comments translated)
 * ============================================================
 * Example (uncomment to compile):
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   cin >> s >> t;
 *   for (int i = 0; i < t.size(); ++i) {
 *     if (t[i] == '*') {
 *       for (int j = 0; j < 26; ++j) a[j][i] = 1;
 *     }
 *     else a[t[i] - 'a'][i] = 1;
 *   }
 *   bitset<N> ans;
 *   ans.set();
 *   for (int i = 0; i < s.size(); ++i) {
 *     if (s[i] != '*') ans &= a[s[i] - 'a'];
 *     ans <<= 1;
 *   }
 *   for (int i = 1; i <= t.size(); ++i)
 *     if (ans[i]) cout << i - s.size() + 1 << ' ';
 * }
 * ============================================================
 */
