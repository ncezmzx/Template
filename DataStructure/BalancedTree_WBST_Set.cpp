#include <bits/stdc++.h>
using namespace std;
#define int long long
namespace wbstset {

constexpr int N = 1e5 + 9;
int n, ch[N << 1][2], sz[N << 1], val[N << 1], stk[N << 1], tp, tot, rt;

void up(int x) { sz[x] = sz[ch[x][0]] + sz[ch[x][1]], val[x] = val[ch[x][1]]; }
void erase(int &x) { stk[++tp] = x, x = 0; }
int make(int x = tp ? stk[tp--] : ++tot) { return ch[x][0] = ch[x][1] = val[x] = sz[x] = 0, x; }
int node(int v, int u = make()) { return val[u] = v, sz[u] = 1, u; }
int link(int x, int y, int z = make()) { return ch[z][0] = x, ch[z][1] = y, up(z), z; }
auto cut(int x) {
  int y = ch[x][0], z = ch[x][1];
  return erase(x), make_pair(y, z);
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
  bool r = sz[ch[x][1]] > sz[ch[x][0]];
  if (!heavy(sz[ch[x][r]], sz[ch[x][!r]])) return ;
  if (need(ch[x][r], r)) rotate(ch[x][r], !r);
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
void ins(int &x, int v) {
  if (!x) return x = node(v), void();
  else if (sz[x] == 1) ch[x][0] = node(min(v, val[x])), ch[x][1] = node(max(v, val[x]));
  else ins(ch[x][v > val[ch[x][0]]], v);
  up(x), balance(x);
}
void remove(int &x, int v) {
  if (!x) return ;
  if (sz[x] == 1) return erase(x);
  else {
    bool r = v > val[ch[x][0]];
    remove(ch[x][r], v);
    if (!ch[x][r]) x = ch[x][!r];
    else up(x), balance(x);
  }
}
int rnk(int x, int v) {
  if (!x) return 0;
  int res = 0;
  while (sz[x] > 1) {
    if (val[ch[x][0]] < v) res += sz[ch[x][0]], x = ch[x][1];
    else x = ch[x][0];
  }
  return res + (val[x] < v);
}
int kth(int x, int k) {
  while (sz[x] > 1) {
    if (sz[ch[x][0]] < k) k -= sz[ch[x][0]], x = ch[x][1];
    else x = ch[x][0];
  }
  return val[x];
}

}
#undef int
/*
 * ============================================================
 * 名称：权值平衡树·集合版（确定性 Weight Balanced Tree 实现的有序多重集）
 * 复杂度：ins/remove/rnk/kth 摊还 O(log n)；无随机优先级，稳定性好
 * 用途：动态有序集合：插入 ins、删除 remove、排名 rnk（< v 的个数）、第 k 小 kth；
 *       mer(x, y) 合并两棵，balance(x) 保持平衡（子树悬殊时旋转），
 *       up/rotate/cut/erase/make/node/link 为内部维护函数
 * 来源：all.cpp 第 30633-30722 行（原样保留；注释已统一移至文件尾部）
 * ============================================================
 * 使用示例（编译时取消注释）：
 *   signed main() {
 *     cin.tie(nullptr)->sync_with_stdio(false);
 *     using namespace wbstset;
 *     ins(rt, 3), ins(rt, 1), ins(rt, 4), ins(rt, 1), ins(rt, 5);
 *     cout << rnk(rt, 4) + 1 << '\n';   // 4 的排名 = 3
 *     cout << kth(rt, 3) << '\n';       // 第 3 小 = 3
 *     remove(rt, 1);                    // 删掉一个 1
 *     cout << kth(rt, 2) << '\n';       // 第 2 小 = 3
 *     return 0;
 *   }
 * ============================================================
 */
