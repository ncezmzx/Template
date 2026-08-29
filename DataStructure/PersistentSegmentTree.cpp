#include <bits/stdc++.h>
using namespace std;

// persistent segment tree over values: static range k-th smallest / count <= x
// rt[i] is the version for prefix i; space = (n + q) * ceil(log2 V)
template <size_t N, size_t SP>
struct persistent_segtree {
  int n_, V, tot, rt[N], lc[SP], rc[SP], cnt_[SP];
  vector<int> b_;  // compressed values (1..V)
  int upd(int pre, int l, int r, int x) {  // version pre with position x count +1
    int u = ++tot;
    lc[u] = lc[pre], rc[u] = rc[pre], cnt_[u] = cnt_[pre] + 1;
    if (l == r) return u;
    int m = l + r >> 1;
    x <= m ? lc[u] = upd(lc[pre], l, m, x) : rc[u] = upd(rc[pre], m + 1, r, x);
    return u;
  }
  void init(const vector<int>& a) {  // a[1..n] (1-indexed, a[0] is a placeholder)
    n_ = (int)a.size() - 1;
    b_.assign(a.begin() + 1, a.end());
    sort(b_.begin(), b_.end()), b_.erase(unique(b_.begin(), b_.end()), b_.end());
    V = (int)b_.size(), tot = 0, rt[0] = 0;
    for (int i = 1; i <= n_; ++i) rt[i] = upd(rt[i - 1], 1, V, (int)(lower_bound(b_.begin(), b_.end(), a[i]) - b_.begin()) + 1);
  }
  int query_kth(int l, int r, int k) {  // k-th smallest in [l, r] (original value)
    int u = rt[l - 1], v = rt[r], L = 1, R = V;
    while (L < R) {
      int m = L + R >> 1, c = cnt_[lc[v]] - cnt_[lc[u]];
      if (k <= c) u = lc[u], v = lc[v], R = m;
      else k -= c, u = rc[u], v = rc[v], L = m + 1;
    }
    return b_[L - 1];
  }
  int query_le(int l, int r, int x) {  // number of values <= x in [l, r]
    int u = rt[l - 1], v = rt[r], L = 1, R = V, res = 0;
    while (L < R) {
      int m = L + R >> 1;
      if (x < b_[m]) {  // x falls left: right half all > x
        u = lc[u], v = lc[v], R = m;
      } else {  // left half all <= x
        res += cnt_[lc[v]] - cnt_[lc[u]];
        u = rc[u], v = rc[v], L = m + 1;
      }
    }
    return res + (b_[L - 1] <= x ? cnt_[v] - cnt_[u] : 0);  // leaf level
  }
};

/*
 * ============================================================
 * Name: persistent segment tree (static range k-th smallest)
 * Complexity: build O(n log n); query O(log n); space O(n log V)
 * Usage: `persistent_segtree<N, SP>`: static range k-th smallest / rank / count
 *        of values <= x.
 *        init(a) (a[0] placeholder, a[1..n] data, compressed internally);
 *        query_kth(l, r, k) k-th smallest (original value); query_le(l, r, x)
 *        count of values <= x.
 * Principle: the value-domain tree is versioned by prefix: rt[i] adds a[i] on
 *            top of rt[i-1] (path cloning); subtracting two versions gives the
 *            range's value distribution
 * Notes: SP = N * 20 for N = 2e5 (log V ~ 18), adjust to the data size;
 *        positions 1-indexed; values must be compressible (duplicates ok)
 * ============================================================
 */
