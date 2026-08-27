#include <bits/stdc++.h>
using namespace std;

template <class S, S (*op)(S, S), S (*e)(), class F, S (*mapping)(F, S), F (*composition)(F, F), F (*id)()>
struct semigroup_segtree {
  struct node {
    S s;
    F tag;
    int lc, rc;
  };
  static constexpr int DEP = 64;               // 深度上限（值域可达 2^63）
  int n, root;
  vector<node> pool;
  int stk[DEP * 3 + 4];                        // 修改/赋值的下探路径（按下探顺序入栈，逆序回拉）
  int pth[DEP + 2], pl[DEP + 2], pr[DEP + 2];  // 询问/二分的叶子路径（带区间）

  void clear() {
    root = 0;
    pool.clear();
    pool.push_back({e(), id(), 0, 0});  // pool[0] 恒为哨兵，节点编号从 1 开始
  }
  void init(int n_) {
    n = n_;
    clear();
  }
  // ACL 的 vector 构造对齐：a[1..n_] 为初值，BFS 建满树（O(n)，仅适用 n 可全建时）
  void init(int n_, const vector<S>& a) {
    init(n_);
    if (n_ < 1) return;
    pool.reserve(2 * n_);
    root = new_node();
    vector<int> qu{root}, ql{1}, qr{n_};  // BFS 层序：先建儿子，后逆序 pull
    for (int i = 0; i < (int)qu.size(); i++) {
      int u = qu[i];
      if (ql[i] == qr[i]) {
        pool[u].s = a[ql[i]];
        continue;
      }
      int m = (ql[i] + qr[i]) >> 1, c1 = new_node(), c2 = new_node();
      pool[u].lc = c1, pool[u].rc = c2;  // 先取返回值再写成员，避免扩容后引用失效
      qu.push_back(c1), ql.push_back(ql[i]), qr.push_back(m);
      qu.push_back(c2), ql.push_back(m + 1), qr.push_back(qr[i]);
    }
    for (int i = (int)qu.size() - 1; i >= 0; i--)  // 逆 BFS 序 = 孩子先于父亲
      if (ql[i] != qr[i]) pull(qu[i]);
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
    // 注意：new_node() 会扩容 pool，必须先取返回值再写成员，
    // 否则 pool[u].lc = new_node() 的求值顺序（C++14 未指定）会写进已释放的旧块（UAF）
    if (!pool[u].lc) {
      int c = new_node();
      pool[u].lc = c;
    }
    if (!pool[u].rc) {
      int c = new_node();
      pool[u].rc = c;
    }
    if (pool[u].tag != id()) {
      apply(pool[u].lc, pool[u].tag);
      apply(pool[u].rc, pool[u].tag);
      pool[u].tag = id();
    }
  }
  void pull(int u) { pool[u].s = op(pool[pool[u].lc].s, pool[pool[u].rc].s); }

  // ---- 非递归核心：[x, y] 分解为「分割节点 + 左右两条边界路径」----
  // 分割节点 u：唯一满足 x <= mid(u) < y 的（内点）节点；[x, y] 的规范分解中，
  // 整段覆盖的节点 = 左边界路径上「向左走」时的右儿子们（含路径终点）+ 右侧对称。
  void update(int x, int y, F f) {  // 区间 [x, y] 应用标记 f
    if (x > y) return;
    if (!root) root = new_node();
    int u = root, l = 1, r = n, tp = 0;
    while (l != r) {  // 先下探到分割节点（或 x==y 的叶子），沿途 push
      int m = (l + r) >> 1;
      if (y <= m) push(u), stk[tp++] = u, u = pool[u].lc, r = m;
      else if (x > m) push(u), stk[tp++] = u, u = pool[u].rc, l = m + 1;
      else break;
    }
    if (l == r) {
      apply(u, f);  // x == y：直接落在叶子上
    } else {
      push(u), stk[tp++] = u;  // 分割节点
      int m = (l + r) >> 1;
      for (int v = pool[u].lc, ll = l, rr = m;;) {  // 左边界路径（跟随 x）
        if (x <= ll) {                              // [ll, rr] 整段被覆盖（rr < y 恒成立）
          apply(v, f);
          break;
        }
        push(v), stk[tp++] = v;
        int mm = (ll + rr) >> 1;
        if (x <= mm) apply(pool[v].rc, f), v = pool[v].lc, rr = mm;  // 右儿子整段覆盖
        else v = pool[v].rc, ll = mm + 1;
      }
      for (int v = pool[u].rc, ll = m + 1, rr = r;;) {  // 右边界路径（跟随 y）
        if (rr <= y) {                                   // [ll, rr] 整段被覆盖（ll > x 恒成立）
          apply(v, f);
          break;
        }
        push(v), stk[tp++] = v;
        int mm = (ll + rr) >> 1;
        if (y > mm) apply(pool[v].lc, f), v = pool[v].rc, ll = mm + 1;  // 左儿子整段覆盖
        else v = pool[v].lc, rr = mm;
      }
    }
    while (tp) pull(stk[--tp]);  // 逆下探序回拉（孩子必先于父亲）
  }
  void update(int x, F f) { update(x, x, f); }

