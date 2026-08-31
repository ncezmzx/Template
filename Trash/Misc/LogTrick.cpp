#include <bits/stdc++.h>
using namespace std;

// log trick: range gcd updates via ODT-style segment merging
struct log_trick_odt {
  map<int, int> mp; // segment table: key = segment start l, value v (segment [l, next(l)-1])
  void split(int x) {
    auto it = prev(mp.upper_bound(x));
    mp[x] = it->second;
  }
  void assign(int l, int r, int x) { // a[l..r] = x
    split(l), split(r + 1);
    auto it = mp.find(l);
    while (it->first != r + 1) it = mp.erase(it);
    mp[l] = x;
  }
  void perform(int l, int r, int x) { // a[i] = gcd(a[i], x) on [l, r]
    split(l), split(r + 1);
    auto it = mp.find(l);
    while (it->first != r + 1) {
      if (it->second > x) it->second = __gcd(it->second, x);
      it = next(it);
    }
  }
};

/*
 * ============================================================
 * Name: log trick — range gcd segment merging (ODT-style gcd updates)
 * Complexity: each perform amortized O(#segments * log V): gcd only decreases
 *             values and equal-valued runs are kept whole, so each segment's
 *             value changes at most O(log V) times
 * Usage: range "a[i] = gcd(a[i], x)" updates combined with range assignment /
 *        point queries, wrapped as log_trick_odt; typical problem: range gcd
 *        + range assign + point output
 * Source: all.cpp 48974-48992 (map/split/assign/perform extracted verbatim,
 *         mp now a member); original initialization (all.cpp 49000-49001):
 *         mp[1] = -1, mp[n] = -1; then mp[i] = a[i] per position; the
 *         element-wise gcd form at all.cpp 48959:
 *         if (a[i] > z) a[i] = __gcd(a[i], z);
 * Note: the classic "scan right endpoint keeping map<gcd, count>" (all
 *       subarray gcd counts in O(n log V)) does not appear in all.cpp; this
 *       is the ODT segment-merge form actually used there (same log-trick idea)
 * ============================================================
 * Example (uncomment to compile):
 * log_trick_odt lt;
 * int main() {
 *   int n = 6;
 *   lt.mp[1] = -1, lt.mp[n] = -1;              // original initialization style
 *   for (int i = 1; i <= n; ++i) lt.mp[i] = 12;  // initial a[1..6] = 12
 *   lt.perform(2, 5, 8);                       // a[i] = gcd(a[i], 8)
 *   for (int i = 1; i <= n; ++i) lt.split(i), lt.split(i + 1), cout << lt.mp[i] << " \n"[i == n];
 *                                              // output: 12 4 4 4 4 12
 *   lt.assign(1, 3, 7);                        // range assign [1,3] = 7
 *   for (int i = 1; i <= n; ++i) lt.split(i), lt.split(i + 1), cout << lt.mp[i] << " \n"[i == n];
 *                                              // output: 7 7 7 4 4 12
 * }
 * ============================================================
 */
