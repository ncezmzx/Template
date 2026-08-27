#include <bits/stdc++.h>
using namespace std;
// ============================================================
// 名称：并查集（Disjoint Set Union，路径压缩，按方向合并）
// 复杂度：近似 O(α(n))（反阿克曼函数）
// 用途：维护连通性/集合合并，get 查根，mer 合并（x 并入 y 的集合）
// 使用示例：
//   dsu d;              // 构造时 fa[0..N-1] = i（iota 初始化）
//   d.mer(x, y);        // 合并 x、y 所在集合，返回是否真的合并
//   d.get(x);           // 查询 x 的根
// 来源：all.cpp 第 55992-56000 行
// ============================================================
#define int long long
constexpr int N = 3e5 + 9;  // 按需调整
struct dsu {
  int fa[N];
  dsu() { iota(fa, fa + N, 0); };
  int get(int x) { return x == fa[x] ? x : fa[x] = get(fa[x]); }
  bool mer(int x, int y) { // x -> y
    if ((x = get(x)) == (y = get(y))) return false;
    return fa[x] = y, true;
  }
};

#ifdef DEMO
signed main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  static dsu d;   // static：fa[N] 约 2.4MB，放栈上会爆栈（Windows 默认栈 1MB）
  d.mer(1, 2), d.mer(2, 3);
  cout << d.get(3) << ' ' << d.get(1) << '\n';  // 3 3
  cout << d.mer(1, 3) << '\n';                  // 0（已在同一集合）
  d.mer(4, 5);
  cout << d.get(4) << ' ' << d.get(5) << '\n';  // 5 5
  return 0;
}
#endif
