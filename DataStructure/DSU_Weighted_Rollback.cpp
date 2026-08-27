#include <bits/stdc++.h>
using namespace std;

// 带权并查集：维护 d[x] = x 到根的累计边权（可加群，如 mod 加法/异或）
constexpr int N = 2e5 + 9;
struct wdsu {
  int p[N];
  long long d[N];
  void init(int n) {
    for (int i = 0; i <= n; ++i) p[i] = i, d[i] = 0;
  }
  int find(int x) {  // 路径压缩并累计权
    if (p[x] == x) return x;
    int r = find(p[x]);
    d[x] += d[p[x]], p[x] = r;
    return r;
  }
  // 合并约束 y = x + w（x --w--> y）：返回是否与已有约束相容
  bool merge(int x, int y, long long w) {
    int rx = find(x), ry = find(y);
    if (rx == ry) return d[y] - d[x] == w;
    p[rx] = ry, d[rx] = d[y] - d[x] - w;  // val[rx]-val[ry]（由 val[y]-val[x]=w 推出）
    return true;
  }
  long long rel(int x, int y) { return d[y] - d[x]; }  // val[y] - val[x]（需已同根：先 find）
};

// 可撤销并查集：按秩合并不路径压缩，rollback 到历史大小
struct rdsu {
  int p[N], rk[N];
  vector<pair<int, int>> hist;  // (被挂的根, 秩是否增加)
  void init(int n) {
    for (int i = 0; i <= n; ++i) p[i] = i, rk[i] = 0;
    hist.clear();
  }
  int find(int x) {
    while (p[x] != x) x = p[x];
    return x;
  }
  bool merge(int a, int b) {  // 返回是否发生合并（已连通则不入栈）
    int ra = find(a), rb = find(b);
    if (ra == rb) return hist.push_back({-1, 0}), false;
    if (rk[ra] < rk[rb]) swap(ra, rb);
    p[rb] = ra, hist.push_back({rb, rk[ra] == rk[rb]});
    rk[ra] += (rk[ra] == rk[rb]);
    return true;
  }
  void rollback(size_t target) {  // 撤销到 hist.size() == target
    while (hist.size() > target) {
      auto pr = hist.back();
      hist.pop_back();
      if (pr.first < 0) continue;
      if (pr.second) rk[p[pr.first]]--;  // 秩回退
      p[pr.first] = pr.first;
    }
  }
};

/*
 * ============================================================
 * 名称：带权并查集 / 可撤销并查集
 * 复杂度：wdsu.merge/find 均摊 O(α)；rdsu.find O(log n)（无路径
 *       压缩），merge/rollback O(1)
 * 用途：wdsu：维护带相对关系的合并（食物链/奇偶性/差分约束式
 *       关系 y = x + w，权为可加群：模加法、异或等）；
 *       rdsu：离线删边/线段树分治/整体二分中需要回滚的连通性
 * 接口：wdsu：merge(x, y, w)（y = x + w，返回是否相容）、
 *       rel(x, y)（同根时 val[y] - val[x]）；
 *       rdsu：merge(a, b)、rollback(hist.size() 快照)
 * 原理：wdsu 路径压缩时把沿途权值累加到根；合并时由
 *       d[rx] = w + d[y] - d[x] 推出新根链上的权；
 *       rdsu 按秩合并保证树高 O(log n)，撤销按栈逆操作
 * 注意：wdsu 的权群需满足可加可减（异或群时把 + 换 ^、- 换 ^）；
 *       rdsu 不能路径压缩（会破坏可撤销性）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   wdsu w;
 *   w.init(5);
 *   w.merge(1, 2, 5), w.merge(2, 3, -2);   // val2 = val1+5, val3 = val2-2
 *   w.find(1), w.find(3);                  // rel 前先 find 压缩路径（d 才是到根全距）
 *   cout << w.rel(1, 3) << '\n';           // 3（val3 - val1 = 5-2）
 *   cout << w.merge(1, 3, 3) << w.merge(1, 3, 4) << '\n';  // 1 0（相容 / 矛盾）
 *   rdsu r;
 *   r.init(5);
 *   size_t snap = r.hist.size();
 *   r.merge(1, 2), r.merge(2, 3);
 *   cout << (r.find(1) == r.find(3)) << '\n';  // 1
 *   r.rollback(snap);
 *   cout << (r.find(1) == r.find(3)) << '\n';  // 0
 * }
 */
