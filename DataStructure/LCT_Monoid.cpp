#include <bits/stdc++.h>
using namespace std;

// generic-monoid Link-Cut Tree: dp[x] = dp[ls] * val[x] * dp[rs]; Mono must be reversible (flip)
// Mono requirements: default constructor = identity; operator*; flip() (swap left/right aggregates on reversal); Mono()
// (if queried path doesn't exist)
template <typename Mono, int N> class LinkCutTree {
#define ls son[x][0]
#define rs son[x][1]
  int son[N][2], fa[N];
  Mono dp[N], val[N];
  bool tag[N];
  void push_up(int x) {
    dp[x] = val[x];
    if (ls) dp[x] = dp[ls] * dp[x];
    if (rs) dp[x] = dp[x] * dp[rs];
  }
  void push_down(int x) {
    if (tag[x]) {
      tag[x] = 0, std::swap(ls, rs);
      if (ls) tag[ls] ^= 1, dp[ls].flip();
      if (rs) tag[rs] ^= 1, dp[rs].flip();
    }
  }
  int get(int x) { return son[fa[x]][1] == x; }
  bool isroot(int x) { return son[fa[x]][0] != x && son[fa[x]][1] != x; }
  void rotate(int x) {
    int f = fa[x], ff = fa[f], t = get(x);
    if (!isroot(f)) son[ff][get(f)] = x;
    son[f][t] = son[x][t ^ 1], fa[son[x][t ^ 1]] = f, push_up(f);
    son[x][t ^ 1] = f, fa[f] = x, fa[x] = ff, push_up(x);
  }
  void update(int x) {
    if (!isroot(x)) update(fa[x]);
    push_down(x);
  }
  void splay(int x) {
    for (update(x); !isroot(x); rotate(x))
      if (!isroot(fa[x])) rotate(get(x) == get(fa[x]) ? fa[x] : x);
  }
  int access(int x) {
    int p = 0;
    for (; x; p = x, x = fa[x]) splay(x), rs = p, push_up(x);
    return p;
  }
  int make_root(int x) { return x = access(x), tag[x] ^= 1, dp[x].flip(), x; }

  public:
  void init(int x, const Mono &v) { val[x] = v, dp[x] = v, tag[x] = 0, son[x][0] = son[x][1] = fa[x] = 0; }
  void link(int x, int y) {
    if (x != y && find(x) != find(y)) fa[make_root(x)] = y;
  }
  void cut(int x, int y) {
    make_root(x), access(y), splay(x);
    if (son[x][1] == y && !son[y][0]) son[x][1] = fa[y] = 0, push_up(x);
  }
  void set(int x, const Mono &v) { splay(x), val[x] = v, push_up(x); }
  Mono query(int u, int v) {
    if (find(u) != find(v)) return Mono();
    return make_root(u), dp[access(v)];
  }
  int find(int x) {
    for (push_down(x = access(x)); ls; x = ls, push_down(x))
      ;
    return splay(x), x;
  }
#undef ls
#undef rs
};

/*
 * ============================================================
 * Name: Link-Cut Tree (generic monoid version: path products + reversal)
 * Complexity: amortized O(log n) per operation
 * Usage: dynamic forest over any invertible monoid: link / cut / find
 *        (component representative) / set (point update) / query (path product
 *        u..v).
 * Comparison: against LCT.cpp (int-max specialization, array form): this
 *        version supports any invertible monoid (sums / extrema / gcd /
 *        commutative matrix products, ...) at the cost of Mono providing
 *        flip() and a default identity constructor; more features, same
 *        amortized-splay speed class (the specialized version is slightly
 *        faster — no generic-multiply indirection)
 * Source: user-provided code; two bugs fixed:
 *        1) push_down flipped tag[0]/dp[0] on null children (sentinel
 *           pollution) -> null checks added;
 *        2) find's loop descended before pushing tags (wrong direction under
 *           reverse tags) -> order corrected
 * Notes: Mono must supply flip() and a default identity constructor; see
 *        Comparison below
 * ============================================================
 */
