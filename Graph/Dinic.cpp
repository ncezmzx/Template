#include <bits/stdc++.h>
using namespace std;

template <class T> struct dinic {
  vector<int> cur, hd, nxt, to, dis;
  vector<T> cap;
  static constexpr T inf = numeric_limits<T>::max();
  int tot;
  dinic(int _n, int _m) : cur(_n + 2), hd(_n + 2), nxt(_m * 2 + 2), to(_m * 2 + 2), cap(_m * 2 + 2), dis(_n + 2), tot(1) {}
  void add(int u, int v, T w) {
    nxt[++tot] = hd[u], hd[u] = tot, to[tot] = v, cap[tot] = w;
    nxt[++tot] = hd[v], hd[v] = tot, to[tot] = u, cap[tot] = 0;
  }
  T dfs(int u, int t, T flow) {
    if (u == t) return flow;
    T ret = 0;
    int du = dis[u] + 1;   // 层号提外层，避免每条边重算
    for (int i = cur[u]; i && flow; i = nxt[i]) {
      cur[u] = i;
      int v = to[i];
      T c = min(cap[i], flow);
      if (c > 0 && dis[v] == du) {
        T tmp = dfs(v, t, c);
        ret += tmp, cap[i] -= tmp, cap[i ^ 1] += tmp, flow -= tmp;
      }
    }
    if (!ret) dis[u] = -1;
    return ret;
  }
  T calc(int s, int t) {
    T ret = 0;
    vector<int> q(dis.size());   // 扁平数组队列：替代每阶段构造的 std::queue（deque 链表开销）
    while (true) {
      fill(dis.begin(), dis.end(), -1);
      int qh = 0, qt = 0;
      q[qt++] = s, dis[s] = 0;
      cur = hd;
      while (qh < qt) {
        int u = q[qh++], du = dis[u] + 1;
        for (int i = hd[u]; i; i = nxt[i]) {
          int v = to[i];
          if (dis[v] == -1 && cap[i] > 0) dis[v] = du, q[qt++] = v;
        }
      }
      if (dis[t] == -1) return ret;
      ret += dfs(s, t, inf);
    }
  }
};

/*
 * ============================================================
 * 名称：Dinic 最大流
 * 复杂度：O(V^2 E)（一般图最坏上界），实际远快于理论界；对二分图/单位容量图约 O(E sqrt(V))
 * 用途：求解有向图（或无向图按两条边拆）的 s-t 最大流
 * 来源：all.cpp 行 5779-5823（原样保留；注释已统一移至文件尾部）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   int n, m, s, t;
 *   cin >> n >> m >> s >> t;
 *   dinic<long long> f(n, m);   // 第二参数 m 为边数（add 次数）
 *   for (int i = 1; i <= m; ++i) {
 *     int u, v, w;
 *     cin >> u >> v >> w;
 *     f.add(u, v, w);
 *   }
 *   cout << f.calc(s, t) << '\n';
 *   cout.flush();   // 演示用：确保缓冲输出落盘（OJ 正常退出会自动 flush）
 * }
 * ============================================================
 */
