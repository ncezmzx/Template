#include <bits/stdc++.h>
using namespace std;


template <size_t N> struct heap_weight_leftist {
  int fa[N], vl[N], sz[N], ls[N], rs[N];
  void cmin(int &x, int y) { x > y && (x = y); }
  int newnode(int x, int i) { return vl[i] = x, sz[i] = 1, i; }
  int top(int x) { return vl[x]; }
  void join(int &x, int y) {
    if (!x || !y) return x |= y, void();
    if (vl[x] > vl[y]) swap(x, y);
    int &s = sz[ls[x]] > sz[rs[x]] ? rs[x] : ls[x];
    sz[x] += sz[y], join(s, y), fa[s] = x;
  }
  void erase(int &h, int x) {
    int f = fa[x], l = ls[x], r = rs[x];
    int &s = f ? ls[f] == x ? ls[f] : rs[f] : h;
    join(s = l, r), fa[s] = f;
  }
  void decrease_key(int &h, int p, int v) { erase(h, p), vl[p] = v, ls[p] = rs[p] = fa[p] = 0, sz[p] = 1, join(h, p); }
};

