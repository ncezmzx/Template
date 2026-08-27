#include <bits/stdc++.h>
using namespace std;
// ============================================================
// 名称：树状数组 / 二叉索引树 (Fenwick / BIT)
// 复杂度：单点加 O(log n)，前缀和查询 O(log n)
// 用途：单点加 + 前缀和；区间和 = query(r) - query(l - 1)
// 使用示例：
//   fenwick bit;            // 需先给全局 n 赋值
//   bit.update(i, v);       // a[i] += v
//   bit.query(r);           // sum(a[1..r])
// 来源：all.cpp 第 6-10 行（struct fenwick，经典版）
// 调整说明：原 55555-55565 行是带时间戳回滚的 CDQ 变体，此处取 6-10 行经典版；
//           原题把坐标翻倍为 2n，故 update 上界为 2*n，可自行改为数组大小。
// ============================================================
#define int long long
constexpr int N = 2e5 + 9;  // 数组大小，按需调整
int n;                      // 数据规模（原题为 2n 坐标空间，故上界 2*n）
struct fenwick {
  int c[2 * N];
  void update(int x, int y) { for (; x <= 2 * n; x += x & -x) c[x] += y; }
  int query(int x, int r = 0) { for (; x > 0; x -= x & -x) r += c[x]; return r; }
};

#ifdef DEMO
signed main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  n = 10;
  static fenwick bit;   // static：c[2*N] 约 3MB，放栈上会爆栈（Windows 默认栈 1MB）
  bit.update(1, 3), bit.update(3, 4), bit.update(4, 2);
  // a = {3,0,4,2,0,...}
  cout << bit.query(4) << '\n';          // 9
  cout << bit.query(4) - bit.query(1) << '\n';  // 区间 [2,4] 和 = 6
  return 0;
}
#endif
