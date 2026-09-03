#include <bits/stdc++.h>
using namespace std;


template <size_t N> struct segtree_lazy {
  int n;
  long long sgt[N << 2], tg[N << 2];
  void apply(int u, int l, int r, long long x) { sgt[u] += (r - l + 1) * x, tg[u] += x; }
  void down(int u, int l, int r) {
    if (tg[u]) {
      int m = (l + r) >> 1;
      apply(u << 1, l, m, tg[u]), apply(u << 1 | 1, m + 1, r, tg[u]);
      tg[u] = 0;
    }
  }
  void update(int u, int l, int r, int x, int y, int z) {
    if (x <= l && r <= y) return apply(u, l, r, z);
    int m = (l + r) >> 1;
    down(u, l, r);
    if (m >= x) update(u << 1, l, m, x, y, z);
    if (m < y) update(u << 1 | 1, m + 1, r, x, y, z);
    sgt[u] = sgt[u << 1] + sgt[u << 1 | 1];
  }

  int find(int u, int l, int r, long long k) {
    if (l == r) return l;
    int m = (l + r) >> 1;
    down(u, l, r);
    if (sgt[u << 1] >= k) return find(u << 1, l, m, k);
    else return find(u << 1 | 1, m + 1, r, k - sgt[u << 1]);
  }
  long long query(int u, int l, int r, int x, int y) {
    if (x <= l && r <= y) return sgt[u];
    int m = (l + r) >> 1;
    long long res = 0;
    down(u, l, r);
    if (m >= x) res += query(u << 1, l, m, x, y);
    if (m < y) res += query(u << 1 | 1, m + 1, r, x, y);
    return res;
  }
};

