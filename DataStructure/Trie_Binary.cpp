#include <bits/stdc++.h>
using namespace std;

// persistent 01-Trie: versioned by insertion order; xor extrema / rank over a
// version range. Trick: max xor-subarray in [l,r] = max s[r] ^ s[p] with
// p in [l-1, r-1], s = prefix xor
template <size_t N, size_t SP>
struct persistent_binary_trie {
  static constexpr int B = 24;  // bit width (values < 2^B; change to 30 / 62 as needed)
  int tot, cnt_, rt[N], tr_[SP][2], sz[SP];
  void init() { tot = 0, cnt_ = 0, rt[0] = 0, tr_[0][0] = tr_[0][1] = 0, sz[0] = 0; }
  int insert(int x) {  // append x (0 <= x < 2^B), returns the new version id
    int pre = rt[cnt_], u = ++tot;
    rt[++cnt_] = u;
    tr_[u][0] = tr_[pre][0], tr_[u][1] = tr_[pre][1], sz[u] = sz[pre] + 1;
    for (int cur = u, i = B - 1; i >= 0; --i) {
      int b = x >> i & 1, nxt = ++tot;
      pre = tr_[pre][b];
      tr_[nxt][0] = tr_[pre][0], tr_[nxt][1] = tr_[pre][1], sz[nxt] = sz[pre] + 1;
      tr_[cur][b] = nxt, cur = nxt;
    }
    return cnt_;
  }
  // pick y among insertions l..r maximizing x ^ y (range must be non-empty)
  int qmax(int l, int r, int x) {
    int u = rt[r], v = rt[l - 1], res = 0;
    for (int i = B - 1; i >= 0; --i) {
      int b = (x >> i & 1) ^ 1;  // prefer the bit opposite to x
      if (sz[tr_[u][b]] - sz[tr_[v][b]] > 0) res |= 1 << i;
      else b ^= 1;
      u = tr_[u][b], v = tr_[v][b];
    }
    return res;
  }
  // count of values <= x among insertions l..r
  int count_le(int l, int r, int x) {
    int u = rt[r], v = rt[l - 1], res = 0;
    for (int i = B - 1; i >= 0; --i) {
      int b = x >> i & 1;
      if (b) res += sz[tr_[u][0]] - sz[tr_[v][0]];  // everything with 0 here is < x
      if (sz[tr_[u][b]] - sz[tr_[v][b]] == 0) return res;  // target branch empty
      u = tr_[u][b], v = tr_[v][b];
    }
    return res + sz[u] - sz[v];  // leaf: values equal to x
  }
};

/*
 * ============================================================
 * Name: persistent 01-Trie (xor extrema / rank over a version range)
 * Complexity: insert / qmax / count_le O(B); space O(nB)
 * Usage: `persistent_binary_trie<N, SP>`: persistent 01-Trie over a multiset's
 *        insertion history.
 *        init(); insert(x) appends a version; qmax(l, r, x) max of x ^ y over
 *        insertions l..r; count_le(l, r, x) count of values <= x.
 * Principle: each version clones the insertion path (B + 1 new nodes) and
 *            shares the rest; version r minus l-1 is the set of insertions
 *            l..r; qmax greedily follows the non-empty branch opposite to x's
 *            bit
 * Notes: B must cover the highest bit (values < 2^B); the qmax range must be
 *        non-empty; no compression needed when the domain is a power of two
 * ============================================================
 */
