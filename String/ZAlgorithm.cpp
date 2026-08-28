#include <bits/stdc++.h>
using namespace std;

// z[i] = LCP of s and s[i..]; z[0] = |s| (same convention as ACL z_algorithm)
vector<int> z_algorithm(const string& s) {
  int n = (int)s.size();
  if (n == 0) return {};
  vector<int> z(n);
  z[0] = 0;
  for (int i = 1, j = 0; i < n; i++) {  // j = start of the rightmost Z-box
    int& k = z[i];
    k = (j + z[j] <= i) ? 0 : min(j + z[j] - i, z[i - j]);  // reuse inside the Z-box
    while (i + k < n && s[k] == s[i + k]) k++;  // brute-force extension
    if (j + z[j] < i + z[i]) j = i;
  }
  z[0] = n;
  return z;
}

// generic container version (same template signature as ACL; works for vector<int> etc.)
template <class T>
vector<int> z_algorithm(const vector<T>& s) {
  int n = (int)s.size();
  if (n == 0) return {};
  vector<int> z(n);
  z[0] = 0;
  for (int i = 1, j = 0; i < n; i++) {
    int& k = z[i];
    k = (j + z[j] <= i) ? 0 : min(j + z[j] - i, z[i - j]);
    while (i + k < n && s[k] == s[i + k]) k++;
    if (j + z[j] < i + z[i]) j = i;
  }
  z[0] = n;
  return z;
}

/*
 * ============================================================
 * Name: Z function / extended KMP (z_algorithm, aligned with ACL)
 * Complexity: O(n) (each position extends once amortized)
 * Usage: z[i] = LCP of s and s[i..n); concatenated as "p # t" it finds p's
 *        match length at every position of t (occurrences / positions) in
 *        O(|p|+|t|)
 * Interface: z_algorithm(string) / z_algorithm(vector<T>), 0-indexed;
 *        convention z[0] = n (same as ACL; zero it yourself if you need z[0]=0)
 * Principle: maintain the rightmost Z-box [j, j+z[j]); when a new position i
 *        falls inside the box, reuse the already-computed z at s[i-j]
 *        (truncated at the box's right end), then extend naively; every
 *        extension advances the global right end, so the total cost is linear
 * Source: ported from AtCoder Library string.hpp z_algorithm (line-by-line)
 * ============================================================
 * Example (uncomment to compile):
 * signed main() {
 *   string s = "abacaba";
 *   vector<int> z = z_algorithm(s);            // {7,0,1,0,3,0,1}
 *   for (int i = 0; i < (int)z.size(); i++) cout << z[i] << " \n"[i + 1 == z.size()];
 *   // matching: occurrences of p in t (concatenate p + '\x01' + t; z == |p| means a hit)
 *   string p = "aba", t = "ababacaba";
 *   vector<int> z2 = z_algorithm(p + '\x01' + t);
 *   int cnt = 0;
 *   for (int i = (int)p.size() + 1; i < (int)z2.size(); i++) cnt += (z2[i] == (int)p.size());
 *   cout << cnt << '\n';                       // 3（t[0], t[2], t[6]）
 * }
 */
