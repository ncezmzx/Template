#include <bits/stdc++.h>
using namespace std;


template <size_t N> struct wbst_set {
  int tp, tot, rt;
  int ch[2 * N][2], sz[2 * N], val[2 * N], stk[2 * N];
  void up(int x) { sz[x] = sz[ch[x][0]] + sz[ch[x][1]], val[x] = val[ch[x][1]]; }
  void erase(int &x) { stk[++tp] = x, x = 0; }
  int make(int x) { return ch[x][0] = ch[x][1] = val[x] = sz[x] = 0, x; }
  int make() { return make(tp ? stk[tp--] : ++tot); }
  int node(int v, int u) { return val[u] = v, sz[u] = 1, u; }
  int node(int v) { return node(v, make()); }
  int link(int x, int y, int z) { return ch[z][0] = x, ch[z][1] = y, up(z), z; }
  int link(int x, int y) { return link(x, y, make()); }
  auto cut(int x) {
    int y = ch[x][0], z = ch[x][1];
    return erase(x), make_pair(y, z);
  }
  void rotate(int &x, bool r) {
    auto [a, b] = cut(x);
    if (r) {
      auto [c, d] = cut(b);
      x = link(link(a, c), d);
    }
    else {
      auto [c, d] = cut(a);
      x = link(c, link(d, b));
    }
  }
  bool heavy(int x, int y) { return x > 3 * y; }
  bool need(int x, int r) { return sz[ch[x][!r]] > 2 * sz[ch[x][r]]; }
  void balance(int &x) {
    if (sz[x] == 1) return;
    bool r = sz[ch[x][1]] > sz[ch[x][0]];
    if (!heavy(sz[ch[x][r]], sz[ch[x][!r]])) return;
    if (need(ch[x][r], r)) rotate(ch[x][r], !r);
    rotate(x, r);
  }
  int mer(int x, int y) {
    if (!x || !y) return x + y;
    if (heavy(sz[x], sz[y])) {
      auto [a, b] = cut(x);
      int z = link(a, mer(b, y));
      return balance(z), z;
    }
    else if (heavy(sz[y], sz[x])) {
      auto [a, b] = cut(y);
      int z = link(mer(x, a), b);
      return balance(z), z;
    }
    else return link(x, y);
  }
  void ins(int &x, int v) {
    if (!x) return x = node(v), void();
    else if (sz[x] == 1) ch[x][0] = node(min(v, val[x])), ch[x][1] = node(max(v, val[x]));
    else ins(ch[x][v > val[ch[x][0]]], v);
    up(x), balance(x);
  }
  void remove(int &x, int v) {
    if (!x) return;
    if (sz[x] == 1) return erase(x);
    else {
      bool r = v > val[ch[x][0]];
      remove(ch[x][r], v);
      if (!ch[x][r]) x = ch[x][!r];
      else up(x), balance(x);
    }
  }
  int rnk(int x, int v) {
    if (!x) return 0;
    int res = 0;
    while (sz[x] > 1) {
      if (val[ch[x][0]] < v) res += sz[ch[x][0]], x = ch[x][1];
      else x = ch[x][0];
    }
    return res + (val[x] < v);
  }
  int kth(int x, int k) {
    while (sz[x] > 1) {
      if (sz[ch[x][0]] < k) k -= sz[ch[x][0]], x = ch[x][1];
      else x = ch[x][0];
    }
    return val[x];
  }
};