  void set(int x, S v) {  // 单点赋值：x 处信息设为 v
    if (!root) root = new_node();
    int u = root, l = 1, r = n, tp = 0;
    while (l != r) {  // 迭代下探到叶子（沿途 push 建路），再逆序回拉
      push(u), stk[tp++] = u;
      int m = (l + r) >> 1;
      if (x <= m) u = pool[u].lc, r = m;
      else u = pool[u].rc, l = m + 1;
    }
    pool[u].s = v;
    while (tp) pull(stk[--tp]);
  }

  S query(int x, int y) {  // 区间 [x, y] 的信息（不新建节点：未建子树按 e() 计）
    if (x > y || !root) return e();
    int u = root, l = 1, r = n;
    while (l != r) {
      int m = (l + r) >> 1;
      if (y <= m) {
        if (!u) return e();
        if (pool[u].tag != id()) push(u);  // 查询只在有标记时下传（此时才建节点）
        u = pool[u].lc, r = m;
      } else if (x > m) {
        if (!u) return e();
        if (pool[u].tag != id()) push(u);
        u = pool[u].rc, l = m + 1;
      } else break;
    }
    if (!u || l == r) return u ? pool[u].s : e();
    if (pool[u].tag != id()) push(u);
    int m = (l + r) >> 1;
    S res = e();
    for (int v = pool[u].lc, ll = l, rr = m;;) {  // 左边界：规范块自顶向下“从右往左”出现，
      if (x <= ll) {                              // 故新块折叠到左边
        if (v) res = op(pool[v].s, res);
        break;
      }
      if (!v) break;  // 未建子树：剩余部分全为 e()
      if (pool[v].tag != id()) push(v);
      int mm = (ll + rr) >> 1;
      if (x <= mm) {
        int c = pool[v].rc;
        if (c) res = op(pool[c].s, res);
        v = pool[v].lc, rr = mm;
      } else v = pool[v].rc, ll = mm + 1;
    }
    for (int v = pool[u].rc, ll = m + 1, rr = r;;) {  // 右边界：规范块自顶向下“从左往右”出现，
      if (rr <= y) {                                  // 故新块折叠到右边
        if (v) res = op(res, pool[v].s);
        break;
      }
      if (!v) break;
      if (pool[v].tag != id()) push(v);
      int mm = (ll + rr) >> 1;
      if (y > mm) {
        int c = pool[v].lc;
        if (c) res = op(res, pool[c].s);
        v = pool[v].rc, ll = mm + 1;
      } else v = pool[v].lc, rr = mm;
    }
    return res;
  }
  S query(int x) { return query(x, x); }
  S get(int x) { return query(x, x); }  // ACL 接口：单点询问
  S all_prod() { return root ? pool[root].s : e(); }  // ACL 接口：全域 [1, n] 信息

