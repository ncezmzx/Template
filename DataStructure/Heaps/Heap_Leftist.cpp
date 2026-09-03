
template <size_t N> struct heap_leftist {
  int fa[N], vl[N], d[N], ls[N], rs[N];
  int newnode(int x, int i) { return vl[i] = x, i; }
  int top(int x) { return vl[x]; }
  void join(int &x, int y) {
    if (!x || !y) return x |= y, void();
    if (vl[x] > vl[y]) swap(x, y);
    join(rs[x], y), fa[rs[x]] = x;
    if (d[ls[x]] > d[rs[x]]) swap(ls[x], rs[x]);
    d[x] = d[rs[x]] + 1;
  }
  void erase(int &h, int x) {
    int f = fa[x], l = ls[x], r = rs[x];
    int &s = f ? ls[f] == x ? ls[f] : rs[f] : h;
    join(s = l, r), fa[s] = f;
    while (f) {
      if (d[ls[f]] > d[rs[f]]) swap(ls[f], rs[f]);
      int nd = d[rs[f]] + 1;
      if (d[f] == nd) break;
      d[f] = nd, f = fa[f];
    }
  }
  void decrease_key(int &h, int p, int v) { erase(h, p), vl[p] = v, ls[p] = rs[p] = fa[p] = d[p] = 0, join(h, p); }
};

