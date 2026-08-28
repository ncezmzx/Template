#include <bits/stdc++.h>
using namespace std;
#define int long long
constexpr int inf = 0x3f3f3f3f3f3f3f3f;
struct matrix {
  vector<int> vec;
  int n, m;
  auto operator[](const int i) { return vec.begin() + i * m; }
  auto operator[](const int i) const { return vec.begin() + i * m; }
  matrix() = default;
  matrix(int n_, int m_) : n(n_), m(m_), vec(n_ * m_, -inf) {}
};
matrix operator*(const matrix &lhs, const matrix &rhs) {
  matrix ret(lhs.n, rhs.m);
  // Floyd-style i-k-j loop + cached row pointers: sequential access, no per-element operator[]
  for (int i = 0; i < lhs.n; ++i) {
    const int *lk = lhs.vec.data() + i * lhs.m;
    int *ri = ret.vec.data() + i * rhs.m;
    for (int k = 0; k < lhs.m; ++k) {
      int v = lk[k];
      if (v == -inf) continue;
      const int *rk = rhs.vec.data() + k * rhs.m;
      for (int j = 0; j < rhs.m; ++j) ri[j] = max(ri[j], v + rk[j]);
    }
  }
  return ret;
}
matrix qpow(matrix a, int b) {
  matrix ret = a; --b;
  for (; b > 0; b >>= 1, a = a * a)
    if (b & 1) ret = ret * a;
  return ret;
}
/*
 * ============================================================
 * ============================================================
 * Name: max-plus matrix fast exponentiation (generalized (max, +) matrices)
 * Complexity: multiplication O(n^3), fast power O(n^3 log b)
 * Usage: Floyd-style (max,+) path problems — maximum-weight paths of exactly
 *        k steps, longest-path counting, etc.
 *        Note: qpow requires b >= 1 (implemented as ret = a first, then --b).
 * Source: all.cpp lines 11324-11348 (kept verbatim, comments translated)
 * ============================================================
 * Example (uncomment to compile):

 * signed main() {
 *   matrix a(2, 2);
 *   a[0][0] = 1; a[0][1] = 2; a[1][0] = 3; a[1][1] = 4;
 *   matrix b = qpow(a, 2);  // (max,+) square: max_k a[i][k] + a[k][j]
 *   cout << b[0][0] << ' ' << b[0][1] << ' ' << b[1][0] << ' ' << b[1][1] << '\n';
 * }
 * ============================================================
 */