  // ACL 接口：最大 r（x-1 <= r <= n）使 g(query(x, r)) 为真；要求 g(e()) == 真
  // 实现为「自 x 的叶子沿路径向上的右侧规范块」逐块贪心 + 失配块内二分下探
  template <class G>
  int max_right(int x, G g) {
    if (x > n || !root) return n;
    int u = root, l = 1, r = n, tp = 0;
    while (l != r) {
      if (pool[u].tag != id()) push(u);
      pth[tp] = u, pl[tp] = l, pr[tp] = r, tp++;
      int m = (l + r) >> 1;
      if (x <= m) u = pool[u].lc, r = m;
      else u = pool[u].rc, l = m + 1;
    }
    pth[tp] = u;  // 叶子 [x, x]（可为哨兵 0：按 e() 处理）
    S sm = e();
    if (!g(op(sm, pool[u].s))) return x - 1;  // 首块（叶子）即失配
    sm = op(sm, pool[u].s);
    for (int i = tp - 1; i >= 0; i--) {
      int p = pth[i];
      if (pth[i + 1] != pool[p].lc) continue;   // 只取「从左儿子上来」的右兄弟块
      int b = pool[p].rc, m = (pl[i] + pr[i]) >> 1;
      if (!b) continue;                         // 空块 = e()，自动满足
      if (g(op(sm, pool[b].s))) {               // 整块并入
        sm = op(sm, pool[b].s);
        continue;
      }
      int w = b, lw = m + 1, rw = pr[i];        // 块内下探找最右可行位置
      while (w && lw < rw) {
        if (pool[w].tag != id()) push(w);
        int mm = (lw + rw) >> 1;
        if (g(op(sm, pool[pool[w].lc].s))) sm = op(sm, pool[pool[w].lc].s), w = pool[w].rc, lw = mm + 1;
        else w = pool[w].lc, rw = mm;
      }
      if (!w) continue;  // 剩余未建 = e()，整块并入
      return lw - 1;     // 叶子 lw 处失配：答案为 lw-1
    }
    return n;
  }

  // ACL 接口：最小 l（1 <= l <= y+1）使 g(query(l, y)) 为真；要求 g(e()) == 真
  template <class G>
  int min_left(int y, G g) {
    if (y < 1 || !root) return 1;
    int u = root, l = 1, r = n, tp = 0;
    while (l != r) {
      if (pool[u].tag != id()) push(u);
      pth[tp] = u, pl[tp] = l, pr[tp] = r, tp++;
      int m = (l + r) >> 1;
      if (y <= m) u = pool[u].lc, r = m;
      else u = pool[u].rc, l = m + 1;
    }
    pth[tp] = u;
    S sm = e();
    if (!g(op(pool[u].s, sm))) return y + 1;  // 首块（叶子）即失配
    sm = op(pool[u].s, sm);
    for (int i = tp - 1; i >= 0; i--) {
      int p = pth[i];
      if (pth[i + 1] != pool[p].rc) continue;   // 只取「从右儿子上来」的左兄弟块
      int b = pool[p].lc, m = (pl[i] + pr[i]) >> 1;
      if (!b) continue;
      if (g(op(pool[b].s, sm))) {  // 整块并入（新块在左侧）
        sm = op(pool[b].s, sm);
        continue;
      }
      int w = b, lw = pl[i], rw = m;  // 块内下探找最左可行位置
      while (w && lw < rw) {
        if (pool[w].tag != id()) push(w);
        int mm = (lw + rw) >> 1;
        if (g(op(pool[pool[w].rc].s, sm))) sm = op(pool[pool[w].rc].s, sm), w = pool[w].lc, rw = mm;
        else w = pool[w].rc, lw = mm + 1;
      }
      if (!w) continue;
      return lw + 1;  // 叶子 lw 失配：答案为 lw+1
    }
    return 1;
  }
};

