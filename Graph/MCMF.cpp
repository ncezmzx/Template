#include <bits/stdc++.h>
using namespace std;

#define int long long
constexpr int N = 5e3 + 9, M = 5e4 + 9, inf = 0x3f3f3f3f3f3f3f3f;
struct mcmf {
  int hd[N], nxt[M * 2], to[M * 2], cap[M * 2], cst[M * 2], tot = 1, dst[N], cur[N], vst[N];
  int q[N + 2], nv = 2;   // 环形队列（容量 N+2，防源点重复入队时占满）+ 实际点数
  void add(int x, int y, int z, int c) {
    nxt[++tot] = hd[x], hd[x] = tot, to[tot] = y, cap[tot] = z, cst[tot] = c;
    nxt[++tot] = hd[y], hd[y] = tot, to[tot] = x, cap[tot] = 0, cst[tot] = -c;
    if (x >= nv) nv = x + 1;
    if (y >= nv) nv = y + 1;
  }
  pair<int, int> dfs(int x, int t, int w) {
    if (x == t) return {w, 0};
    vst[x] = 1;
    int flow = 0, cost = 0, dx = dst[x];
    for (int i = cur[x]; i && w; i = nxt[i]) {
      cur[x] = i;
      if (vst[to[i]]) continue;
      int cp = min(cap[i], w);
      if (cp > 0 && dx + cst[i] == dst[to[i]]) {
        auto [f, cs] = dfs(to[i], t, cp);
        flow += f, w -= f, cost += f * cst[i] + cs;
        cap[i] -= f, cap[i ^ 1] += f;
      }
    }
    vst[x] = 0;
    if (!flow) dst[x] = -1;
    return {flow, cost};
  }
  pair<int, int> calc(int s, int t) {
    int flow = 0, cost = 0;
    while (true) {
      memset(dst, 0x3f, nv * sizeof(int));
      memcpy(cur, hd, nv * sizeof(int));
      int qh = 0, qt = 0;   // 环形数组队列：vst 保证同时在队元素 <= N+1
      q[qt] = s, dst[s] = 0;
      if (++qt > N + 1) qt = 0;
      while (qh != qt) {
        int x = q[qh];
        if (++qh > N + 1) qh = 0;
        vst[x] = false;
        int dx = dst[x];
        for (int i = hd[x]; i; i = nxt[i]) {
          int nd = dx + cst[i];
          if (cap[i] > 0 && nd < dst[to[i]]) {
            dst[to[i]] = nd;
            if (!vst[to[i]]) {
              vst[to[i]] = true;
              q[qt] = to[i];
              if (++qt > N + 1) qt = 0;
            }
          }
        }
      }
      if (dst[t] > 1e9) return {flow, cost};
      auto [x, y] = dfs(s, t, inf);
      flow += x, cost += y;
    }
  }
};

/*
 * ============================================================
 * 名称：最小费用最大流（MCMF，SPFA 求最短路 + dfs 沿最短路推流）
 * 复杂度：每次 SPFA O(VE)，增广轮数 O(F)（F 为流量），总体 O(F * V * E) 级别
 * 用途：在保证流量最大的前提下求最小总费用；静态数组版本需按题目调整 N / M 常量
 *       （原题 N = 5e3+9, M = 5e4+9）；#define int long long 与
 *       inf = 0x3f3f3f3f3f3f3f3f 需配套使用
 * 来源：all.cpp 行 28964-29015（原样保留；注释已统一移至文件尾部；原代码为全局实例
 *       "} e;"，模板中请自行声明 mcmf e;）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   int n, m, s, t;
 *   cin >> n >> m >> s >> t;
 *   static mcmf e;   // 实例较大（约 2MB），static 放静态区避免栈溢出（原题中为全局实例 e）
 *   for (int i = 1; i <= m; ++i) {
 *     int x, y, z, c;
 *     cin >> x >> y >> z >> c;
 *     e.add(x, y, z, c);
 *   }
 *   auto [x, y] = e.calc(s, t);
 *   cout << x << ' ' << y;   // 最大流 与 最小费用
 *   cout.flush();            // 演示用：确保缓冲输出落盘（OJ 正常退出会自动 flush）
 * }
 * ============================================================
 */
