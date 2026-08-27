#include <bits/stdc++.h>
using namespace std;

// 可持久化 01-Trie：按插入顺序版本化，支持"版本区间"内的异或最值 / <= x 计数
// 套路：区间 [l,r] 异或和最大 = max s[r] ^ s[p]（p ∈ [l-1, r-1]，s 为前缀异或）
constexpr int B = 24;                          // 位宽（值 < 2^24，按需改 30 / 62）
constexpr int N = 2e5 + 9, SP = N * (B + 2) + 9;
int tot, cnt_, rt[N], tr_[SP][2], sz[SP];

void init() { tot = 0, cnt_ = 0, rt[0] = 0, tr_[0][0] = tr_[0][1] = 0, sz[0] = 0; }
int insert(int x) {  // 追加一个数 x（0 <= x < 2^B），返回新版本号
  int pre = rt[cnt_], u = ++tot;
  rt[++cnt_] = u;
  tr_[u][0] = tr_[pre][0], tr_[u][1] = tr_[pre][1], sz[u] = sz[pre] + 1;
  for (int cur = u, i = B - 1; i >= 0; --i) {
    int b = x >> i & 1, nxt = ++tot;
    pre = tr_[pre][b];
    tr_[nxt][0] = tr_[pre][0], tr_[nxt][1] = tr_[pre][1], sz[nxt] = sz[pre] + 1;
    tr_[cur][b] = nxt, cur = nxt;
  }
  return cnt_;
}
// 第 l..r 个已插入数（版本闭区间）中选 y，最大化 x ^ y（区间需非空）
int qmax(int l, int r, int x) {
  int u = rt[r], v = rt[l - 1], res = 0;
  for (int i = B - 1; i >= 0; --i) {
    int b = (x >> i & 1) ^ 1;  // 期望走与 x 相反的位
    if (sz[tr_[u][b]] - sz[tr_[v][b]] > 0) res |= 1 << i;
    else b ^= 1;
    u = tr_[u][b], v = tr_[v][b];
  }
  return res;
}
// 第 l..r 个数中值 <= x 的个数
int count_le(int l, int r, int x) {
  int u = rt[r], v = rt[l - 1], res = 0;
  for (int i = B - 1; i >= 0; --i) {
    int b = x >> i & 1;
    if (b) res += sz[tr_[u][0]] - sz[tr_[v][0]];  // 此位取 0 的（同前缀下）全部 < x
    if (sz[tr_[u][b]] - sz[tr_[v][b]] == 0) return res;  // 目标分支为空
    u = tr_[u][b], v = tr_[v][b];
  }
  return res + sz[u] - sz[v];  // 叶子：恰好等于 x 的
}

/*
 * ============================================================
 * 名称：可持久化 01-Trie（版本区间异或最值 / rank）
 * 复杂度：insert O(B)；qmax / count_le O(B)；空间 O(nB)
 * 用途：维护多重集的版本历史，按"第 l..r 次插入"做版本区间查询：
 *       区间异或和最大（前缀异或 + qmax）、区间内 <= x 计数、
 *       最大异或和带修改（两棵配合）等
 * 接口：init()；insert(x) 追加版本；qmax(l, r, x)（max x^y）；
 *       count_le(l, r, x)（<= x 个数）
 * 原理：每个版本克隆插入路径（B+1 个新节点），其余共享；
 *       版本 r 与 l-1 相减即第 l..r 个插入的数组成的集合；
 *       qmax 贪心走与期望位相反且非空的分支
 * 注意：B 需覆盖最高位（值 < 2^B）；qmax 区间不能为空；
 *       与主席树互补：值域为 2 的幂时本模板免离散化
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   init();
 *   insert(5), insert(2), insert(8), insert(6);  // 版本 1..4
 *   cout << qmax(2, 4, 9) << '\n';    // 15（9^6 = 15，集合 {2,8,6}）
 *   cout << qmax(1, 2, 1) << '\n';    // 4（{5,2} 中 1^5=4 最大）
 *   cout << count_le(2, 4, 6) << '\n';  // 2（{2,8,6} 中 <= 6 的）
 *   cout << count_le(1, 4, 7) << '\n';  // 3（{5,2,8,6} 中 <= 7 的）
 * }
 */
