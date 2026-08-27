#include <bits/stdc++.h>
using namespace std;

// ACL lazy_segtree 的忠实移植（逐函数对齐 atcoder/lazysegtree.hpp）：
// 0-indexed、半开区间 [l, r)、静态满 2^k 数组布局、全程非递归
template <class S, S (*op)(S, S), S (*e)(), class F, S (*mapping)(F, S), F (*composition)(F, F), F (*id)()>
struct lazy_segtree {
  lazy_segtree() : lazy_segtree(0) {}
  explicit lazy_segtree(int n_) : lazy_segtree(vector<S>(n_, e())) {}
  explicit lazy_segtree(const vector<S>& v) : _n((int)v.size()) {
    size = 1, log = 0;
    while (size < _n) size <<= 1, ++log;  // bit_ceil
    d = vector<S>(2 * size, e());
    lz = vector<F>(size, id());
    for (int i = 0; i < _n; i++) d[size + i] = v[i];
    for (int i = size - 1; i >= 1; i--) update(i);
  }

  void set(int p, S x) {  // 单点赋值 d[p] = x
    p += size;
    for (int i = log; i >= 1; i--) push(p >> i);
    d[p] = x;
    for (int i = 1; i <= log; i++) update(p >> i);
  }
  S get(int p) {  // 单点取值
    p += size;
    for (int i = log; i >= 1; i--) push(p >> i);
    return d[p];
  }
  S prod(int l, int r) {  // op(a[l..r))，空区间返回 e()
    if (l == r) return e();
    l += size, r += size;
    for (int i = log; i >= 1; i--) {  // 只 push 与区间边界相关的祖先
      if (((l >> i) << i) != l) push(l >> i);
      if (((r >> i) << i) != r) push((r - 1) >> i);
    }
    S sml = e(), smr = e();
    while (l < r) {  // 自底向上收拢规范分解
      if (l & 1) sml = op(sml, d[l++]);
      if (r & 1) smr = op(d[--r], smr);
      l >>= 1, r >>= 1;
    }
    return op(sml, smr);
  }
  S all_prod() { return d[1]; }

  void apply(int p, F f) {  // 单点 apply
    p += size;
    for (int i = log; i >= 1; i--) push(p >> i);
    d[p] = mapping(f, d[p]);
    for (int i = 1; i <= log; i++) update(p >> i);
  }
  void apply(int l, int r, F f) {  // 区间 [l, r) apply
    if (l == r) return;
    l += size, r += size;
    for (int i = log; i >= 1; i--) {
      if (((l >> i) << i) != l) push(l >> i);
      if (((r >> i) << i) != r) push((r - 1) >> i);
    }
    int l2 = l, r2 = r;  // 整段覆盖的节点直接打标记
    while (l < r) {
      if (l & 1) all_apply(l++, f);
      if (r & 1) all_apply(--r, f);
      l >>= 1, r >>= 1;
    }
    l = l2, r = r2;
    for (int i = 1; i <= log; i++) {  // 回拉边界祖先
      if (((l >> i) << i) != l) update(l >> i);
      if (((r >> i) << i) != r) update((r - 1) >> i);
    }
  }

  template <bool (*g)(S)>
  int max_right(int l) { return max_right(l, [](S x) { return g(x); }); }
  template <class G>  // 最大 r 使 g(prod(l, r)) 真；要求 g(e()) 真
  int max_right(int l, G g) {
    if (l == _n) return _n;
    l += size;
    for (int i = log; i >= 1; i--) push(l >> i);
    S sm = e();
    do {
      while (l % 2 == 0) l >>= 1;
      if (!g(op(sm, d[l]))) {          // 该规范块失配：块内下探
        while (l < size) {
          push(l);
          l = 2 * l;
          if (g(op(sm, d[l]))) sm = op(sm, d[l]), l++;
        }
        return l - size;
      }
      sm = op(sm, d[l]);
      l++;
    } while ((l & -l) != l);
    return _n;
  }