/*
 * ============================================================
 * 名称：自定义半群信息线段树（动态开点，懒标记，非递归）
 * 复杂度：单次操作 O(log n)（无递归调用）；节点数 = O(修改次数 * log n) 动态分配
 * 用途：维护自定义信息 info 与懒标记 tag 的区间操作（与 ACL lazy_segtree 同构）：
 *       info + info -> info（op，区间合并，可不交换）
 *       info + tag  -> info（mapping，把标记作用到信息上）
 *       tag + tag   -> tag（composition，标记复合）
 *       e() / id() 为信息与标记的幺元（空段/空标记）
 * 接口（1-indexed，闭区间 [x, y]）：
 *       update(x, y, z)     区间修改：[x, y] 应用标记 z
 *       update(x, y)        单点修改：x 处应用标记 y
 *       set(x, y)           单点赋值：x 处信息设为 y
 *       query(x, y)         区间询问，返回 info；query(x) 单点（get(x) 同义）
 *       all_prod()          全域 [1, n] 的 info
 *       max_right(x, g)     最大 r 使 g(query(x, r)) 真（需 g(e()) 真）
 *       min_left(y, g)      最小 l 使 g(query(l, y)) 真（需 g(e()) 真）
 *       init(n)             空树（范围 [1, n]，可至 1e9 级）
 *       init(n, a)          ACL 式 vector 建树：a[1..n] 为初值，O(n)
 *       clear()/reserve(k)  清空 / 预留节点池
 * 原理：节点池动态分配（0 为哨兵，未建子树按 e() 计）；全部操作改为迭代——
 *       区间操作先下探到「分割节点」（唯一满足 x <= mid < y 的节点），
 *       再沿左右两条边界路径走到底：整段覆盖的规范块打标记/取信息，
 *       修改沿入栈路径逆序回拉；max_right/min_left 沿叶子路径向上的
 *       规范块逐块贪心 + 失配块内二分下探（与 ACL 自底向上写法等价）；
 *       询问/二分只在节点带标记时才下传（此时才创建节点）
 * 注意：composition(f, g) 表示"先 g 后 f"（mapping(composition(f,g),x)
 *       == mapping(f, mapping(g, x))）；mapping 必须满足 mapping(f, e()) == e()
 *       （未创建节点视为 e()，即整段操作对"隐式空段"必须是空操作）——
 *       区间赋值类语义成立；区间加+区间和不满足该条件（空段的段长信息
 *       不存在），如需区间加请把段长并入 info，或改用 SegmentTree_ACL.cpp
 *       （静态满树，无此限制）；max_right/min_left 为 ACL 规范块贪心语义，
 *       整块通过时不再检查块内前缀（适合随区间扩大单调失效的 g）；
 *       多测例注意 init/clear
 * ============================================================
 * 使用示例（编译时取消注释；区间赋值 + 区间最大值，满足 mapping(f, e())==e()）：
 * using S = long long;
 * using F = long long;
 * S op(S a, S b) { return max(a, b); }
 * S e() { return LLONG_MIN; }
 * F id() { return LLONG_MIN; }
 * S mapping(F f, S s) { return f == id() ? s : f; }   // 区间赋值
 * F composition(F f, F g) { return f; }               // 后写覆盖（先 g 后 f）
 * semigroup_segtree<S, op, e, F, mapping, composition, id> st;
 * signed main() {
 *   st.init(1e9);                                // 动态开点：范围可很大
 *   st.update(2, 4, 7);                          // [2,4] 赋值为 7
 *   st.update(3, 9);                             // 单点 3 赋值为 9
 *   cout << st.query(1, 10) << '\n';             // 9
 *   st.set(4, 5);
 *   cout << st.query(4) << '\n';                 // 5
 *   // ACL 对齐接口：
 *   vector<S> a{0, 1, 2, 3, 4};                  // a[1..4] = {1,2,3,4}
 *   st.init(4, a);                               // O(n) 建满树
 *   st.update(1, 2, 10);                         // [1,2] 赋值为 10
 *   cout << st.get(2) << ' ' << st.all_prod() << '\n';            // 10 10
 *   cout << st.max_right(3, [](S s) { return s < 5; }) << '\n';   // 4（[3,4] 最大值 4 < 5，全通过）
 *   cout << st.min_left(4, [](S s) { return s < 5; }) << '\n';    // 3（l=2 时含 10，失配）
 *   st.clear();
 * }
 */
