#include <bits/stdc++.h>
using namespace std;
#define int long long

// fhq-Treap: split/merge by size or by value, with reverse lazy tag
template <size_t N> struct treap {
  int tot, rt;
  int ch[N][2], sz[N], val[N], pr[N], tg[N];
  mt19937 rng{random_device{}()};
  int node(int v) {
    int x = ++tot;
    val[x] = v, sz[x] = 1, pr[x] = rng();
    return x;
  }
  void up(int x) { sz[x] = 1 + sz[ch[x][0]] + sz[ch[x][1]]; }
  void apply(int x) { swap(ch[x][0], ch[x][1]), tg[x] ^= 1; } // reverse tag
  void down(int x) {
    if (tg[x]) apply(ch[x][0]), apply(ch[x][1]), tg[x] = 0;
  }
  void split(int x, int k, int &a, int &b) { // by size: first k nodes -> a
    if (!x) return a = b = 0, void();
    down(x);
    if (sz[ch[x][0]] >= k) split(ch[x][0], k, a, ch[x][0]), b = x, up(x);
    else split(ch[x][1], k - sz[ch[x][0]] - 1, ch[x][1], b), a = x, up(x);
  }
  void split_v(int x, int v, int &a, int &b) { // by value: < v -> a, >= v -> b
    if (!x) return a = b = 0, void();
    down(x);
    if (val[x] < v) split_v(ch[x][1], v, ch[x][1], b), a = x, up(x);
    else split_v(ch[x][0], v, a, ch[x][0]), b = x, up(x);
  }
  int merge(int a, int b) {
    if (!a || !b) return a + b;
    if (pr[a] < pr[b]) return down(a), ch[a][1] = merge(ch[a][1], b), up(a), a;
    return down(b), ch[b][0] = merge(a, ch[b][0]), up(b), b;
  }
  void insert(int &x, int v) {
    int a, b;
    split_v(x, v, a, b);
    x = merge(merge(a, node(v)), b);
  }
  void erase(int &x, int v) { // removes ALL nodes equal to v (set semantics)
    int a, b, c;
    split_v(x, v, a, b);
    split_v(b, v + 1, c, b);
    x = merge(a, b);
  }
  int kth(int x, int k) { // k-th smallest value
    while (true) {
      down(x);
      if (sz[ch[x][0]] >= k) x = ch[x][0];
      else if (k == sz[ch[x][0]] + 1) return val[x];
      else k -= sz[ch[x][0]] + 1, x = ch[x][1];
    }
  }
  void dfs(int x) { // in-order dump
    if (!x) return;
    down(x);
    dfs(ch[x][0]), cout << val[x] << ' ', dfs(ch[x][1]);
  }
};

/*
 * ============================================================
 * Name: fhq-Treap (split/merge, no rotations)
 * Complexity: amortized O(log n) per operation (randomized priorities)
 * Usage: split / merge, no rotations; `treap<N>`, root passed by reference: rt
 *        = merge(a, b); insert(rt, v); erase(rt, v); kth(rt, k).
 *        set mode: split_v(x, v, a, b) splits < v / >= v, giving insert / erase
 *        / kth / pred / succ.
 *        sequence mode: split(x, k, a, b) takes the first k nodes; apply(x)
 *        tags a reversal (implicit treap).
 * Principle: random priority pr + heap order under merge gives expected
 *            balance; split / merge are the only primitives
 * Notes: erase removes ALL nodes equal to v (set semantics); reset tot / rt
 *        between test cases
 * ============================================================
 */
