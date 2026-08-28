#include <bits/stdc++.h>
using namespace std;
#define int long long

// Splay tree for sequence maintenance (range reverse, etc.)
template <size_t N>
struct splay {
  int rt, tot;
  int ch[N][2], fa[N], sz[N], val[N], tg[N];
  void up(int x) { sz[x] = 1 + sz[ch[x][0]] + sz[ch[x][1]]; }
  void apply(int x) { swap(ch[x][0], ch[x][1]), tg[x] ^= 1; }  // reverse tag
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
  void splay_(int x, int goal) {  // lift x until its parent is goal
    while (fa[x] != goal) {
      int y = fa[x], z = fa[y];
      if (z != goal) rotate((ch[y][1] == x) ^ (ch[z][1] == y) ? x : y);
      rotate(x);
    }
    if (!goal) rt = x;
  }
  // balanced build of values l..r (use sentinels 0 and n+1 around a length-n sequence)
  int build(int l, int r) {
    if (l > r) return 0;
    int m = (l + r) >> 1, x = ++tot;
    val[x] = m, sz[x] = 1;
    if (l < m) ch[x][0] = build(l, m - 1), fa[ch[x][0]] = x;
    if (m < r) ch[x][1] = build(m + 1, r), fa[ch[x][1]] = x;
    up(x);
    return x;
  }
  int kth(int k) {  // position of the k-th node (1-indexed)
    int x = rt;
    while (true) {
      down(x);
      if (sz[ch[x][0]] >= k) x = ch[x][0];
      else if (k == sz[ch[x][0]] + 1) return x;
      else k -= sz[ch[x][0]] + 1, x = ch[x][1];
    }
  }
  void reverse(int l, int r) {  // reverse positions l..r (with sentinels: shift by 1)
    int L = kth(l), R = kth(r + 2);
    splay_(L, 0), splay_(R, L);
    apply(ch[R][0]);
  }
  void print(int x) {  // in-order dump
    if (!x) return;
    down(x);
    print(ch[x][0]), cout << val[x] << ' ', print(ch[x][1]);
  }
};

/*
 * ============================================================
 * Name: splay tree (sequence operations: range reverse etc.)
 * Complexity: amortized O(log n) per operation
 * Usage: sequence maintenance, wrapped as splay<N>: range reverse, range
 *        insert/delete, range moves; kth both endpoints around [l, r], splay
 *        them to the root and the root's right child, and the interval
 *        becomes the left subtree of the root's right child — tag it or
 *        operate on it wholesale
 * Principle: every access rotates the node to the root (splay); amortized
 *        analysis gives O((n+q) log n) overall
 * Notes: alternative implementations of the same balanced-tree job live in
 *        BalancedTree_Treap.cpp and BalancedTree_WBST_*.cpp; this file is
 *        the sequence version (with sentinels: building 1..n wrapped by 0
 *        and n+1 avoids kth overflow — the example builds 0..n+1 directly,
 *        kth positions shift by +1)
 * Usage pattern: rt = build(0, n + 1); reverse(l, r) reverses [l, r] (real positions)
 * ============================================================
 * Example (uncomment to compile; range reverse):
 * static splay<100009> sp;
 * signed main() {
 *   int n, m;
 *   cin >> n >> m;
 *   sp.rt = sp.build(0, n + 1);
 *   while (m--) {
 *     int l, r;
 *     cin >> l >> r;
 *     sp.reverse(l, r);
 *   }
 *   sp.print(sp.rt);
 * }
 * ============================================================
 */
