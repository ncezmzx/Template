#include <bits/stdc++.h>
using namespace std;
#define int long long

// 60 位线性基（子集异或空间），O(60) 单次操作
struct xor_basis {
  int b[60] = {}, cnt = 0;  // b[i]：最高位为 i 的基向量（默认零初始化）
  void clear() { memset(b, 0, sizeof b), cnt = 0; }
  bool insert(int x) {  // 插入 x；返回是否使秩增大
    for (int i = 59; i >= 0; --i)
      if (x >> i & 1) {
        if (!b[i]) return b[i] = x, ++cnt, true;
        x ^= b[i];
      }
    return false;
  }
  bool contains(int x) {  // x 是否可由已有基异或表出（0 恒可）
    for (int i = 59; i >= 0; --i)
      if (x >> i & 1) {
        if (!b[i]) return false;
        x ^= b[i];
      }
    return true;
  }
  int qmax(int x = 0) {  // x 异或子集的最大值
    for (int i = 59; i >= 0; --i)
      if ((x ^ b[i]) > x) x ^= b[i];
    return x;
  }
  int qmin(int x = 0) {  // x 异或子集的最小值
    for (int i = 0; i < 60; ++i)
      if ((x ^ b[i]) < x) x ^= b[i];
    return x;
  }
  void merge(const xor_basis& o) {
    for (int i = 0; i < 60; ++i)
      if (o.b[i]) insert(o.b[i]);
  }
  void normalize() {  // 消元：使 b[i] 在更低基位上无 1（qkth 前置）
    for (int i = 0; i < 60; ++i)
      if (b[i])
        for (int j = i - 1; j >= 0; --j)
          if (b[j] && (b[i] >> j & 1)) b[i] ^= b[j];
  }
  int qkth(int k) {  // 全部 2^cnt 个子集异或值从小到大第 k 个（k 从 1 起，含 0）
    if (k > (int)1 << cnt) return -1;
    normalize();
    int res = 0;
    for (int i = 0, j = 0; i < 60; ++i)
      if (b[i]) {
        if (k - 1 >> j & 1) res ^= b[i];
        ++j;
      }
    return res;
  }
  int cnt_le(int x) {  // 子集异或中 <= x 的个数（值域有序，二分第 k 小）
    int lo = 1, hi = (int)1 << cnt;  // 全部 <= x 的最靠右排名
    while (lo < hi) {
      int mid = lo + hi + 1 >> 1;
      qkth(mid) <= x ? lo = mid : hi = mid - 1;
    }
    return qkth(lo) <= x ? lo : 0;
  }
};

/*
 * ============================================================
 * 名称：线性基（60 位异或空间）
 * 复杂度：插入/查询 O(60)；merge O(60^2)；qkth/cnt_le O(60^2)
 * 用途：维护一组数的子集异或值域：
 *       insert 插入；contains 判断可表出；qmax/qmin 查最值；
 *       qkth 第 k 小（含 0）；cnt_le 值 <= x 的子集异或计数；
 *       merge 合并两基；clear 清空
 * 原理：高斯消元思想——每个基向量 b[i] 是当前最高位为 i 的代表元；
 *       查询从高位贪心（qmax）或低位消 1（qmin）；qkth 先消元成
 *       规范形（低基位无 1），第 k-1 的二进制位按基序号（从低到高）
 *       决定是否取该基（排序 = 顶位决定序，基序号即名次位）
 * 注意：插入的数需 < 2^60；cnt 为秩，可表出值共 2^cnt 个（含 0）
 * ============================================================
 * 使用示例（编译时取消注释；洛谷 P3812）：
 * xor_basis bs;
 * signed main() {
 *   bs.insert(1), bs.insert(2), bs.insert(3);   // {1,2,3}
 *   cout << bs.qmax() << '\n';                  // 3（1^2=3, 1^3=2, 2^3=1, 1^2^3=0）
 *   cout << bs.contains(0) << ' ' << bs.contains(4) << '\n';  // 1 0
 *   cout << bs.qkth(1) << ' ' << bs.qkth(4) << '\n';          // 0 3
 *   cout << bs.cnt_le(2) << '\n';               // 3（0,1,2）
 * }
 */
