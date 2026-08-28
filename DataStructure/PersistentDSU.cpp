#include <bits/stdc++.h>
using namespace std;

// 可持久化并查集：历史版本查询 + 按大小合并（无路径压缩，保证可持久化）
// 两棵可持久化线段树分别存 fa 与 siz；ver 0 为初始版本（fa[i]=i, siz[i]=1）
constexpr int N = 4e6 + 9, MV = 2e5 + 9;  // N ≈ 4n + 36m；MV = 版本数上界
int lc[N], rc[N], val[N], tot;
int faroot[MV], sizroot[MV], vcnt, n_;

int build(int l, int r, int type) {  // type 0：fa（叶子=下标）；type 1：siz（叶子=1）
  int u = ++tot;
  lc[u] = rc[u] = 0;
  if (l == r) {
    val[u] = type ? 1 : l;
    return u;
  }
  int m = l + r >> 1;
  lc[u] = build(l, m, type), rc[u] = build(m + 1, r, type);
  return u;
}
int upd(int pre, int l, int r, int p, int v) {
  int u = ++tot;
  lc[u] = lc[pre], rc[u] = rc[pre], val[u] = val[pre];
  if (l == r) {
    val[u] = v;
    return u;
  }
  int m = l + r >> 1;
  p <= m ? lc[u] = upd(lc[pre], l, m, p, v) : rc[u] = upd(rc[pre], m + 1, r, p, v);
  return u;
}
int qry(int u, int l, int r, int p) {
  if (l == r) return val[u];
  int m = l + r >> 1;
  return p <= m ? qry(lc[u], l, m, p) : qry(rc[u], m + 1, r, p);
}
void pdsu_init(int n) {
  n_ = n, tot = 0, vcnt = 0;
  faroot[0] = build(1, n, 0);
  sizroot[0] = build(1, n, 1);
}
int find(int ver, int x) {  // 版本 ver 中 x 的根（深度 O(log n)）
  int f = qry(faroot[ver], 1, n_, x);
  return f == x ? x : find(ver, f);
}
int pdsu_merge(int ver, int x, int y) {  // 在版本 ver 上合并 x,y，返回新版本号
  int rx = find(ver, x), ry = find(ver, y);
  ++vcnt;
  if (rx == ry) {
    faroot[vcnt] = faroot[ver], sizroot[vcnt] = sizroot[ver];
    return vcnt;
  }
  int sx = qry(sizroot[ver], 1, n_, rx), sy = qry(sizroot[ver], 1, n_, ry);
  if (sx < sy) swap(rx, ry), swap(sx, sy);
  faroot[vcnt] = upd(faroot[ver], 1, n_, ry, rx);       // 小树挂大树
  sizroot[vcnt] = upd(sizroot[ver], 1, n_, rx, sx + sy);
  return vcnt;
}
bool same(int ver, int x, int y) { return find(ver, x) == find(ver, y); }

/*
 * ============================================================
 * 名称：可持久化并查集
 * 复杂度：单次合并/查询 O(log² n)（find O(log n) 次点查，每查 O(log n)）
 * 用途：需要"回到历史版本"的并查集（离线可撤销题可用栈回退替代）；
 *       经典：洛谷 P3402 可持久化并查集（1 合并 / 2 回版本 k / 3 询问）
 * 接口：pdsu_init(n)；pdsu_merge(ver, x, y) → 新版本号；
 *       same(ver, x, y)；find(ver, x)。"回版本 k" 即把当前 ver 赋为 k
 * 原理：用两棵可持久化线段树分别持久化 fa[] 与 siz[]，版本 = 两棵树的
 *       根对。合并采用按大小（秩）合并保证树高 O(log n)，因此 find 可
 *       递归；**不能路径压缩**（会破坏旧版本共享的结构）
 * 注意：值域/节点号 1..n；N ≈ 4n + 36m（n、m 同阶 1e5 用 N=4e6；P3402
 *       n,m=2e5 需 N≈8e6）；内部用 32 位 int 存下标以省内存
 * 来源：OI-Wiki《可持久化并查集》（https://oi-wiki.org/ds/persistent-in-dsu/）
 * ============================================================
 * 使用示例（编译时取消注释；洛谷 P3402）：
 * signed main() {
 *   int n, m;
 *   cin >> n >> m;
 *   pdsu_init(n);
 *   int ver = 0;
 *   while (m--) {
 *     int op;
 *     cin >> op;
 *     if (op == 1) {
 *       int a, b;
 *       cin >> a >> b;
 *       ver = pdsu_merge(ver, a, b);
 *     } else if (op == 2) {
 *       int k;
 *       cin >> k;
 *       ver = k;
 *     } else {
 *       int a, b;
 *       cin >> a >> b;
 *       cout << same(ver, a, b) << '\n';
 *     }
 *   }
 * }
 * ============================================================
 */
