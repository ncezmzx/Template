#include <bits/stdc++.h>
using namespace std;
template<class S, S (*op)(S, S), S (*e)(), size_t N = 200000>
struct SqrtTree {
  int n, bsz, bn, tsz;
  S pre[static_cast<int>(sqrt(N)) + 9][static_cast<int>(sqrt(N)) + 9];
  S suf[static_cast<int>(sqrt(N)) + 9][static_cast<int>(sqrt(N)) + 9];
  S t[(static_cast<int>(sqrt(N)) + 9) << 2], a[N];

  void sqt_build(int _n, const S *arr) {
    n = _n, bsz = sqrt(n) + 1, bn = (n + bsz - 1) / bsz;
    for (int i = 1; i <= n; ++i) a[i] = arr[i];
    for (int b = 1; b <= bn; ++b) {
      int L = (b - 1) * bsz + 1, R = min(b * bsz, n);
      pre[b][1] = a[L];
      for (int i = L + 1; i <= R; ++i) pre[b][i - L + 1] = op(pre[b][i - L], a[i]);
      suf[b][R - L + 1] = a[R];
      for (int i = R - 1; i >= L; --i) suf[b][i - L + 1] = op(a[i], suf[b][i - L + 2]);
    }
    tsz = 1;
    while (tsz < bn) tsz <<= 1;
    for (int i = 1; i <= tsz * 2; ++i) t[i] = e();
    for (int i = 1; i <= bn; ++i) {
      int len = min(bsz, n - (i - 1) * bsz);
      t[i + tsz - 1] = pre[i][len];
    }
    for (int i = tsz - 1; i >= 1; --i) t[i] = op(t[i << 1], t[i << 1 | 1]);
  }

  S tquery(int p, int l, int r, int x, int y) {
    if (x <= l && r <= y) return t[p];
    int m = (l + r) >> 1;
    S res = e();
    if (x <= m) res = op(res, tquery(p << 1, l, m, x, y));
    if (m < y) res = op(res, tquery(p << 1 | 1, m + 1, r, x, y));
    return res;
  }

  S sqt_query(int l, int r) {
    int bl = (l - 1) / bsz + 1, br = (r - 1) / bsz + 1;
    if (bl == br) {
      S res = e();
      for (int i = l; i <= r; ++i) res = op(res, a[i]);
      return res;
    }
    S res = suf[bl][l - (bl - 1) * bsz];
    if (bl + 1 <= br - 1) res = op(res, tquery(1, 1, tsz, bl + 1, br - 1));
    return op(res, pre[br][r - (br - 1) * bsz]);
  }
};

/*
 * ============================================================
 * 名称：Sqrt Tree（静态区间最值/可结合查询）
 * 复杂度：预处理 O(n log log n)（块前缀后缀和 + 块间递归线段树），查询 O(log log n)
 * 用途：静态数组区间可结合查询（min/max/gcd/sum 等，qop 定义结合律运算），
 *       查询常数小于线段树（log log n 且无递归跳转）；本实现为"分块前缀/
 *       后缀和 + 块间线段树"的两层版（块间再分块可到 O(1)，见 OI-wiki）
 * 原理：分块（块长 ~√n）：每块预处理块内前缀/后缀聚合；块间聚合用线段树
 *       维护（块数 ~√n）；区间查询 = 左块后缀 + 中间整块线段树 + 右块前缀
 * 注意：qop 需满足结合律；qe() 为幺元；本文件为静态版（无修改），
 *       支持修改需按块重建（块长 √n 时单次修改 O(√n) 重建整块）
 * ============================================================
 * 使用示例（编译时取消注释；区间最小值）：
 * constexpr int N = 2e5 + 9;
 * int a[N];
 * int op(int x, int y) { return min(x, y); }
 * int e() { return 0x3f3f3f3f; }
 * SqrtTree<int, op, e, N> s;
 * signed main() {
 *   cin >> n;
 *   for (int i = 1; i <= n; ++i) cin >> a[i];
 *   s.sqt_build();
 *   int q;
 *   cin >> q;
 *   while (q--) {
 *     int l, r;
 *     cin >> l >> r;
 *     cout << s.sqt_query(l, r) << '\n';
 *   }
 * }
 * ============================================================
 */
