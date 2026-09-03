#define int long long

template <size_t N> struct heap_pairing {
  int vl[N], sn[N], bt[N], fa[N];
  int newnode(int x, int i) { return vl[i] = x, i; }
  int top(int x) { return vl[x]; }
  void join(int &x, int y) {
    if (!x || !y) return x |= y, void();
    if (vl[x] > vl[y]) swap(x, y);
    if ((bt[y] = exchange(sn[fa[y] = x], y))) fa[bt[y]] = y;
  }
  void decrease_key(int &h, int p, int v) {
    int f = fa[p], &s = f ? sn[f] == p ? sn[f] : bt[f] : h, t = bt[p];
    t && (fa[t] = f, bt[p] = 0), s = t, fa[p] = 0, vl[p] = v, join(h, p);
  }
  int mgsn(int x) {
    if (!x) return 0;
    int y = bt[x], z = mgsn(bt[y]);
    return join(x, y), join(z, x), z;
  }
  void erase(int &h, int x) {
    decrease_key(h, x, LLONG_MIN);
    h = mgsn(sn[h]), fa[h] = bt[h] = 0;
  }
};

