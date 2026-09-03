#define int long long

struct xor_basis {
  int b[60] = {}, cnt = 0;
  void clear() { memset(b, 0, sizeof b), cnt = 0; }
  bool insert(int x) {
    for (int i = 59; i >= 0; --i)
      if (x >> i & 1) {
        if (!b[i]) return b[i] = x, ++cnt, true;
        x ^= b[i];
      }
    return false;
  }
  bool contains(int x) {
    for (int i = 59; i >= 0; --i)
      if (x >> i & 1) {
        if (!b[i]) return false;
        x ^= b[i];
      }
    return true;
  }
  int qmax(int x = 0) {
    for (int i = 59; i >= 0; --i)
      if ((x ^ b[i]) > x) x ^= b[i];
    return x;
  }
  int qmin(int x = 0) {
    for (int i = 0; i < 60; ++i)
      if ((x ^ b[i]) < x) x ^= b[i];
    return x;
  }
  void merge(const xor_basis &o) {
    for (int i = 0; i < 60; ++i)
      if (o.b[i]) insert(o.b[i]);
  }
  void normalize() {
    for (int i = 0; i < 60; ++i)
      if (b[i])
        for (int j = i - 1; j >= 0; --j)
          if (b[j] && (b[i] >> j & 1)) b[i] ^= b[j];
  }
  int qkth(int k) {
    if (k > (int)1 << cnt) return -1;
    normalize();
    int res = 0;
    for (int i = 0, j = 0; i < 60; ++i)
      if (b[i]) {
        if (k - 1 >> j & 1) res ^= b[i];
        ++j;
      }
    return res;
  }
  int cnt_le(int x) {
    int lo = 1, hi = (int)1 << cnt;
    while (lo < hi) {
      int mid = lo + hi + 1 >> 1;
      qkth(mid) <= x ? lo = mid : hi = mid - 1;
    }
    return qkth(lo) <= x ? lo : 0;
  }
};

