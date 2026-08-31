#include <bits/stdc++.h>
using namespace std;

// z[i] = LCP of s and s[i..]; z[0] = |s| (same convention as ACL z_algorithm)
vector<int> z_algorithm(const string &s) {
  int n = (int)s.size();
  if (n == 0) return {};
  vector<int> z(n);
  z[0] = 0;
  for (int i = 1, j = 0; i < n; i++) { // j = start of the rightmost Z-box
    int &k = z[i];
    k = (j + z[j] <= i) ? 0 : min(j + z[j] - i, z[i - j]); // reuse inside the Z-box
    while (i + k < n && s[k] == s[i + k]) k++;             // brute-force extension
    if (j + z[j] < i + z[i]) j = i;
  }
  z[0] = n;
  return z;
}

// generic container version (same template signature as ACL; works for vector<int> etc.)
template <class T> vector<int> z_algorithm(const vector<T> &s) {
  int n = (int)s.size();
  if (n == 0) return {};
  vector<int> z(n);
  z[0] = 0;
  for (int i = 1, j = 0; i < n; i++) {
    int &k = z[i];
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
 *        match length at every position of t (occurrences / positions) in O(|p|
 *        + |t|).
 *        Method list: see Interface below.
 * Interface: z_algorithm(string) / z_algorithm(vector<T>), 0-indexed;
 *        convention z[0] = n (same as ACL; zero it yourself if you need z[0]=0)
 * Principle: maintain the rightmost Z-box [j, j+z[j]); when a new position i
 *            falls inside the box, reuse the already-computed z at s[i-j]
 *            (truncated at the box's right end) and then extend naively; every
 *            extension advances the global right end, so the total cost is
 *            linear
 * Source: ported from AtCoder Library string.hpp z_algorithm (line-by-line)
 * Notes: 0-indexed; the convention is z[0] = n (same as ACL, so zero it
 *        yourself if you need z[0] = 0)
 * ============================================================
 */
