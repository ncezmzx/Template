
template <class S, S (*e)(), class F, F (*id)()>
struct lazy_segtree {
  lazy_segtree() : lazy_segtree(0) {}
  explicit lazy_segtree(int n_) : lazy_segtree(vector<S>(n_, e())) {}
  explicit lazy_segtree(const vector<S> &v) : _n((int)v.size()) {
    size = 1, log = 0;
    while (size < _n) size <<= 1, ++log;
    d = vector<S>(2 * size, e());
    lz = vector<F>(size, id());
    for (int i = 0; i < _n; i++) d[size + i] = v[i];
    for (int i = size - 1; i >= 1; i--) update(i);
  }

  void set(int p, S x) {
    p += size;
    for (int i = log; i >= 1; i--) push(p >> i);
    d[p] = x;
    for (int i = 1; i <= log; i++) update(p >> i);
  }
  S get(int p) {
    p += size;
    for (int i = log; i >= 1; i--) push(p >> i);
    return d[p];
  }
  S prod(int l, int r) {
    if (l == r) return e();
    l += size, r += size;
    for (int i = log; i >= 1; i--) {
      if (((l >> i) << i) != l) push(l >> i);
      if (((r >> i) << i) != r) push((r - 1) >> i);
    }
    S sml = e(), smr = e();
    while (l < r) {
      if (l & 1) sml = sml + d[l++];
      if (r & 1) smr = d[--r] + smr;
      l >>= 1, r >>= 1;
    }
    return sml + smr;
  }
  S all_prod() { return d[1]; }

  void apply(int p, F f) {
    p += size;
    for (int i = log; i >= 1; i--) push(p >> i);
    d[p] = d[p] + f;
    for (int i = 1; i <= log; i++) update(p >> i);
  }
  void apply(int l, int r, F f) {
    if (l == r) return;
    l += size, r += size;
    for (int i = log; i >= 1; i--) {
      if (((l >> i) << i) != l) push(l >> i);
      if (((r >> i) << i) != r) push((r - 1) >> i);
    }
    int l2 = l, r2 = r;
    while (l < r) {
      if (l & 1) all_apply(l++, f);
      if (r & 1) all_apply(--r, f);
      l >>= 1, r >>= 1;
    }
    l = l2, r = r2;
    for (int i = 1; i <= log; i++) {
      if (((l >> i) << i) != l) update(l >> i);
      if (((r >> i) << i) != r) update((r - 1) >> i);
    }
  }

  template <bool (*g)(S)> int max_right(int l) {
    return max_right(l, [](S x) { return g(x); });
  }
  template <class G>
  int max_right(int l, G g) {
    if (l == _n) return _n;
    l += size;
    for (int i = log; i >= 1; i--) push(l >> i);
    S sm = e();
    do {
      while (l % 2 == 0) l >>= 1;
      if (!g(sm + d[l])) {
        while (l < size) {
          push(l);
          l = 2 * l;
          if (g(sm + d[l])) sm = sm + d[l], l++;
        }
        return l - size;
      }
      sm = sm + d[l];
      l++;
    } while ((l & -l) != l);
    return _n;
  }

  template <bool (*g)(S)> int min_left(int r) {
    return min_left(r, [](S x) { return g(x); });
  }
  template <class G>
  int min_left(int r, G g) {
    if (r == 0) return 0;
    r += size;
    for (int i = log; i >= 1; i--) push((r - 1) >> i);
    S sm = e();
    do {
      r--;
      while (r > 1 && (r % 2)) r >>= 1;
      if (!g(d[r] + sm)) {
        while (r < size) {
          push(r);
          r = 2 * r + 1;
          if (g(d[r] + sm)) sm = d[r] + sm, r--;
        }
        return r + 1 - size;
      }
      sm = d[r] + sm;
    } while ((r & -r) != r);
    return 0;
  }

  private:
  int _n, size, log;
  vector<S> d;
  vector<F> lz;

  void update(int k) { d[k] = d[2 * k] + d[2 * k + 1]; }
  void all_apply(int k, F f) {
    d[k] = d[k] + f;
    if (k < size) lz[k] = lz[k] + f;
  }
  void push(int k) {
    all_apply(2 * k, lz[k]);
    all_apply(2 * k + 1, lz[k]);
    lz[k] = id();
  }
};

