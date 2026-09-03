
template <size_t N> struct wbst_seq {
  int tp, tot, rt;
  int ch[2 * N][2], sz[2 * N], val[2 * N], stk[2 * N], tg[2 * N];
  void up(int x) { sz[x] = sz[ch[x][0]] + sz[ch[x][1]], val[x] = val[ch[x][1]]; }
  void apply(int x) { tg[x] ^= 1, swap(ch[x][0], ch[x][1]); }
  void down(int x) {
    if (tg[x]) apply(ch[x][0]), apply(ch[x][1]), tg[x] = 0;
  }
  void erase(int &x) { stk[++tp] = x, x = 0; }
  int make(int x) { return ch[x][0] = ch[x][1] = val[x] = sz[x] = 0, x; }
  int make() { return make(tp ? stk[tp--] : ++tot); }
  int node(int v, int u) { return val[u] = v, sz[u] = 1, u; }
  int node(int v) { return node(v, make()); }
  int link(int x, int y, int z) { return ch[z][0] = x, ch[z][1] = y, up(z), z; }
  int link(int x, int y) { return link(x, y, make()); }
  auto cut(int x) {
    int y = ch[x][0], z = ch[x][1];
    return down(x), erase(x), make_pair(y, z);
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
    down(x);
    bool r = sz[ch[x][1]] > sz[ch[x][0]];
    if (!heavy(sz[ch[x][r]], sz[ch[x][!r]])) return;
    down(ch[x][r]);
    if (need(ch[x][r], r)) down(ch[ch[x][r]][!r]), rotate(ch[x][r], !r);
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
  auto spl(int x, int k) {
    if (!x) return make_pair(0, 0);
    if (!k) return make_pair(0, x);
    if (k == sz[x]) return make_pair(x, 0);
    auto [a, b] = cut(x);
    if (k <= sz[a]) {
      auto [c, d] = spl(a, k);
      return make_pair(c, mer(d, b));
    }
    else {
      auto [c, d] = spl(b, k - sz[a]);
      return make_pair(mer(a, c), d);
    }
  }
  int build(int l, int r) {
    if (l == r) return node(l);
    int m = (l + r) >> 1;
    return link(build(l, m), build(m + 1, r));
  }
  void print(int x) {
    if (sz[x] == 1) return cout << val[x] << ' ', void();
    down(x);
    print(ch[x][0]), print(ch[x][1]);
  }
};

