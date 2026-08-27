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
  // Floyd 式 i-k-j 转移 + 行指针缓存：顺序访存，免去 operator[] 的逐元素寻址
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
 * 名称：max-plus 矩阵快速幂（广义 (max, +) 矩阵）
 * 复杂度：乘法 O(n^3)，快速幂 O(n^3 log b)
 * 用途：Floyd 式（max,+）路径问题——恰走 k 步的最大权路径、最长路计数等。
 *       注意：qpow 要求 b >= 1（实现为先取 ret = a 再 --b）。
 * 来源：all.cpp 行 11324-11348（原样保留；注释已统一移至文件尾部）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   matrix a(2, 2);
 *   a[0][0] = 1; a[0][1] = 2; a[1][0] = 3; a[1][1] = 4;
 *   matrix b = qpow(a, 2);  // (max,+) 平方：max_k a[i][k] + a[k][j]
 *   cout << b[0][0] << ' ' << b[0][1] << ' ' << b[1][0] << ' ' << b[1][1] << '\n';
 * }
 * ============================================================
 */
