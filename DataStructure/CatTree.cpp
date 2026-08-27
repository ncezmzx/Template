#include <bits/stdc++.h>
using namespace std;
template<class T, size_t N> struct cattree {
  T ct[__lg(N) + 2][N];
  void build(int n, const T a[]) {
    for (int h = 0, s = 1; s <= n; ++h, s <<= 1) {
      for (int i = s; i <= n; i += s << 1) {
        ct[h][i - 1] = a[i - 1];
        for (int j = i - 1; j > i - s; --j) ct[h][j - 1] = a[j - 1] + ct[h][j];
        if (i == n) break;
        ct[h][i] = a[i];
        for (int j = i + 1; j < i + s && j < n; ++j)
          ct[h][j] = ct[h][j - 1] + a[j];
      }
    }
  }
  T query(int l, int r) {
    if (l == r) return ct[0][l];
    int h = __lg(l ^ r);
    return ct[h][l] + ct[h][r];
  }
};
/*
 * ============================================================
 * 名称：猫树（Cat Tree，静态 O(1) 区间可结合查询）
 * 复杂度：预处理 O(n log n)，查询 O(1)
 * 用途：静态数组、查询运算满足结合律（+、max、min、gcd、xor 等，用 operator+ 表达）的区间查询
 * 来源：all.cpp 第 814-833 行（原样保留；注释已统一移至文件尾部）
 * ============================================================
 * 使用示例（编译时取消注释）：
 *   signed main() {
 *     cin.tie(nullptr)->sync_with_stdio(false);
 *     int a[6] = {1, 3, 5, 2, 4, 6};
 *     cattree<int, 100> ct;
 *     ct.build(6, a);
 *     cout << ct.query(1, 3) << '\n';  // 3+5+2 = 10
 *     cout << ct.query(0, 5) << '\n';  // 21
 *     cout << ct.query(2, 2) << '\n';  // 5
 *     return 0;
 *   }
 * ============================================================
 */
