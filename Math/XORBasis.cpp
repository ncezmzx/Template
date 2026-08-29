#include <bits/stdc++.h>
using namespace std;
#define int long long

// 60-bit xor linear basis; O(60) per operation
struct xor_basis {
  int b[60] = {}, cnt = 0;  // b[i] = basis vector with highest bit i
  void clear() { memset(b, 0, sizeof b), cnt = 0; }
  bool insert(int x) {  // insert x; true if the rank increased
    for (int i = 59; i >= 0; --i)
      if (x >> i & 1) {
        if (!b[i]) return b[i] = x, ++cnt, true;
        x ^= b[i];
      }
    return false;
  }
  bool contains(int x) {  // is x representable as an xor of basis vectors
    for (int i = 59; i >= 0; --i)
      if (x >> i & 1) {
        if (!b[i]) return false;
        x ^= b[i];
      }
    return true;
  }
  int qmax(int x = 0) {  // max of x xor (subset xor)
    for (int i = 59; i >= 0; --i)
      if ((x ^ b[i]) > x) x ^= b[i];
    return x;
  }
  int qmin(int x = 0) {  // min of x xor (subset xor)
    for (int i = 0; i < 60; ++i)
      if ((x ^ b[i]) < x) x ^= b[i];
    return x;
  }
  void merge(const xor_basis& o) {
    for (int i = 0; i < 60; ++i)
      if (o.b[i]) insert(o.b[i]);
  }
  void normalize() {  // eliminate lower basis bits from each b[i] (needed by qkth)
    for (int i = 0; i < 60; ++i)
      if (b[i])
        for (int j = i - 1; j >= 0; --j)
          if (b[j] && (b[i] >> j & 1)) b[i] ^= b[j];
  }
  int qkth(int k) {  // k-th smallest subset xor (1-indexed, 0 included)
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
  int cnt_le(int x) {  // number of subset xors <= x (binary search on qkth)
    int lo = 1, hi = (int)1 << cnt;  // rightmost rank with value <= x
    while (lo < hi) {
      int mid = lo + hi + 1 >> 1;
      qkth(mid) <= x ? lo = mid : hi = mid - 1;
    }
    return qkth(lo) <= x ? lo : 0;
  }
};

/*
 * ============================================================
 * Name: xor linear basis (60-bit xor space)
 * Complexity: insert / query O(60); merge O(60^2); qkth / cnt_le O(60^2)
 * Usage: maintain the subset-xor value set of a number collection, `xor_basis`:
 *        insert; contains (representability test); qmax / qmin extrema;
 *        qkth k-th smallest (0 included); cnt_le count of subset xors <= x;
 *        merge two bases; clear.
 * Principle: Gaussian elimination: each basis vector b[i] owns the highest bit
 *            i; queries are greedy from the top (qmax) or eliminate low bits
 *            (qmin); qkth first normalizes to reduced form (no 1s on lower
 *            basis bits), then the binary bits of k-1 by basis index decide
 *            which vectors to take
 * Notes: inserted values must be < 2^60; cnt is the rank, so 2^cnt values are
 *        representable including 0
 * ============================================================
 */
