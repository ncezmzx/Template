#include <bits/stdc++.h>
using namespace std;
#define int long long

// 最小树形图（朱刘算法）：有向图以 root 为根的外向最小生成树，O(nm)；0-indexed
constexpr int NV = 109;
struct edge {
  int u, v, w;
};
int id_[NV], vis_[NV], pre[NV], ine[NV];

// n 点 m 边，边存 e（会被就地改写），返回树形图总权；不存在返回 -1
long long zhu_liu(int n, int m, int root, vector<edge>& e) {
  long long res = 0;
  for (;;) {
    fill(ine, ine + n, LLONG_MAX);
    for (int i = 0; i < m; ++i)  // 每点最小入边
      if (e[i].u != e[i].v && e[i].w < ine[e[i].v]) ine[e[i].v] = e[i].w, pre[e[i].v] = e[i].u;
    for (int i = 0; i < n; ++i)
      if (i != root && ine[i] == LLONG_MAX) return -1;  // 有点不可达
    int cnt_ = 0;
    fill(id_, id_ + n, -1), fill(vis_, vis_ + n, -1);
    for (int i = 0; i < n; ++i) {
      if (i == root) continue;
      res += ine[i];
      int v = i;  // 沿 pre 找环（时间戳防绕）
      while (vis_[v] != i && id_[v] == -1 && v != root) vis_[v] = i, v = pre[v];
      if (v != root && id_[v] == -1) {  // 新环：统一编号（do-while，环 v→pre→…→v）
        int u = v;
        do {
          id_[u] = cnt_;
          u = pre[u];
        } while (u != v);
        ++cnt_;
      }
    }
    if (!cnt_) return res;  // 无环：结束
    for (int i = 0; i < n; ++i)
      if (id_[i] == -1) id_[i] = cnt_++;  // 环外点编号
    for (int i = 0; i < m; ++i) {  // 缩点重写：环内入边权减去已计入的最小入边
      int v = e[i].v;
      e[i].u = id_[e[i].u], e[i].v = id_[e[i].v];
      if (e[i].u != e[i].v) e[i].w -= ine[v];
    }
    n = cnt_, root = id_[root];
  }
}

/*
 * ============================================================
 * 名称：最小树形图（朱刘 / Chu-Liu 算法）
 * 复杂度：O(nm)（每轮至少缩一个环，至多 n 轮）
 * 用途：有向图以 root 为根的最小外向生成树（每个非根恰一条
 *       入边且根可达所有点），返回总权（不存在返回 -1）
 * 原理：贪心——每点取最小入边；若无环即为树形图；否则把每个
 *       入边环缩成超级点，环外指向环内点的边权减去该点原最小
 *       入边（换边代价），重复直到无环
 * 注意：e 会被就地改写（需要保留原边请拷贝）；自环自动忽略；
 *       需要更快可用 Tarjan 的 O(m log m) 堆优化版（本题规模一般够）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   // 3 点：0→1(1), 1→2(1), 2→0(1), 0→2(4)；根 0 的树形图 = 0→1 + 0→2? 入边：1:0→1(1)，2: 1→2(1) 或 0→2(4) → 总 2
 *   vector<edge> e{{0, 1, 1}, {1, 2, 1}, {2, 0, 1}, {0, 2, 4}};
 *   cout << zhu_liu(3, 4, 0, e) << '\n';   // 2（0→1, 1→2）
 *   vector<edge> e2{{1, 0, 1}};
 *   cout << zhu_liu(2, 1, 0, e2) << '\n';  // -1（点 1 不可达）
 *   vector<edge> e3{{0, 1, 5}, {1, 0, 1}, {0, 1, 3}};
 *   cout << zhu_liu(2, 3, 0, e3) << '\n';  // 3（重边取小）
 * }
 */
