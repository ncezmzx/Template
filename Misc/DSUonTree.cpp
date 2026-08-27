#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int N = 1e5 + 9;
int n, col[N], cnt[N], sz[N], son[N], big[N], mx, sum, ans[N];
vector<int> es[N];

void dfs(int x, int ff) {
  sz[x] = 1;
  for (int y : es[x])
    if (y != ff) {
      dfs(y, x), sz[x] += sz[y];
      if (sz[y] > sz[son[x]]) son[x] = y;
    }
}

void add(int x, int ff, int d) {
  int c = col[x];
  cnt[c] += d;
  if (d > 0) {
    if (cnt[c] > mx) mx = cnt[c], sum = c;
    else if (cnt[c] == mx) sum += c;
  }
  for (int y : es[x])
    if (y != ff && !big[y]) add(y, x, d);
}

void dfs2(int x, int ff, bool keep) {
  for (int y : es[x])
    if (y != ff && y != son[x]) dfs2(y, x, false);
  if (son[x]) dfs2(son[x], x, true), big[son[x]] = 1;
  add(x, ff, 1);
  ans[x] = sum;
  if (son[x]) big[son[x]] = 0;
  if (!keep) add(x, ff, -1), mx = 0, sum = 0;
}

/*
 * ============================================================
 * 名称：树上启发式合并（DSU on tree，子树统计）
 * 复杂度：O(n log n)
 * 用途：静态树上的子树查询，例如：
 *       1) 每棵子树内出现次数最多的颜色编号和（本模板实现，CF 600E 原题）
 *       2) 子树内不同颜色数 / 第 k 小值 / 与深度相关的统计等
 *       核心思路：重儿子信息保留，轻儿子暴力重算，总复杂度 O(n log n)
 * 原理：第一遍 dfs 求重儿子；第二遍 dfs2 先处理所有轻儿子（不保留），
 *       再处理重儿子（保留），最后把轻儿子子树暴力 add 回来，得到当前子树
 *       的完整统计；add(x, ff, -1) 清空整棵子树
 * 注意：add 里的统计逻辑（mx/sum）按题目改写；cnt[] 下标为颜色编号；
 *       mx/sum 在清空时需复位（已在 !keep 分支处理）
 * 用法：dfs(1, 0); dfs2(1, 0, true); ans[x] 即 x 子树答案
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   cin >> n;
 *   for (int i = 1; i <= n; ++i) cin >> col[i];
 *   for (int i = 1, u, v; i < n; ++i) {
 *     cin >> u >> v;
 *     es[u].push_back(v), es[v].push_back(u);
 *   }
 *   dfs(1, 0), dfs2(1, 0, true);
 *   for (int i = 1; i <= n; ++i) cout << ans[i] << " \n"[i == n];
 * }
 * ============================================================
 */
