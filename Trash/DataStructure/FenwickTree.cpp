#define int long long

template <size_t N> struct fenwick {
  int n;
  int c[N];
  void update(int x, int y) {
    for (; x <= n; x += x & -x) c[x] += y;
  }
  int query(int x, int r = 0) {
    for (; x > 0; x -= x & -x) r += c[x];
    return r;
  }
};

