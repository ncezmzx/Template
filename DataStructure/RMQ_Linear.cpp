#include <bits/stdc++.h>
using namespace std;

// static RMQ (FastST): Z-sized blocks queried through a Cartesian-tree bitmask,
// block extrema answered by a U-level sparse table
template <size_t U = 20, size_t Z = 32, class T = int>
struct FastST {
  static_assert(Z && (Z & (Z - 1)) == 0, "Z must be a power of two");
  static_assert(Z <= sizeof(size_t) * 8, "Z must fit in one size_t bitmask");
  int n;
  vector<size_t> f;
  array<vector<T>, U> st;
  vector<T> pr, sf, arr;
  function<bool(T, T)> comp;
  void set(const function<bool(T, T)> &cmp) { comp = cmp; }
  T get(const T &x, const T &y) { return comp(x, y) ? x : y; }
  void build(vector<T> &vec, int sz) {  // vec is 1-indexed (vec[0] unused); it is consumed
    n = sz;
    f.assign(n + 1, 0);
    arr.swap(vec), pr = sf = arr;
    for (int i = 0; i < U; ++i) st[i].assign(n / Z + 1, T());
    for (int i = 1; i <= n; ++i) {
      int b = i / Z;
      // i == 1 or i % Z == 0 opens a block: seed it instead of folding into T()
      st[0][b] = (i == 1 || i % Z == 0) ? arr[i] : get(st[0][b], arr[i]);
      if (i > 1 && i / Z == (i - 1) / Z) pr[i] = get(pr[i - 1], pr[i]);
    }
    for (int i = n - 1; i >= 1; --i)
      if ((i + 1) / Z == i / Z) sf[i] = get(sf[i + 1], sf[i]);
    for (int i = 1; i < U; ++i)
      for (int j = 1; j + (1 << i) - 1 <= n / Z; ++j)
        st[i][j] = get(st[i - 1][j], st[i - 1][j + (1 << (i - 1))]);
    vector<int> stk(Z + 1);  // monotonic stack, reset at every block boundary
    int tp = 0;
    for (int i = 1; i <= n; ++i) {
      if (i / Z != (i - 1) / Z) tp = 0;
      else f[i] = f[i - 1];
      while (tp && comp(arr[i], arr[stk[tp]])) f[i] &= ~(size_t(1) << (stk[tp--] & (Z - 1)));
      stk[++tp] = i, f[i] |= size_t(1) << (i & (Z - 1));
    }
  }
  T query(int l, int r) {  // extremum over [l, r], 1 <= l <= r <= n
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

/*
 * ============================================================
 * Name: static RMQ (FastST: in-block bitmask stack + sparse table over blocks)
 * Complexity: build O(n + (n/Z) log(n/Z)); query O(1) (one ctz, or two table
 *             lookups); space O(n + U * n/Z)
 * Usage: `FastST<U, Z, T>`: set(cmp), then build(vec, n) with a 1-indexed vec
 *        (vec[0] unused); query(l, r) returns the extremum *value*, not its
 *        position. cmp(a, b) means "a beats b": std::less for a range minimum,
 *        std::greater for a range maximum; ties keep the leftmost extremum.
 * Principle: blocks of Z elements. Inside a block a monotonic stack encodes the
 *            Cartesian-tree ancestor chain of each position as a Z-bit mask
 *            f[i], so an in-block query is l + ctz(f[r] >> (l mod Z)); pr / sf
 *            hold per-block prefix / suffix extrema and a U-level sparse table
 *            covers the whole blocks strictly between them.
 * Notes: build() consumes vec — it is swapped out, so pass a copy if you still
 *        need the data;
 *        set(cmp) must be called before build(), otherwise comp throws
 *        std::bad_function_call (cmp is a std::function, so it is not inlined);
 *        Z must be a power of two and at most 64; U must satisfy n/Z < 2^U
 *        (U = 20, Z = 32 covers n up to ~3.4e7);
 *        no identity element is needed — only a strict weak ordering — so this
 *        also works for extrema over struct types; the array is static, there
 *        are no updates.
 * ============================================================
 */
