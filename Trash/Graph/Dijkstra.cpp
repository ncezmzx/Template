#include <bits/stdc++.h>
using namespace std;

/*
 * 名称：堆优化 Dijkstra（单源最短路，非负权）
 * 复杂度：O((n + m) log n)
 * 用途：非负权图单源最短路；也可多源（把多个源点初始入堆即可，见原题 52143-52147）
 * 实现：priority_queue 大根堆存 {-dist, 点} 等效小根堆；出堆时用 d != dst[x] 判旧标记
 * 使用示例：
 *   es[x].emplace_back(y, z);       // 加有向边（无向图加两条）
 *   dijkstra(s);
 *   // 之后 dst[i] 为 s 到 i 的最短路
 * 来源：all.cpp 行 52116-52157（核心循环 52148-52157 原样保留）
 */
#define int long long
constexpr int N = 1e5 + 9;
int n, m, dst[N];
vector<pair<int, int>> es[N];
void dijkstra(int s) {
  memset(dst, 0x3f, sizeof dst);              // long long 下 0x3f... 为极大值
  priority_queue<pair<int, int>> q;           // 大根堆存 {-距离, 点}
  q.push({dst[s] = 0, s});
  while (!q.empty()) {
    auto [d, x] = q.top();
    d = -d, q.pop();
    if (d != dst[x]) continue;                // 旧标记（非最短路）跳过
    for (auto [y, z] : es[x])
      if (dst[y] > dst[x] + z) {              // 松弛
        dst[y] = dst[x] + z;
        q.emplace(-dst[y], y);
      }
  }
}

#ifdef DEMO
signed main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  cin >> n >> m;
  for (int i = 1, x, y, z; i <= m; ++i) {
    cin >> x >> y >> z;
    es[x].emplace_back(y, z), es[y].emplace_back(x, z);   // 无向图示例
  }
  dijkstra(1);
  for (int i = 1; i <= n; ++i)
    cout << (dst[i] >= 0x3f3f3f3f3f3f3f3f ? -1 : dst[i]) << " \n"[i == n];  // 不可达输出 -1
  cout.flush();   // 演示用：确保缓冲输出落盘（OJ 正常退出会自动 flush）
}
#endif
