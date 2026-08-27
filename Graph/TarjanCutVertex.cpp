#include <bits/stdc++.h>
using namespace std;

#define int long long
constexpr int N = 200009;
int n, m, dfn[N], idx, low[N], buc[N], rt;
vector<int> es[N];
void tarjan(int x) {
  low[x] = dfn[x] = ++idx;
  int sz = 0;
  for (int y : es[x]) {
    if (!dfn[y]) {
      ++sz;
      tarjan(y), low[x] = min(low[x], low[y]);
      if (low[y] >= dfn[x] && x != rt) buc[x] = 1;
    }
    else low[x] = min(low[x], dfn[y]);
  }
  if (x == rt && sz > 1) buc[x] = 1;
}

/*
 * ============================================================
 * 名称：Tarjan 割点（无向图）
 * 复杂度：O(n + m)
 * 用途：求无向图中的割点（删除后图不再连通的点），buc[x] = 1 表示 x 是割点；
 *       判定：非根点 x 存在子节点 y 使 low[y] >= dfn[x]；根节点有至少 2 个子树（sz > 1）
 * 来源：all.cpp 行 56109-56127（原样保留；注释已统一移至文件尾部）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   cin >> n >> m;
 *   for (int i = 1, x, y; i <= m; ++i) {
 *     cin >> x >> y;
 *     es[x].push_back(y), es[y].push_back(x);
 *   }
 *   for (int i = 1; i <= n; ++i)
 *     if (!dfn[i]) rt = i, tarjan(i);
 *   for (int i = 1; i <= n; ++i)
 *     if (buc[i]) cout << i << ' ';      // 输出所有割点
 *   cout << '\n';
 * }
 * ============================================================
 */
