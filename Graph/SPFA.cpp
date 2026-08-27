#include <bits/stdc++.h>
using namespace std;
#define int long long

// SPFA（SLF 优化）：含负权最短路、负环检测、差分约束系统
constexpr int N = 5e3 + 9, M = 2e4 + 9;
int n_, ecnt, hd[N], nxt[M], to[M];
long long wt[M], dis[N];
int cnt_[N];
bool inq[N];

void init(int n) {
  n_ = n, ecnt = 1;
  fill(hd, hd + n + 1, 0);  // 从 0 号清起（diff_constraints 以 0 为超级源）
}
void add(int u, int v, long long w) {  // 有向边
  ++ecnt, to[ecnt] = v, wt[ecnt] = w, nxt[ecnt] = hd[u], hd[u] = ecnt;
}
// 从 s 跑最短路；返回 false 表示从 s 可达负环（此时 dis 无意义）
bool run(int s) {
  fill(dis + 1, dis + n_ + 1, LLONG_MAX);
  fill(cnt_ + 1, cnt_ + n_ + 1, 0);
  fill(inq + 1, inq + n_ + 1, false);
  deque<int> q;
  dis[s] = 0, inq[s] = true, q.push_back(s);
  while (!q.empty()) {
    int u = q.front();
    q.pop_front(), inq[u] = false;
    for (int e = hd[u]; e; e = nxt[e]) {
      int v = to[e];
      if (dis[u] + wt[e] < dis[v]) {
        dis[v] = dis[u] + wt[e];
        if (!inq[v]) {
          if (++cnt_[v] >= n_) return false;  // 入队次数过多 → 负环
          inq[v] = true;
          if (!q.empty() && dis[v] < dis[q.front()]) q.push_front(v);  // SLF
          else q.push_back(v);
        }
      }
    }
  }
  return true;
}

// 差分约束系统：求 x[1..n] 满足全部 x_a - x_b <= c
// 约束 ↔ 加边 b -> a（权 c）；超级源 0 向所有点连 0 边保证全图可达
// 返回 false 无解（负环）；有解时 x[i] = dis[i]（一组最大解）
bool diff_constraints(const vector<array<long long, 3>>& cs, int n, vector<long long>& x) {
  init(n + 1);
  for (const auto& t : cs) add(t[1], t[0], t[2]);  // x_a - x_b <= c → b -> a
  for (int i = 1; i <= n; ++i) add(0, i, 0);
  if (!run(0)) return false;
  x.assign(n + 1, 0);
  for (int i = 1; i <= n; ++i) x[i] = dis[i];
  return true;
}

/*
 * ============================================================
 * 名称：SPFA 最短路（负环检测 + 差分约束系统）
 * 复杂度：通常 O(km)（k 小常数，最坏 O(nm)）；差分约束同阶
 * 用途：含负权边的单源最短路（Dijkstra 不适用的场景）、
 *       负环检测、差分约束系统求可行解；
 *       附：同余最短路（物 worthless 转移）亦用本模板建图
 * 接口：init(n) / add(u, v, w)；run(s) 返回 false 表示有负环，
 *       否则 dis[] 为最短路；diff_constraints(cs, n, x) 解
 *       形如 x_a - x_b <= c 的约束组（cs 元素为 {a, b, c}）
 * 原理：Bellman-Ford 的队列优化——只有被松弛过的点才需要再松弛；
 *       SLF（短进队首）显著降低常数；某点入队 >= n 次说明
 *       存在可无限松弛的环（负环）；差分约束把 x_a <= x_b + c
 *       视作三角形不等式建图，超级源保证连通，无解 ↔ 负环
 * 注意：判负环需从能到达全图的源（差分约束用 0 号超级源）；
 *       求最小解把约束取反（x_b - x_a <= -c 跑最长路或全部取负）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   init(3);
 *   add(1, 2, -2), add(2, 3, -2), add(3, 1, -2);  // 负环
 *   cout << run(1) << '\n';                       // 0（有负环）
 *   init(3);
 *   add(1, 2, 4), add(1, 3, -1), add(2, 3, 2);
 *   cout << run(1) << ' ' << dis[3] << '\n';      // 1 -1（1→3 直达 -1）
 *   vector<array<long long, 3>> cs{{2, 1, 3}, {3, 2, -1}, {1, 3, 5}};
 *   // x2 - x1 <= 3, x3 - x2 <= -1, x1 - x3 <= 5
 *   vector<long long> x;
 *   bool ok = diff_constraints(cs, 3, x);
 *   cout << ok << ' ' << x[2] - x[1] << ' ' << x[3] - x[2] << '\n';  // 1 且差值满足约束
 * }
 */
