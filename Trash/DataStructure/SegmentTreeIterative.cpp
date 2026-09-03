#include <bits/stdc++.h>
using namespace std;


template <size_t N> struct segtree_iterative {
  int n;
  pair<int, int> sgt[N << 1];

  void update(int x, const pair<int, int> &y) {
    sgt[x += n - 1] = y;
    for (x >>= 1; x; x >>= 1) sgt[x] = max(sgt[x << 1], sgt[x << 1 | 1]);
  }

  pair<int, int> query(int l, int r) {
    l = max(1, l), r = min(r, n);
    if (l > r) return {-1, -1};
    pair<int, int> res;
    for (l += n - 1, r += n; l ^ r; l >>= 1, r >>= 1) {
      if (l & 1) res = max(res, sgt[l++]);
      if (r & 1) res = max(res, sgt[--r]);
    }
    return res;
  }
};

