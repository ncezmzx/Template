#include <bits/stdc++.h>
using namespace std;
#define int long long
using d64 = long double;


struct matrix {
  vector<d64> vec;
  int n, m;
  auto operator[](const int i) { return vec.begin() + i * m; }
  auto operator[](const int i) const { return vec.begin() + i * m; }
  matrix() = default;
  matrix(int _n, int _m) : n(_n), m(_m), vec(_n * _m, 0) {}
};
matrix operator*(const matrix &lhs, const matrix &rhs) {
  assert(lhs.m == rhs.n);
  matrix ret(lhs.n, rhs.m);
  for (int i = 0; i < lhs.n; ++i) {
    for (int j = 0; j < rhs.m; ++j) {
      for (int k = 0; k < lhs.m; ++k) {
        ret[i][j] += lhs[i][k] * rhs[k][j];
      }
    }
  }
  return ret;
}
matrix I(int n) {
  matrix ret(n, n);
  for (int i = 0; i < n; ++i) ret[i][i] = 1;
  return ret;
}
matrix qpow(matrix a, int b) {
  matrix r = I(a.n);
  for (; b; b >>= 1, a = a * a)
    if (b & 1) r = r * a;
  return r;
}