  template <bool (*g)(S)>
  int min_left(int r) { return min_left(r, [](S x) { return g(x); }); }
  template <class G>  // 最小 l 使 g(prod(l, r)) 真；要求 g(e()) 真
  int min_left(int r, G g) {
    if (r == 0) return 0;
    r += size;
    for (int i = log; i >= 1; i--) push((r - 1) >> i);
    S sm = e();
    do {
      r--;
      while (r > 1 && (r % 2)) r >>= 1;
      if (!g(op(d[r], sm))) {
        while (r < size) {
          push(r);
          r = 2 * r + 1;
          if (g(op(d[r], sm))) sm = op(d[r], sm), r--;
        }
        return r + 1 - size;
      }
      sm = op(d[r], sm);
    } while ((r & -r) != r);
    return 0;
  }

 private:
  int _n, size, log;
  vector<S> d;
  vector<F> lz;

  void update(int k) { d[k] = op(d[2 * k], d[2 * k + 1]); }
  void all_apply(int k, F f) {
    d[k] = mapping(f, d[k]);
    if (k < size) lz[k] = composition(f, lz[k]);
  }
  void push(int k) {
    all_apply(2 * k, lz[k]);
    all_apply(2 * k + 1, lz[k]);
    lz[k] = id();
  }
};

/*
 * ============================================================
 * 名称：懒标记线段树·ACL 式（静态满树，泛型 op/mapping，全程非递归）
 * 复杂度：build O(n)；set/get/prod/apply/max_right/min_left 均 O(log n)
 * 用途：与 AtCoder Library lazy_segtree 完全同构的通用线段树：
 *       op/e 维护幺半群信息 S；mapping/composition/id 维护幺半群作用 F；
 *       模板参数与接口逐一对齐 ACL（0-indexed、半开区间 [l, r)）：
 *       set(p,x) get(p) prod(l,r) all_prod() apply(p,f) apply(l,r,f)
 *       max_right(l,g) min_left(r,g)（g 需满足 g(e()) 真；
 *       语义为沿规范块贪心：整块通过即越过，适合随区间扩大单调失效的 g）
 * 与 SegmentTree_Semigroup.cpp（动态开点版）的取舍：
 *       本版：静态满树（n 需可全建）、不要求 mapping(f, e()) == e()，
 *             支持"区间仿射 + 区间和"等依赖段长的作用——ACL 同款语义；
 *       动态版：值域可至 1e9 级，但作用需对"隐式空段 e()"正确
 * 原理：叶置于 [size, 2*size)（size 为 ≥ n 的 2 的幂），完全二叉数组布局；
 *       prod/apply 先沿两侧边界自 log 层向下 push 懒标记，再自底向上
 *       收拢/打标；set/get 沿单点路径 push 后直改直读；max_right/min_left
 *       从叶子出发沿规范块贪心，失配块内二分下探（无递归）
 * 注意：仅需单点修改时可用恒等作用退化（或沿用 zkw 版 IterativeLazy）；
 *       n == 0 时仅 all_prod/prod(l,l) 合法；prod 空区间返回 e()
 * ============================================================
 * 使用示例（编译时取消注释；区间仿射 + 区间和，动态版不支持的经典场景）：
 * using S = pair<long long, long long>;  // (和, 长度)
 * using F = pair<long long, long long>;  // a*x + b
 * S op(S a, S b) { return {a.first + b.first, a.second + b.second}; }
 * S e() { return {0, 0}; }
 * F id() { return {1, 0}; }
 * S mapping(F f, S s) { return {f.first * s.first + f.second * s.second, s.second}; }
 * F composition(F f, F g) {  // 先 g 后 f：f.(g.x) = f.a*(g.a*x+g.b)+f.b
 *   return {f.first * g.first, f.first * g.second + f.second};
 * }
 * lazy_segtree<S, op, e, F, mapping, composition, id> st;
 * signed main() {
 *   vector<S> a{{1, 1}, {2, 1}, {3, 1}};    // {1,2,3}
 *   st = lazy_segtree<S, op, e, F, mapping, composition, id>(a);
 *   st.apply(1, 3, {2, 1});                 // a = {1,5,7}
 *   cout << st.prod(0, 3).first << '\n';    // 13
 *   st.set(0, {10, 1});                     // a = {10,5,7}
 *   cout << st.get(0).first << ' ' << st.prod(2, 3).first << '\n';  // 10 7
 *   cout << st.max_right(0, [](S s) { return s.first <= 11; }) << '\n';  // 1（前缀和 15 越界）
 *   cout << st.min_left(3, [](S s) { return s.first <= 11; }) << '\n';   // 2（后缀和 12 越界）
 * }
 */
