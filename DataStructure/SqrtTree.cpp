#include <bits/stdc++.h>
using namespace std;


template <class S, S (*op)(S, S), S (*e)(), size_t N = 200000> struct SqrtTree {
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

