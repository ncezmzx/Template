#include <bits/stdc++.h>
using namespace std;

// 李超线段树：值域 [1, n] 上插入线段/直线 y = kx + b，查询某 x 处的最小值
// 全域直线插入不新建节点；线段插入每次 O(log n) 新节点
constexpr int N = 1e6 + 9;    // 值域上限（按需调整）
constexpr int SP = 2e6 + 9;    // 节点池（线段插入次数 × 2log n，按需调整）
constexpr long long INF = 4e18;
int n_, tot, rt, lc[SP], rc[SP];
long long K[SP], B[SP];  // 节点存放一条线 y = Kx + B（哨兵 K=0, B=INF）

int new_node() {
  ++tot, lc[tot] = rc[tot] = 0, K[tot] = 0, B[tot] = INF;
  return tot;
}
void init(int n) { n_ = n, tot = 0, rt = new_node(); }
inline long long f(long long k, long long b, int x) { return k * x + b; }  // 需保证不溢出
void insert_full(int u, int l, int r, long long k, long long b) {  // 在 [l,r] 整段上保留较优线
  int m = l + r >> 1;
  bool lef = f(k, b, l) < f(K[u], B[u], l), mid = f(k, b, m) < f(K[u], B[u], m);
  if (mid) swap(k, K[u]), swap(b, B[u]);  // 中点更优者留本节点
  if (l == r) return;
  if (lef != mid) {  // 交点在左半 → 旧线去左子树
    if (!lc[u]) lc[u] = new_node();
    insert_full(lc[u], l, m, k, b);
  } else {  // 交点在右半（或无交点劣者单调）→ 去右子树
    if (!rc[u]) rc[u] = new_node();
    insert_full(rc[u], m + 1, r, k, b);
  }
}
void ins(int& u, int l, int r, int x, int y, long long k, long long b) {  // 线段限制在 [x,y]
  if (!u) u = new_node();
  if (x <= l && r <= y) return insert_full(u, l, r, k, b);
  int m = l + r >> 1;
  if (x <= m) ins(lc[u], l, m, x, y, k, b);
  if (y > m) ins(rc[u], m + 1, r, x, y, k, b);
}
void add_line(long long k, long long b) { insert_full(rt, 1, n_, k, b); }        // 全域直线
void add_seg(int x, int y, long long k, long long b) { ins(rt, 1, n_, x, y, k, b); }  // 线段 [x,y]
long long query(int x) {  // 覆盖 x 的所有线段/直线在 x 处的最小值（INF 表示无覆盖）
  int u = rt, l = 1, r = n_;
  long long res = INF;
  while (u) {
    res = min(res, f(K[u], B[u], x));
    int m = l + r >> 1;
    if (x <= m) u = lc[u], r = m;
    else u = rc[u], l = m + 1;
  }
  return res;
}

/*
 * ============================================================
 * 名称：李超线段树（线段插入，区间最小查询）
 * 复杂度：插入 O(log n)（新节点 ≤ 2log n）；查询 O(log n)
 * 用途：维护线段/直线集合，查询任意整点 x 处的最小（大）函数值：
 *       斜率优化 DP 强制在线、威佐夫博弈类最值覆盖等
 * 接口：init(n) 值域 [1,n]；add_line(k,b) 全域直线；
 *       add_seg(x,y,k,b) 线段限定 [x,y]；query(x) 最小值
 * 原理：每个节点存"在该节点中点最优"的一条线；插入时与节点线比
 *       中点，劣者按端点关系沉入左/右子树（两线至多一个交点，
 *       交点侧决定递归方向）；查询沿根→叶路径取所有节点线最小
 * 注意：k·x + b 需在 long long 内（数据保证）；查询最大值把
 *       insert_full/query 的比较方向全部取反；哨兵 B=INF 表示空
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   init(10);
 *   add_line(1, 0);            // y = x
 *   add_line(-1, 5);           // y = 5 - x
 *   add_seg(4, 8, 0, 1);       // [4,8] 上 y = 1
 *   cout << query(2) << ' ' << query(3) << '\n';  // 2 2（两直线交点 (2.5, 2.5)）
 *   cout << query(5) << ' ' << query(9) << '\n';  // 0 -4（5 处 y=5-x=0 最优；9 处线段未覆盖取 y=5-9）
 * }
 */
