#define int long long

template <size_t N> struct heap_skew {
  int fa[N], vl[N], ls[N], rs[N];
  int newnode(int x, int i) { return vl[i] = x, i; }
  int top(int x) { return vl[x]; }
  void join(int &x, int y) {
    if (!x || !y) return x |= y, void();
    if (vl[x] > vl[y]) swap(x, y);
    join(rs[x], y), fa[rs[x]] = x, swap(ls[x], rs[x]);
  }
  void decrease_key(int &h, int p, int v) {
    int f = fa[p], &s = f ? ls[f] == p ? ls[f] : rs[f] : h;
    s = fa[p] = 0, vl[p] = v, join(h, p);
  }
  void erase(int &h, int x) {
    decrease_key(h, x, LLONG_MIN);
    int l = ls[h], r = rs[h];
    join(h = l, r), fa[h] = 0;
  }
};

