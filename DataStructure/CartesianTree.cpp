#include <bits/stdc++.h>
using namespace std;


template <size_t N> struct cartesian_tree {
  int n, a[N], lc[N], rc[N], fa[N], stk[N];
  void build(int n_) {
    n = n_;
    for (int i = 1; i <= n; ++i) lc[i] = rc[i] = fa[i] = 0;
    int tp = 0;
    for (int i = 1; i <= n; ++i) {
      int last = 0;
      while (tp && a[stk[tp]] > a[i]) last = stk[tp--];
      if (tp) rc[stk[tp]] = i, fa[i] = stk[tp];
      lc[i] = last;
      if (last) fa[last] = i;
      stk[++tp] = i;
    }
  }
};

