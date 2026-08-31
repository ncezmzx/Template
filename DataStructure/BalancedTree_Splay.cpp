#include <bits/stdc++.h>
using namespace std;

// Splay tree for sequence maintenance (range reverse, etc.)
template <size_t N> struct splay {
  int rt, tot;
  int ch[N][2], fa[N], sz[N], val[N], tg[N];
  void up(int x) { sz[x] = 1 + sz[ch[x][0]] + sz[ch[x][1]]; }
  void apply(int x) { swap(ch[x][0], ch[x][1]), tg[x] ^= 1; }
  void down(int x) {
    if (tg[x]) apply(ch[x][0]), apply(ch[x][1]), tg[x] = 0;
  }
  void rotate(int x) {
    int y = fa[x], z = fa[y], k = ch[y][1] == x;
    if (z) ch[z][ch[z][1] == y] = x;
    fa[x] = z;
    ch[y][k] = ch[x][k ^ 1];
    if (ch[x][k ^ 1]) fa[ch[x][k ^ 1]] = y;
    ch[x][k ^ 1] = y, fa[y] = x;
    up(y), up(x);
  }
  void splay_(int x, int goal) {
    while (fa[x] != goal) {
      int y = fa[x], z = fa[y];
      if (z != goal) rotate((ch[y][1] == x) ^ (ch[z][1] == y) ? x : y);
      rotate(x);
    }
    if (!goal) rt = x;
  }

  int build(int l, int r) {
    if (l > r) return 0;
    int m = (l + r) >> 1, x = ++tot;
    val[x] = m, sz[x] = 1;
    if (l < m) ch[x][0] = build(l, m - 1), fa[ch[x][0]] = x;
    if (m < r) ch[x][1] = build(m + 1, r), fa[ch[x][1]] = x;
    up(x);
    return x;
  }
  int kth(int k) {
    int x = rt;
    while (true) {
      down(x);
      if (sz[ch[x][0]] >= k) x = ch[x][0];
      else if (k == sz[ch[x][0]] + 1) return x;
      else k -= sz[ch[x][0]] + 1, x = ch[x][1];
    }
  }
  void reverse(int l, int r) {
    int L = kth(l), R = kth(r + 2);
    splay_(L, 0), splay_(R, L);
    apply(ch[R][0]);
  }
  void print(int x) {
    if (!x) return;
    down(x);
    print(ch[x][0]), cout << val[x] << ' ', print(ch[x][1]);
  }
};

/*
 * ============================================================
 * Name: splay tree (sequence operations: range reverse etc.)
 * Complexity: amortized O(log n) per operation
 * Usage: sequence maintenance (range reverse / insert / delete / move):
 *        `splay<N>`; build(l, r) balanced build; kth(k) node at 1-indexed
 *        position k; reverse(l, r); print(x) in-order dump.
 *        Sentinel convention: build(0, n + 1) around a length-n sequence, then
 *        kth positions shift by +1.
 * Principle: every access rotates the node to the root; amortized O((n + q) log
 *            n) overall
 * Notes: reset rt / tot between test cases; alternatives:
 *        BalancedTree_Treap.cpp, BalancedTree_WBST_*.cpp
 * ============================================================
 */
