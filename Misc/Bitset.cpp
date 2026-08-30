#include <bits/stdc++.h>
using namespace std;
#define int long long
constexpr int N = 3e5 + 9;
string s, t;
bitset<N> a[26];
/*
 * ============================================================
 * Name: bitset trick — wildcard string matching (26 per-character bitsets + AND + shift)
 * Complexity: O(|S| * |T| / 64) (each ans &= a[c] and ans <<= 1 costs
 *             O(|T|/64))
 * Usage: all match starts when the pattern t contains '*' (matching any one
 *        character); one bitset a[c] per character c maintains "is each
 *        position still possible" in parallel:
 *        for each character of s, ans &= a[c] then shift left by one.
 * Source: all.cpp lines 33335-33356 (kept verbatim, comments translated)
 * Notes: the demo input is lowercase letters and '*' only
 * ============================================================
 */
