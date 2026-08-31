#include <bits/stdc++.h>
using namespace std;

// persistent balanced tree (fully persistent fhq-Treap):
// insert/erase return a new root; every historical version stays queryable
template <size_t N> struct persistent_treap {
  int lc[N], rc[N], sz[N], pri[N], tot;
  long long val[N];
  unsigned long long sd = 88172645463325252ull; // fixed seed (swap for chrono anti-hack)
  unsigned long long rng() {
    unsigned long long x = (sd += 0x9e3779b97f4a7c15ull);
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ull;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebull;
    return x ^ (x >> 31);
  }
  int newnode(long long x) {
    int u = ++tot;
    lc[u] = rc[u] = 0, sz[u] = 1, val[u] = x, pri[u] = (int)(rng() & 0x3fffffff);
    return u;
  }
  void upd(int u) { sz[u] = sz[lc[u]] + sz[rc[u]] + 1; }
  // split by value: <= k goes to a, > k to b (clones along the way, original untouched)
  void split(int u, long long k, int &a, int &b) {
    if (!u) {
      a = b = 0;
      return;
    }
    int v = ++tot;
    lc[v] = lc[u], rc[v] = rc[u], val[v] = val[u], pri[v] = pri[u], sz[v] = sz[u];
    if (val[v] <= k) split(rc[v], k, rc[v], b), a = v;
    else split(lc[v], k, a, lc[v]), b = v;
    upd(v);
  }
  int merge(int a, int b) {
    if (!a || !b) return a | b;
    int u = ++tot;
    if (pri[a] < pri[b]) {
      lc[u] = lc[a], rc[u] = rc[a], val[u] = val[a], pri[u] = pri[a], sz[u] = sz[a];
      rc[u] = merge(rc[a], b);
    }
    else {
      lc[u] = lc[b], rc[u] = rc[b], val[u] = val[b], pri[u] = pri[b], sz[u] = sz[b];
      lc[u] = merge(a, lc[b]);
    }
    upd(u);
    return u;
  }
  int insert(int rt, long long x) { // insert one x, new root (multiset)
    int a, b;
    split(rt, x, a, b);
    return merge(merge(a, newnode(x)), b);
  }
  int erase(int rt, long long x) { // erase one x (no-op if absent), new root
    int a, b, c;
    split(rt, x, a, b);
    split(a, x - 1, a, c);
    c = merge(lc[c], rc[c]); // drop the root of c (exactly one x)
    return merge(merge(a, c), b);
  }
  long long kth(int rt, int k) { // k-th smallest (1-indexed); LLONG_MIN if out of range
    int u = rt;
    while (u) {
      if (k <= sz[lc[u]]) u = lc[u];
      else if (k == sz[lc[u]] + 1) return val[u];
      else k -= sz[lc[u]] + 1, u = rc[u];
    }
    return LLONG_MIN;
  }
  int rnk(int rt, long long x) { // count of elements < x (allocates nothing)
    int u = rt, res = 0;
    while (u) {
      if (val[u] < x) res += sz[lc[u]] + 1, u = rc[u];
      else u = lc[u];
    }
    return res;
  }
  long long pre(int rt, long long x) { // strict predecessor; LLONG_MIN if none
    int u = rt;
    long long res = LLONG_MIN;
    while (u) {
      if (val[u] < x) res = val[u], u = rc[u];
      else u = lc[u];
    }
    return res;
  }
  long long nxt(int rt, long long x) { // strict successor; LLONG_MAX if none
    int u = rt;
    long long res = LLONG_MAX;
    while (u) {
      if (val[u] > x) res = val[u], u = lc[u];
      else u = rc[u];
    }
    return res;
  }
};

/*
 * ============================================================
 * Name: persistent balanced tree (fully persistent fhq-Treap)
 * Complexity: insert / erase / kth / rank / predecessor / successor O(log n)
 * Usage: `persistent_treap<N>`: ordered multiset keeping every historical
 *        version.
 *        insert(rt, x) / erase(rt, x) -> new root;
 *        kth(rt, k), rnk(rt, x), pre(rt, x), nxt(rt, x) are pure queries
 *        (allocate nothing); "rollback to version k" = use its root.
 * Principle: split / merge are copy-on-write: every visited node is cloned
 *            first, so structures referenced by old roots are never mutated;
 *            random priorities keep balance
 * Notes: N ~ (n + m) * 2 log n (~4e6 for 1e5 ops); erase removes only one copy;
 *        kth returns LLONG_MIN when out of range
 * Source: OI-Wiki "Persistent balanced tree" (https://oi-wiki.org/ds/persistent-balanced/)
 * ============================================================
 */
