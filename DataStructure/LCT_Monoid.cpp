#include <bits/stdc++.h>
using namespace std;

// 泛型半群 Link-Cut-Tree：dp[x] = dp[ls] * val[x] * dp[rs]，Mono 需可逆（flip）
// Mono 需求：默认构造为幺元；operator*；flip()（整段反转时交换左右累积）
template <typename Mono, int N>
class LinkCutTree {
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
      tag[x] = 0;
      std::swap(ls, rs);
      if (ls) tag[ls] ^= 1, dp[ls].flip();   // 空儿子不动哨兵（原稿会翻转 tag[0]）
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
    update(x);
    for (; !isroot(x); rotate(x))
      if (!isroot(fa[x])) rotate(get(x) == get(fa[x]) ? fa[x] : x);
  }
  int access(int x) {
    int p = 0;
    for (; x; p = x, x = fa[x]) splay(x), rs = p, push_up(x);
    return p;
  }
  int make_root(int x) {
    return x = access(x), tag[x] ^= 1, dp[x].flip(), x;
  }

 public:
  void init(int x, const Mono &v) {   // 单点初值（x 的 dp/val 置 v）
    val[x] = v, dp[x] = v, tag[x] = 0, son[x][0] = son[x][1] = fa[x] = 0;
  }
  void link(int x, int y) { fa[make_root(x)] = y; }
  void cut(int x, int y) {
    make_root(x), access(y), splay(x);
    if (son[x][1] == y && !son[y][0]) son[x][1] = fa[y] = 0, push_up(x);
  }
  void set(int x, const Mono &v) { splay(x), val[x] = v, push_up(x); }
  Mono query(int u, int v) { return make_root(u), dp[access(v)]; }
  int find(int x) {
    x = access(x);
    for (push_down(x); ls; x = ls, push_down(x));   // 先下放标记再下移（原稿顺序颠倒会走错方向）
    splay(x);
    return x;
  }
#undef ls
#undef rs
};

/*
 * ============================================================
 * 名称：Link-Cut-Tree（泛型半群版，路径乘积 + 反转）
 * 复杂度：均摊 O(log n) 每操作
 * 用途：动态森林上 link / cut / find（连通分量代表）、
 *       set（单点改）/ query（路径乘积 u..v）
 * 对比：与 LCT.cpp（int 最大值特化、数组版）相比：
 *       本版支持任意可逆半群（和 / 最值 / gcd / 可交换矩阵乘等），
 *       代价是 Mono 需提供 flip() 与幺元默认构造；功能更齐，
 *       速度两者同阶（均摊 splay），特化版略快（无泛型乘法间接层）
 * 来源：用户提供代码；已修两处问题：
 *       1) push_down 在空儿子上翻转 tag[0]/dp[0]（哨兵污染）→ 加空判；
 *       2) find 的循环先下移后下放标记（有反转标记时走错方向）→ 改正顺序
 * ============================================================
 * 使用示例（编译时取消注释；路径最大值，与 LCT.cpp 等价语义）：
 * struct MxMonoid {
 *   long long v = -0x3f3f3f3f3f3f3f3f;   // 默认构造 = 幺元
 *   MxMonoid() = default;
 *   MxMonoid(long long x) : v(x) {}
 *   void flip() {}   // max 可交换：反转不影响累积；不可交换半群需交换内部累积
 * };
 * MxMonoid operator*(MxMonoid a, MxMonoid b) { return MxMonoid(max(a.v, b.v)); }
 * signed main() {
 *   LinkCutTree<MxMonoid, 100009> lct;
 *   int n = 5;
 *   long long w[] = {0, 3, 1, 4, 2, 5};
 *   for (int i = 1; i <= n; ++i) lct.init(i, MxMonoid(w[i]));
 *   lct.link(1, 2), lct.link(2, 3), lct.link(3, 4), lct.link(4, 5);
 *   cout << lct.query(1, 5).v << '\\n';   // 5
 *   lct.set(3, MxMonoid(9));
 *   cout << lct.query(2, 4).v << '\\n';   // 9
 * }
 * ============================================================
 */
