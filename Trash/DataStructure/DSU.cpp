#define int long long

template <size_t N> struct dsu {
  int fa[N];
  dsu() { iota(fa, fa + N, 0); }
  int get(int x) { return x == fa[x] ? x : fa[x] = get(fa[x]); }
  bool mer(int x, int y) {
    if ((x = get(x)) == (y = get(y))) return false;
    return fa[x] = y, true;
  }
};

