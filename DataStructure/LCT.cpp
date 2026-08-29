#include <bits/stdc++.h>
using namespace std;
#define int long long

// Link-Cut Tree (splay-based): dynamic forest with path aggregates
template <size_t N>
struct link_cut_tree {
  int ch[N][2], fa[N], rev[N], val[N], mx[N], stk[N];
  bool isroot(int x) { return ch[fa[x]][0] != x && ch[fa[x]][1] != x; }
  void up(int x) { mx[x] = max(val[x], max(mx[ch[x][0]], mx[ch[x][1]])); }
  void apply(int x) {
    if (x) swap(ch[x][0], ch[x][1]), rev[x] ^= 1;
  }
  void down(int x) {
    if (rev[x]) apply(ch[x][0]), apply(ch[x][1]), rev[x] = 0;
  }
  void rotate(int x) {
    int y = fa[x], z = fa[y], k = ch[y][1] == x;
    if (!isroot(y)) ch[z][ch[z][1] == y] = x;
    fa[x] = z;
    ch[y][k] = ch[x][k ^ 1];
    if (ch[x][k ^ 1]) fa[ch[x][k ^ 1]] = y;
    ch[x][k ^ 1] = y, fa[y] = x;
    up(y), up(x);
  }
  void splay(int x) {
    int tp = 0, y = x;
    stk[++tp] = y;
    while (!isroot(y)) stk[++tp] = y = fa[y];
    while (tp) down(stk[tp--]);
    while (!isroot(x)) {
      y = fa[x];
      int z = fa[y];
      if (!isroot(y)) rotate((ch[y][1] == x) ^ (ch[z][1] == y) ? x : y);
      rotate(x);
    }
  }
  void access(int x) {
    for (int y = 0; x; y = x, x = fa[x]) splay(x), ch[x][1] = y, up(x);
  }
  void makeroot(int x) { access(x), splay(x), apply(x); }
  int findroot(int x) {
    access(x), splay(x);
    while (ch[x][0]) down(x), x = ch[x][0];
    splay(x);
    return x;
  }
  void split(int x, int y) { makeroot(x), access(y), splay(y); }  // expose path x->y at y
  void link(int x, int y) { makeroot(x), fa[x] = y; }
  void cut(int x, int y) {  // requires x, y directly connected
    split(x, y);
    if (ch[y][0] == x && !ch[x][1]) ch[y][0] = fa[x] = 0, up(y);
  }
  int path_max(int x, int y) {
    split(x, y);
    return mx[y];
  }
  void point_set(int x, int v) {
    splay(x);
    val[x] = v;
    up(x);
  }
};

/*
 * ============================================================
 * Name: Link-Cut Tree (dynamic trees, splay-based)
 * Complexity: amortized O(log n) per operation
 * Usage: dynamic forest, `link_cut_tree<N>`: link(x, y); cut(x, y) (x, y must
 *        be adjacent); makeroot(x) reroots; findroot(x);
 *        split(x, y) exposes the x->y path as a splay rooted at y (aggregate in
 *        mx[y]); path_max / point_set are samples (change up for sum / xor /
 *        ...).
 * Principle: preferred-path decomposition, one splay per path keyed by depth;
 *            access exposes the path to the root; makeroot uses a lazy reverse
 *            tag
 * Notes: vertices are 1..n; clear the arrays between test cases; virtual-child
 *        info is not maintained (add it yourself for subtree aggregates)
 * ============================================================
 */
