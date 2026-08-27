#include <bits/stdc++.h>
using namespace std;

// 欧拉路 / 欧拉回路（Hierholzer，迭代实现）
// 无向：链式前向星成对存边（i 与 i^1），used 标记；有向：链表指针删边
constexpr int N = 2e5 + 9, M = 4e5 + 9;

struct euler_undirected {
  int n, ecnt = 1, hd[N], nxt[M], to[M], deg[N];
  bool used[M];
  void init(int n_) {
    n = n_, ecnt = 1;
    fill(hd + 1, hd + n + 1, 0), fill(deg + 1, deg + n + 1, 0);
    fill(used, used + M, false);  // 复位边标记（多测复用）
  }
  void add(int u, int v) {  // 无向边（重边自环均可）
    ++ecnt, to[ecnt] = v, nxt[ecnt] = hd[u], hd[u] = ecnt, ++deg[u];
    ++ecnt, to[ecnt] = u, nxt[ecnt] = hd[v], hd[v] = ecnt, ++deg[v];
  }
  int odd_cnt() {  // 奇度顶点数（0：有回路；2：有路径；否则无解）
    int c = 0;
    for (int i = 1; i <= n; ++i) c += deg[i] & 1;
    return c;
  }
  // 返回从 s 出发用尽所有边的顶点序列（空表示不存在）；调用前需保证
  // odd_cnt() 为 0（s 任意非孤立点）或 2（s 为奇度点），且边所在点连通
  vector<int> path(int s) {
    vector<int> stk, res;
    stk.push_back(s);
    while (!stk.empty()) {
      int v = stk.back();
      int& e = hd[v];
      while (e && used[e]) e = nxt[e];
      if (e) {
        used[e] = used[e ^ 1] = true;
        stk.push_back(to[e]);
        e = nxt[e];
      } else
        res.push_back(v), stk.pop_back();
    }
    reverse(res.begin(), res.end());
    return res;
  }
};

struct euler_directed {
  int n, ecnt = 1, hd[N], nxt[M], to[M], in_[N], out_[N];
  void init(int n_) {
    n = n_, ecnt = 1;
    fill(hd + 1, hd + n + 1, 0), fill(in_ + 1, in_ + n + 1, 0), fill(out_ + 1, out_ + n + 1, 0);
  }
  void add(int u, int v) {
    ++ecnt, to[ecnt] = v, nxt[ecnt] = hd[u], hd[u] = ecnt, ++out_[u], ++in_[v];
  }
  bool has_path(int& s) {  // 检查入出度条件；s 置为起点（成功时）
    int a = 0, b = 0;
    s = 0;
    for (int i = 1; i <= n; ++i) {
      int d = out_[i] - in_[i];
      if (d > 1 || d < -1) return false;
      if (d == 1) ++a, s = i;
      if (d == -1) ++b;
      if (!s && (out_[i] || in_[i])) s = i;
    }
    if (a == 0 && b == 0) return true;                     // 回路（s 已取非孤立点）
    return a == 1 && b == 1;                               // 路径
  }
  // 用尽所有边的顶点序列（空表示不存在；还需边所在点从 s 可达，见注释）
  vector<int> path(int s) {
    vector<int> stk, res;
    stk.push_back(s);
    while (!stk.empty()) {
      int v = stk.back();
      if (hd[v]) {
        int e = hd[v];
        hd[v] = nxt[e];  // 删边（有向无需 used）
        stk.push_back(to[e]);
      } else
        res.push_back(v), stk.pop_back();
    }
    reverse(res.begin(), res.end());
    return res;
  }
};

/*
 * ============================================================
 * 名称：欧拉路 / 欧拉回路（Hierholzer 迭代）
 * 复杂度：O(n + m)（每条边访问一次）
 * 用途：无向/有向图的欧拉路径与欧拉回路（每条边恰经过一次）：
 *       无向存在条件：连通（忽略孤立点）+ 奇度点 0 个（回路）
 *       或 2 个（路径，起点为奇度点）；
 *       有向存在条件：连通 + 入出度差全 0（回路）或恰一对
 *       +1/-1（路径，起点为 out-in=1 的点）
 * 接口：init(n) / add(u, v)；odd_cnt()（无向度判定）；
 *       has_path(s)（有向度判定并给出起点 s）；
 *       path(s) 返回顶点序列（含起终点，长 m+1；空表示失败）
 * 原理：Hierholzer——从起点沿未用边深入，死胡同顶点依次入答案栈，
 *       回退时换边；最终答案逆序即欧拉序（迭代实现免爆栈）
 * 注意：path 不检查连通性（度数条件满足但不连通时会输出
 *       "部分路径"且长度 < m+1，可据此判断）；自环/重边均支持；
 *       对象含大数组（数 MB），必须定义为全局或 static（勿放栈上）
 * ============================================================
 * 使用示例（编译时取消注释；对象为 static 防栈溢出）：
 * signed main() {
 *   static euler_undirected g;
 *   g.init(4);
 *   g.add(1, 2), g.add(2, 3), g.add(3, 4), g.add(4, 1), g.add(1, 3);
 *   cout << g.odd_cnt() << '\n';              // 2（1 与 3 为奇度 → 路径）
 *   auto p = g.path(1);                       // 例如 1 2 3 4 1 3（顺序不唯一）
 *   for (int v : p) cout << v << ' ';         // 长度 = 6 = m + 1
 *   cout << '\n';
 *   static euler_directed h;
 *   h.init(3);
 *   h.add(1, 2), h.add(2, 3), h.add(3, 1);
 *   int s;
 *   cout << h.has_path(s) << ' ' << s << '\n';  // 1 1（回路）
 * }
 */
