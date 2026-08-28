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
 * Complexity: amortized O(log n) per operation (splay potential)
 * Usage: dynamic forest, wrapped as link_cut_tree<N>:
 *        link(x, y) add edge; cut(x, y) remove edge (x, y adjacent);
 *        makeroot(x) reroot; findroot(x) tree root of x;
 *        split(x, y) expose the x->y path as a splay rooted at y (then mx[y]
 *        holds the path aggregate); path_max / point_set are sample ops
 *        (change up to maintain sum / xor / min etc.)
 * Principle: preferred-path decomposition; each preferred path is a splay keyed
 *        by depth; access exposes the path from x to the root; makeroot uses a
 *        lazy reverse tag; virtual-child info is not maintained (add a virtual
 *        aggregate if subtree info is needed)
 * Notes: cut requires x, y to be directly connected; vertices 1..n;
 *        clear the arrays between test cases
 * ============================================================
 * Example (uncomment to compile; path maximum):
 * static link_cut_tree<100009> lct;
 * signed main() {
 *   int n, m;
 *   cin >> n >> m;
 *   for (int i = 1; i <= n; ++i) {
 *     cin >> lct.val[i];
 *     lct.mx[i] = lct.val[i];
 *   }
 *   while (m--) {
 *     int o, x, y;
 *     cin >> o >> x >> y;
 *     if (o == 0) cout << lct.path_max(x, y) << '\n';
 *     if (o == 1) lct.link(x, y);
 *     if (o == 2) lct.cut(x, y);
 *     if (o == 3) lct.point_set(x, y);
 *   }
 * }
 * ============================================================
 */
