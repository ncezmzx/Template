#include <bits/stdc++.h>
using namespace std;
namespace wbst {

constexpr int N = 1e5 + 9;
int n, m, ch[N << 1][2], sz[N << 1], val[N << 1], stk[N << 1], tg[N << 1], tp, tot, rt, a[N];

void up(int x) { sz[x] = sz[ch[x][0]] + sz[ch[x][1]], val[x] = val[ch[x][1]]; }
void apply(int x) { tg[x] ^= 1, swap(ch[x][0], ch[x][1]); }
void down(int x) { if (tg[x]) apply(ch[x][0]), apply(ch[x][1]), tg[x] = 0; }
void erase(int &x) { stk[++tp] = x, x = 0; }
int make(int x = tp ? stk[tp--] : ++tot) { return ch[x][0] = ch[x][1] = val[x] = sz[x] = 0, x; }
int node(int v, int u = make()) { return val[u] = v, sz[u] = 1, u; }
int link(int x, int y, int z = make()) { return ch[z][0] = x, ch[z][1] = y, up(z), z; }
auto cut(int x) {
  int y = ch[x][0], z = ch[x][1];
  return down(x), erase(x), make_pair(y, z);
}
void rotate(int &x, bool r) {
  auto [a, b] = cut(x);
  if (r) {
    auto [c, d] = cut(b);
    x = link(link(a, c), d);
  }
  else {
    auto [c, d] = cut(a);
    x = link(c, link(d, b));
  }
}
bool heavy(int x, int y) { return x > 3 * y; }
bool need(int x, int r) { return sz[ch[x][!r]] > 2 * sz[ch[x][r]]; }
void balance(int &x) {
  if (sz[x] == 1) return ;
  down(x);
  bool r = sz[ch[x][1]] > sz[ch[x][0]];
  if (!heavy(sz[ch[x][r]], sz[ch[x][!r]])) return ;
  down(ch[x][r]);
  if (need(ch[x][r], r)) down(ch[ch[x][r]][!r]), rotate(ch[x][r], !r);
  rotate(x, r);
}
int mer(int x, int y) {
  if (!x || !y) return x + y;
  if (heavy(sz[x], sz[y])) {
    auto [a, b] = cut(x);
    int z = link(a, mer(b, y));
    return balance(z), z;
  }
  else if (heavy(sz[y], sz[x])) {
    auto [a, b] = cut(y);
    int z = link(mer(x, a), b);
    return balance(z), z;
  }
  else return link(x, y);
}
auto spl(int x, int k) {
  if (!x) return make_pair(0, 0);
  if (!k) return make_pair(0, x);
  if (k == sz[x]) return make_pair(x, 0);
  auto [a, b] = cut(x);
  if (k <= sz[a]) {
    auto [c, d] = spl(a, k);
    return make_pair(c, mer(d, b));
  }
  else {
    auto [c, d] = spl(b, k - sz[a]);
    return make_pair(mer(a, c), d);
  }
}
int build(int l, int r) {
  if (l == r) return node(l);
  int m = (l + r) >> 1;
  return link(build(l, m), build(m + 1, r));
}
void print(int x) {
  if (sz[x] == 1) return cout << val[x] << ' ', void();
  down(x);
  print(ch[x][0]), print(ch[x][1]);
}

}
/*
 * ============================================================
 * 名称：权值平衡树·序列版（确定性 Weight Balanced Tree，带懒标记 reverse）
 * 复杂度：split/merge/balance 摊还 O(log n)；无随机优先级，可完全替代文艺平衡树
 * 用途：区间反转/区间操作：
 *       spl(x, k) 按大小拆成 (前 k 个, 剩余)；mer(x, y) 合并两棵；
 *       apply(x) 打反转懒标记（交换左右子树）；balance(x) 保持平衡；
 *       up/down/rotate/cut/erase/make/node/link 为内部维护函数
 * 来源：all.cpp 第 30725-30799 行（原样保留；注释已统一移至文件尾部）
 * ============================================================
 * 使用示例（编译时取消注释）：
 *   signed main() {
 *     cin.tie(nullptr)->sync_with_stdio(false);
 *     wbst::n = 5;
 *     wbst::rt = wbst::build(1, 5);          // 1 2 3 4 5
 *     auto [a, b] = wbst::spl(wbst::rt, 1);  // 拆出前 1 个
 *     auto [c, d] = wbst::spl(b, 3);         // 拆出 [2,4]
 *     wbst::apply(c);                        // 反转 [2,4]
 *     wbst::rt = wbst::mer(wbst::mer(a, c), d);
 *     wbst::print(wbst::rt);                 // 1 4 3 2 5
 *     return 0;
 *   }
 * ============================================================
 */
