#define int long long

template <size_t N> struct suffix_array {
  int n, sw;
  int a[N], rk[N], sa[N], id[N], buc[N], h[N];
  bool samerank(int x, int y) {
    if (rk[x] != rk[y]) return false;
    if (sw < 0) return true;
    int px = x + sw <= n ? rk[x + sw] : 0, py = y + sw <= n ? rk[y + sw] : 0;
    return px == py;
  }
  void bucsort(int m) {
    memset(buc + 1, 0, m * sizeof(int));
    for (int i = 1; i <= n; ++i) ++buc[rk[i]];
    for (int i = 1; i <= m; ++i) buc[i] += buc[i - 1];
    for (int i = n; i >= 1; --i) sa[buc[rk[id[i]]]--] = id[i];
    id[sa[1]] = 1;
    for (int i = 2; i <= n; ++i) id[sa[i]] = id[sa[i - 1]] + !samerank(sa[i - 1], sa[i]);
    memcpy(rk + 1, id + 1, n * sizeof(int));
  }

  void build() {
    for (int i = 1; i <= n; ++i) rk[i] = a[i], sa[i] = id[i] = i;
    sw = -1;
    bucsort(n);
    for (int w = 1, p = 0, m = rk[sa[n]]; w < n; w <<= 1, m = rk[sa[n]], p = 0) {
      for (int j = n - w + 1; j <= n; ++j) id[++p] = j;
      for (int j = 1; j <= n; ++j)
        if (sa[j] > w) id[++p] = sa[j] - w;
      sw = w;
      bucsort(m);
      if (rk[sa[n]] == n) break;
    }
  }
  void build_height() {
    for (int i = 1, k = 0; i <= n; ++i) {
      if (k) --k;
      while (a[i + k] == a[sa[rk[i] - 1] + k]) ++k;
      h[rk[i]] = k;
    }
  }
};

