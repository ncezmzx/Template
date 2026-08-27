#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int N = 4e5 + 9;
int n, sz, H;
long long sum[N << 1], tag[N << 1], len[N << 1];

void init(int n_) {
  n = n_;
  sz = 1, H = 0;
  while (sz < n) sz <<= 1, ++H;
  for (int i = 1; i <= sz * 2; ++i) sum[i] = tag[i] = 0;
  for (int i = sz; i < sz + n; ++i) len[i] = 1;
  for (int i = sz - 1; i >= 1; --i) len[i] = len[i << 1] + len[i << 1 | 1];
}

void apply(int p, long long v) {
  sum[p] += v * len[p];
  tag[p] += v;
}

void push(int p) {
  if (tag[p]) apply(p << 1, tag[p]), apply(p << 1 | 1, tag[p]), tag[p] = 0;
}

void pull(int p) { sum[p] = sum[p << 1] + sum[p << 1 | 1] + tag[p] * len[p]; }

void range_add(int l, int r, long long v) {
  l += sz - 1, r += sz - 1;
  int l0 = l, r0 = r;
  for (int i = H; i >= 1; --i) {
    push(l0 >> i);
    if ((r0 >> i) != (l0 >> i)) push(r0 >> i);
  }
  while (l <= r) {
    if (l & 1) apply(l++, v);
    if (!(r & 1)) apply(r--, v);
    l >>= 1, r >>= 1;
  }
  for (int i = 1; i <= H; ++i) {
    pull(l0 >> i);
    if ((r0 >> i) != (l0 >> i)) pull(r0 >> i);
  }
}

long long range_sum(int l, int r) {
  l += sz - 1, r += sz - 1;
  int l0 = l, r0 = r;
  for (int i = H; i >= 1; --i) {
    push(l0 >> i);
    if ((r0 >> i) != (l0 >> i)) push(r0 >> i);
  }
  long long res = 0;
  while (l <= r) {
    if (l & 1) res += sum[l++];
    if (!(r & 1)) res += sum[r--];
    l >>= 1, r >>= 1;
  }
  return res;
}

void point_set(int x, long long v) {
  range_add(x, x, v - range_sum(x, x));
}

/*
 * ============================================================
 * 名称：非递归线段树（zkw 式，区间加 + 区间和，带懒标记）
 * 复杂度：区间修改/询问 O(log n)，无递归（常数小）
 * 用途：区间加、区间求和、单点赋值（point_set 用差值实现）；
 *       需要更高性能时的线段树替代（比递归版快约 30%-50%）
 * 原理：满二叉树数组存储（sz 为 ≥ n 的 2 的幂），len[p] 预存节点段长；
 *       修改/询问前先沿叶子到根的路径 push 懒标记（只 push 与区间相关的
 *       祖先），区间覆盖的节点直接打标记，最后再 pull 回路径
 * 注意：下标 1..n；n 可为任意正整数（不足 2 的幂时补零段，len 为 0 的
 *       节点不参与）；本模板为区间加/区间和，改 apply/pull 可扩展其他
 *       可加信息（如区间最值，懒标记语义相应调整）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   init(5);
 *   range_add(2, 4, 3);                 // a = {0,3,3,3,0}
 *   cout << range_sum(1, 5) << '\n';    // 9
 *   point_set(3, 10);                   // a = {0,3,10,3,0}
 *   cout << range_sum(1, 5) << '\n';    // 16
 *   cout << range_sum(3, 3) << '\n';    // 10
 * }
 * ============================================================
 */
