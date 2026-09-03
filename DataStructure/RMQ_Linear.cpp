
template <size_t U = 20, size_t Z = 32, class T = int> struct FastST {
  static_assert(Z && (Z & (Z - 1)) == 0, "Z must be a power of two");
  static_assert(Z <= sizeof(size_t) * 8, "Z must fit in one size_t bitmask");
  int n;
  vector<size_t> f;
  array<vector<T>, U> st;
  vector<T> pr, sf, arr;
  function<bool(T, T)> comp;
  void set(const function<bool(T, T)> &cmp) { comp = cmp; }
  T get(const T &x, const T &y) { return comp(x, y) ? x : y; }
  void build(vector<T> &vec, int sz) {
    n = sz;
    f.assign(n + 1, 0);
    arr.swap(vec), pr = sf = arr;
    for (int i = 0; i < U; ++i) st[i].assign(n / Z + 1, T());
    for (int i = 1; i <= n; ++i) {
      int b = i / Z;

      st[0][b] = (i == 1 || i % Z == 0) ? arr[i] : get(st[0][b], arr[i]);
      if (i > 1 && i / Z == (i - 1) / Z) pr[i] = get(pr[i - 1], pr[i]);
    }
    for (int i = n - 1; i >= 1; --i)
      if ((i + 1) / Z == i / Z) sf[i] = get(sf[i + 1], sf[i]);
    for (int i = 1; i < U; ++i)
      for (int j = 1; j + (1 << i) - 1 <= n / Z; ++j) st[i][j] = get(st[i - 1][j], st[i - 1][j + (1 << (i - 1))]);
    vector<int> stk(Z + 1);
    int tp = 0;
    for (int i = 1; i <= n; ++i) {
      if (i / Z != (i - 1) / Z) tp = 0;
      else f[i] = f[i - 1];
      while (tp && comp(arr[i], arr[stk[tp]])) f[i] &= ~(size_t(1) << (stk[tp--] & (Z - 1)));
      stk[++tp] = i, f[i] |= size_t(1) << (i & (Z - 1));
    }
  }
  T query(int l, int r) {
    if (l / Z == r / Z) return arr[l + __builtin_ctzll(f[r] >> (l & (Z - 1)))];
    T ret = get(pr[r], sf[l]);
    l /= Z, r /= Z, ++l, --r;
    if (l <= r) {
      int d = __lg(r - l + 1);
      ret = get(ret, get(st[d][l], st[d][r - (1 << d) + 1]));
    }
    return ret;
  }
};

