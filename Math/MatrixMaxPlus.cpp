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
  matrix ret = a;
  --b;
  for (; b > 0; b >>= 1, a = a * a)
    if (b & 1) ret = ret * a;
  return ret;
}

