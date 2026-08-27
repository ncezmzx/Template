#include <bits/stdc++.h>
using namespace std;

// 一般图最大匹配（带花树算法），O(n³)；1-indexed
// 颜色：col 1 = 外层（黑，队列侧，含根），col 2 = 内层（白）
constexpr int N = 509;
struct Blossom {
  int n, par[N], match_[N], pre[N], col[N], q_[N], hd, tl;
  vector<int> g[N];
  void init(int n_) {
    n = n_;
    for (int i = 0; i <= n; ++i) g[i].clear();
    fill(match_ + 1, match_ + n + 1, 0);
  }
  void add(int u, int v) { g[u].push_back(v), g[v].push_back(u); }
  int find_(int x) { return par[x] == x ? x : par[x] = find_(par[x]); }
  int lca(int x, int y) {  // 两个外层点的交替链首个公共点（花根）
    static int tim = 0, vis[N] = {0};
    ++tim;
    for (;; swap(x, y))
      if (x) {
        x = find_(x);
        if (vis[x] == tim) return x;
        vis[x] = tim;
        x = pre[match_[x]];  // 外层 → 伴侣(内层) 的 pre（外层）；根处终止
      }
  }
  void blossom(int x, int y, int l) {  // 沿 x 的链到花根 l：重定向 pre 并缩花
    while (find_(x) != l) {
      pre[x] = y;                    // 外层 x 的 pre 指向对侧（花内路径反转）
      int w = match_[x];             // 内层伴侣
      if (col[w] == 2) col[w] = 1, q_[tl++] = w;  // 花内内层点提升为外层入队
      if (find_(x) == x) par[x] = l;  // 缩入花 l
      if (find_(w) == w) par[w] = l;
      y = w;
      x = pre[w];                    // 沿链上行
    }
  }
  bool augment(int s) {  // 从未匹配点 s 找增广路
    for (int i = 1; i <= n; ++i) par[i] = i, col[i] = 0;
    hd = tl = 0, q_[tl++] = s, col[s] = 1;
    while (hd < tl) {
      int u = q_[hd++];
      for (int v : g[u]) {
        if (find_(u) == find_(v) || col[v] == 2) continue;  // 同花 / 内层点（偶环）：跳过
        if (col[v] == 1) {  // 外层-外层相邻：奇环 → 缩花
          int l = lca(u, v);
          blossom(u, v, l), blossom(v, u, l);
        } else if (!match_[v]) {  // 未匹配：沿 pre 交替翻转
          pre[v] = u;
          for (int x = v; x;) {
            int y = pre[x], z = match_[y];
            match_[x] = y, match_[y] = x;
            x = z;
          }
          return true;
        } else if (!col[v]) {  // 未染色：v 内层、其匹配点外层入队
          pre[v] = u, col[v] = 2;
          if (!col[match_[v]]) col[match_[v]] = 1, q_[tl++] = match_[v];
        }
      }
    }
    return false;
  }
  int solve() {  // 返回最大匹配数；match_[i] 为 i 的匹配点（0 为未匹配）
    int res = 0;
    for (int i = 1; i <= n; ++i)
      if (!match_[i] && augment(i)) ++res;
    return res;
  }
};

/*
 * ============================================================
 * 名称：一般图最大匹配（带花树 / Blossom 算法）
 * 复杂度：O(n³)（n 点；实际远快于上界）
 * 用途：非二分图的最大匹配（奇环场景）：
 *       solve() 匹配数；match_[i] 匹配对象（0 未匹配）
 * 原理：BFS 交替树（col 1 外层/黑 = 队列侧含根，col 2 内层/白）
 *       找增广路；外层-外层相邻形成奇环 → 缩花（并查集 par），
 *       花内内层点提升为外层继续扩展；lca 用时间戳沿
 *       pre/match 链交替上跳求花根；找到未匹配点沿 pre 翻转。
 *       外层-内层相邻必为偶环，直接跳过
 * 来源：对照 the-tourist/algo flows/blossom.cpp 的经典实现重写
 * 注意：自环/重边由 add 自行避免调用；邻接表无向对称存储
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   Blossom bm;                       // 五边形 1-2-3-4-5-1（奇环）
 *   bm.init(5);
 *   bm.add(1, 2), bm.add(2, 3), bm.add(3, 4), bm.add(4, 5), bm.add(5, 1);
 *   cout << bm.solve() << '\n';       // 2
 *   bm.init(4);                       // 三角形 + 孤立点
 *   bm.add(1, 2), bm.add(2, 3), bm.add(3, 1);
 *   cout << bm.solve() << '\n';       // 1
 * }
 */
