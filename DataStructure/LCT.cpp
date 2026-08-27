#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int N = 1e5 + 9;
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

void split(int x, int y) { makeroot(x), access(y), splay(y); }

void link(int x, int y) { makeroot(x), fa[x] = y; }

void cut(int x, int y) {
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

/*
 * ============================================================
 * 名称：LCT（Link-Cut Tree，动态树，Splay 实现）
 * 复杂度：均摊 O(log n) 每次操作（splay 势能）
 * 用途：动态森林：
 *       link(x, y) 连边；cut(x, y) 删边（x、y 相邻时）；
 *       makeroot(x) 换根；findroot(x) 找所在树根；
 *       split(x, y) 把 x→y 路径提为以 y 为根的 splay（之后 mx[y]/sum[y]
 *       即路径信息）；path_max / point_set 为示例操作（改 up 可维护
 *       和/异或/最值等可加信息）
 * 原理：实链剖分 + 每条实链一棵 Splay（键为深度）；access 打通 x 到根
 *       的实链，makeroot 用懒标记翻转实现换根；虚儿子信息不维护
 *       （需要子树信息需维护虚儿子聚合）
 * 注意：cut 需确认 x、y 直接相连；节点编号 1..n；多组数据清空数组
 * ============================================================
 * 使用示例（编译时取消注释；路径最大值）：
 * signed main() {
 *   int n, m;
 *   cin >> n >> m;
 *   for (int i = 1; i <= n; ++i) {
 *     cin >> val[i];
 *     mx[i] = val[i];
 *   }
 *   while (m--) {
 *     int o, x, y;
 *     cin >> o >> x >> y;
 *     if (o == 0) cout << path_max(x, y) << '\n';
 *     if (o == 1) link(x, y);
 *     if (o == 2) cut(x, y);
 *     if (o == 3) point_set(x, y);
 *   }
 * }
 * ============================================================
 */
