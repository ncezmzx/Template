#include <bits/stdc++.h>
using namespace std;

template <class S, S (*op)(S, S), S (*e)(), class F, S (*mapping)(F, S), F (*composition)(F, F), F (*id)()>
struct semigroup_segtree {
  struct node {
    S s;
    F tag;
    int lc, rc;
  };
  int n, root;
  vector<node> pool;

  void clear() {
    root = 0;
    pool.clear();
    pool.push_back({e(), id(), 0, 0});  // pool[0] 恒为哨兵，节点编号从 1 开始
  }
  void init(int n_) {
    n = n_;
    clear();
  }
  void reserve(int k) { pool.reserve(k); }
  int new_node() {
    pool.push_back({e(), id(), 0, 0});
    return (int)pool.size() - 1;
  }

  void apply(int u, F f) {
    if (!u) return;
    pool[u].s = mapping(f, pool[u].s);
    pool[u].tag = composition(f, pool[u].tag);
  }
  void push(int u) {
    if (!pool[u].lc) pool[u].lc = new_node();
    if (!pool[u].rc) pool[u].rc = new_node();
    if (pool[u].tag != id()) {
      apply(pool[u].lc, pool[u].tag);
      apply(pool[u].rc, pool[u].tag);
      pool[u].tag = id();
    }
  }
  void pull(int u) { pool[u].s = op(pool[pool[u].lc].s, pool[pool[u].rc].s); }

  void update(int u, int l, int r, int x, int y, F f) {
    if (x <= l && r <= y) return apply(u, f), void();
    push(u);
    int m = (l + r) >> 1;
    if (x <= m) update(pool[u].lc, l, m, x, y, f);
    if (m < y) update(pool[u].rc, m + 1, r, x, y, f);
    pull(u);
  }
  void update(int x, int y, F f) {
    if (!root) root = new_node();
    update(root, 1, n, x, y, f);
  }
  void update(int x, F f) { update(x, x, f); }

  void set(int u, int l, int r, int x, S v) {
    if (l == r) return pool[u].s = v, void();
    push(u);
    int m = (l + r) >> 1;
    if (x <= m) set(pool[u].lc, l, m, x, v);
    else set(pool[u].rc, m + 1, r, x, v);
    pull(u);
  }
  void set(int x, S v) {
    if (!root) root = new_node();
    set(root, 1, n, x, v);
  }

  S query(int u, int l, int r, int x, int y) {
    if (x <= l && r <= y) return u ? pool[u].s : e();
    if (u && pool[u].tag != id()) push(u);   // 查询不创建子节点：无标记才不下推
    int m = (l + r) >> 1;
    S res = e();
    if (x <= m) res = op(res, query(u ? pool[u].lc : 0, l, m, x, y));
    if (m < y) res = op(res, query(u ? pool[u].rc : 0, m + 1, r, x, y));
    return res;
  }
  S query(int x, int y) { return query(root, 1, n, x, y); }
  S query(int x) { return query(x, x); }
};

/*
 * ============================================================
 * 名称：自定义半群信息线段树（动态开点，懒标记）
 * 复杂度：单次操作 O(log n)；节点数 = O((修改次数 + 查询次数) * log n) 动态分配
 * 用途：维护自定义信息 info 与懒标记 tag 的区间操作：
 *       info + info -> info（op，区间合并）
 *       info + tag  -> info（mapping，把标记作用到信息上）
 *       tag + tag   -> tag（composition，标记复合）
 *       e() / id() 为信息与标记的幺元（空段/空标记）
 * 接口：
 *       update(x, y)      单点修改：x 处应用标记 y
 *       update(x, y, z)   区间修改：[x, y] 应用标记 z
 *       set(x, y)         单点赋值：x 处信息设为 y
 *       query(x, y)       区间询问：[x, y] 的信息和，返回 info
 *       query(x)          单点询问，返回 info
 *       init(n)/clear()   动态开设大小（范围 [1, n]）与清空
 * 原理：节点池动态分配（0 为空节点），首次触达时才创建；
 *       push 在需要细分时把懒标记下传并创建子节点；
 *       query 未创建的区域按 e() 处理
 * 注意：mapping 需对"整段"语义正确（如区间加时 sum 需乘段长，由用户实现）；
 *       composition(f, g) 表示"先 g 后 f"（mapping(composition(f,g),x)
 *       == mapping(f, mapping(g, x))）；多测例注意 init/clear
 * ============================================================
 * 使用示例（编译时取消注释；区间赋值 + 区间最大值，满足 mapping(f, e())==e()）：
 * using S = long long;
 * using F = long long;
 * S op(S a, S b) { return max(a, b); }
 * S e() { return LLONG_MIN; }
 * S mapping(F f, S s) { return f == id() ? s : f; }   // 区间赋值
 * F composition(F f, F g) { return f; }               // 后写覆盖（先 g 后 f）
 * F id() { return LLONG_MIN; }
 * semigroup_segtree<S, op, e, F, mapping, composition, id> st;
 * signed main() {
 *   st.init(1e9);                                // 动态开点：范围可很大
 *   st.update(2, 4, 7);                          // [2,4] 赋值为 7
 *   st.update(3, 9);                             // 单点 3 赋值为 9
 *   cout << st.query(1, 10) << '\n';             // 9
 *   st.set(4, 5);
 *   cout << st.query(4) << '\n';                 // 5
 *   st.clear();
 * }
 * 注意：mapping 必须满足 mapping(f, e()) == e()（未创建节点视为 e()），
 *       即整段操作对"隐式空段"必须是空操作；区间加+区间和的例子不满足该条件
 *       （空段的段长信息不存在），如需区间加请把段长并入 info（op/mapping
 *       自行处理），或用本示例的赋值类语义
 */
