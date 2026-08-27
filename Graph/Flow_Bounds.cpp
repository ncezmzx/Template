#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int N = 1e5 + 9, M = 1e6 + 9, INF = 0x3f3f3f3f3f3f3f3f;
int hd[N], nxt[M], to[M], cap[M], tot = 1, cur[N], dep[N], q[N], nv = 2;

void add(int x, int y, int c) {
  nxt[++tot] = hd[x], hd[x] = tot, to[tot] = y, cap[tot] = c;
  nxt[++tot] = hd[y], hd[y] = tot, to[tot] = x, cap[tot] = 0;
  if (x >= nv) nv = x + 1;
  if (y >= nv) nv = y + 1;   // 记录实际点数，收缩每阶段的 memset/memcpy 范围
}

bool bfs(int s, int t) {
  memset(dep, -1, nv * sizeof(int));
  int qh = 0, qt = 0;        // 扁平数组队列，免掉 std::queue 的链表开销
  q[qt++] = s, dep[s] = 0;
  while (qh < qt) {
    int x = q[qh++], dx = dep[x] + 1;
    for (int i = hd[x]; i; i = nxt[i])
      if (cap[i] && dep[to[i]] == -1) dep[to[i]] = dx, q[qt++] = to[i];
  }
  return dep[t] != -1;
}

int dfs(int x, int t, int w) {
  if (x == t) return w;
  int flow = 0, dx = dep[x] + 1;
  for (int& i = cur[x]; i && w; i = nxt[i]) {
    int v = to[i];
    if (cap[i] && dep[v] == dx) {
      int k = dfs(v, t, min(w, cap[i]));
      flow += k, w -= k, cap[i] -= k, cap[i ^ 1] += k;
    }
  }
  return flow;
}

int maxflow(int s, int t) {
  int r = 0;
  while (bfs(s, t)) {
    memcpy(cur, hd, nv * sizeof(int));
    r += dfs(s, t, INF);
  }
  return r;
}

int n, m, in[N];
int SS, TT;

/*
 * ============================================================
 * 名称：上下界网络流（可行流 / 最大流 / 最小流）
 * 复杂度：O(Dinic) = O(V^2 E) 最坏，实际远快
 * 用途：边带流量下界 [l, r] 的网络流：
 *       1) 无源汇可行流：存在循环流使每条边流量 ∈ [l, r]；
 *       2) 有源汇最大流/最小流：在满足下界的前提下求 s→t 的最大/最小流量
 * 原理：每条边拆成"必流下界 l"与"可选超额 [0, r-l]"：先强行流 l，用
 *       超级源 S/汇 T 平衡每个点的出入差（in[x] = 入下界和 - 出下界和，
 *       in[x] > 0 连 S→x 容量 in[x]，否则连 x→T 容量 -in[x]）；
 *       跑 S→T 满流即有可行流；有源汇时从 t→s 连 INF 边化为无源汇，
 *       该边上流过的量即最小可行流，去掉它后再从 s→t 增广得到最大流
 * 注意：使用前清空 hd/tot/cap；SS、TT 为原图源汇，S、T 为超级源汇；
 *       in[] 记录每个点的出入下界差；边 (u,v,l,r) 需 add(u, v, r-l) 且
 *       in[u] -= l, in[v] += l
 * ============================================================
 * 使用示例（编译时取消注释；有源汇最大流，P5198 风格）：
 * signed main() {
 *   cin >> n >> m >> SS >> TT;
 *   for (int i = 1; i <= m; ++i) {
 *     int u, v, l, r;
 *     cin >> u >> v >> l >> r;
 *     in[u] -= l, in[v] += l;        // 下界先流
 *     add(u, v, r - l);              // 超额容量
 *   }
 *   int S = n + 1, T = n + 2, sum = 0;
 *   for (int i = 1; i <= n; ++i) {
 *     if (in[i] > 0) add(S, i, in[i]), sum += in[i];
 *     else if (in[i] < 0) add(i, T, -in[i]);
 *   }
 *   add(TT, SS, INF);                // 有源汇转无源汇
 *   if (maxflow(S, T) != sum) return cout << "NO\n", 0;
 *   int flow = 0;
 *   for (int i = hd[SS]; i; i = nxt[i])
 *     if (to[i] == TT) flow = cap[i];        // TT→SS 边上的流量 = 最小可行流
 *   flow += maxflow(SS, TT);                 // 继续增广得最大流
 *   cout << flow << '\n';
 * }
 * ============================================================
 */
