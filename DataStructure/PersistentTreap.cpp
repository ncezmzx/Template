#include <bits/stdc++.h>
using namespace std;


template <size_t N> struct persistent_treap {
  int lc[N], rc[N], sz[N], pri[N], tot;
  long long val[N];
  unsigned long long sd = 88172645463325252ull;
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
  int insert(int rt, long long x) {
    int a, b;
    split(rt, x, a, b);
    return merge(merge(a, newnode(x)), b);
  }
  int erase(int rt, long long x) {
    int a, b, c;
    split(rt, x, a, b);
    split(a, x - 1, a, c);
    c = merge(lc[c], rc[c]);
    return merge(merge(a, c), b);
  }
  long long kth(int rt, int k) {
    int u = rt;
    while (u) {
      if (k <= sz[lc[u]]) u = lc[u];
      else if (k == sz[lc[u]] + 1) return val[u];
      else k -= sz[lc[u]] + 1, u = rc[u];
    }
    return LLONG_MIN;
  }
  int rnk(int rt, long long x) {
    int u = rt, res = 0;
    while (u) {
      if (val[u] < x) res += sz[lc[u]] + 1, u = rc[u];
      else u = lc[u];
    }
    return res;
  }
  long long pre(int rt, long long x) {
    int u = rt;
    long long res = LLONG_MIN;
    while (u) {
      if (val[u] < x) res = val[u], u = rc[u];
      else u = lc[u];
    }
    return res;
  }
  long long nxt(int rt, long long x) {
    int u = rt;
    long long res = LLONG_MAX;
    while (u) {
      if (val[u] > x) res = val[u], u = lc[u];
      else u = rc[u];
    }
    return res;
  }
};

