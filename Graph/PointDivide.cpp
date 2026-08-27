#include <bits/stdc++.h>
using namespace std;

#define int long long
constexpr int N = 1e5 + 9;
int n, m, a[N], mi[20][N], dfn[N], idx, rt, fa[N], ban[N], sz[N], dep[N], cnt;
vector<int> es[N], c[2][N];
void update(int u, int x, int y, int o) {
  for (++x; x < c[o][u].size(); x += x & -x) c[o][u][x] += y;
}
int query(int u, int x, int o, int r = 0) {
  for (++x, x = min(x, (int) c[o][u].size() - 1); x > 0; x -= x & -x) r += c[o][u][x];
  return r;
}
void dfs(int x, int ff) {
  mi[0][dfn[x] = ++idx] = ff, dep[x] = dep[ff] + 1;
  for (int y : es[x])
    if (y ^ ff) dfs(y, x);
}
int get(int x, int y) { return dfn[x] < dfn[y] ? x : y; }
int lca_built = -1;   // mi 稀疏表惰性构建（原实现缺此步，d>=1 层全 0 导致 dist 错）
void build_mi() {
  for (int k = 1; k < 20; ++k)
    for (int i = 1; i + (1 << k) - 1 <= idx; ++i)
      mi[k][i] = get(mi[k - 1][i], mi[k - 1][i + (1 << (k - 1))]);
  lca_built = idx;
}
int lca(int x, int y) {
  if (x == y) return x;
  if (lca_built != idx) build_mi();
  if ((x = dfn[x]) > (y = dfn[y])) swap(x, y);
  int d = __lg(y - ++x + 1);
  return get(mi[d][x], mi[d][y - (1 << d) + 1]);
}
int dist(int x, int y) { return dep[x] + dep[y] - 2 * dep[lca(x, y)]; }
void dfs3(int x, int ff) {
  sz[x] = 1;
  for (int y : es[x])
    if (y ^ ff && !ban[y]) dfs3(y, x), sz[x] += sz[y];
}
void dfs2(int x, int ff) {
  int mx = cnt - sz[x];
  for (int y : es[x])
    if (y ^ ff && !ban[y]) dfs2(y, x), mx = max(mx, sz[y]);
  if (mx * 2 <= cnt) rt = x;
}
void dfs1(int x, int ff) {
  dfs3(x, 0), cnt = sz[x], dfs2(x, 0);
  ban[rt] = 1, fa[rt] = ff, c[0][rt].resize(cnt + 2), c[1][rt].resize(cnt + 2);
  int u = rt;
  for (int y : es[u])
    if (y ^ ff && !ban[y]) dfs1(y, u);
}
int query(int x, int y) {
  int res = 0;
  for (int pr = 0, i = x; i; pr = i, i = fa[i]) {
    int d = dist(i, x);
    res += query(i, y - d, 0);
    if (pr) res -= query(pr, y - d, 1);
  }
  return res;
}
void update(int x, int y) {
  for (int i = x; i; i = fa[i]) update(i, dist(x, i), y, 0);
  for (int i = x; fa[i]; i = fa[i]) update(i, dist(x, fa[i]), y, 1);
}

/*
 * ============================================================
 * 名称：点分治（动态点分树 / 重心分治）
 * 复杂度：建点分树 O(n log n)（每层 O(当前连通块大小)），单次查询/修改 O(log n * log n)
 * 用途：统计树上与"距离"相关的信息（例：距点 x 不超过 k 的点的权值和，支持单点修改）；
 *       对每个重心 rt 开两个 BIT c[0][rt]（各距离贡献）/ c[1][rt]（到父重心的距离，
 *       用于容斥）；update/query 沿点分树向上更新 / 容斥求和；开头 LCA 部分
 *       （dfs/get/lca/dist）供 dist(x, y) 使用
 * 来源：all.cpp 行 7965-8041（原样保留；注释已统一移至文件尾部）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   cin >> n >> m;
 *   for (int i = 1; i <= n; ++i) cin >> a[i];
 *   for (int i = 1, x, y; i < n; ++i) {
 *     cin >> x >> y, es[x].push_back(y), es[y].push_back(x);
 *   }
 *   dfs(1, 0);
 *   for (int i = 1; i < 20; ++i)
 *     for (int j = 1; j + (1 << i) - 1 <= n; ++j) mi[i][j] = get(mi[i - 1][j], mi[i - 1][j + (1 << (i - 1))]);
 *   dfs1(1, 0);
 *   for (int i = 1; i <= n; ++i) update(i, a[i]);   // 初始化
 *   while (m--) {
 *     int o, x, y;
 *     cin >> o >> x >> y;
 *     if (o == 0) cout << query(x, y) << '\n';      // 查询距 x 不超过 y 的权值和
 *     else update(x, y - a[x]), a[x] = y;           // 修改点 x 权值为 y
 *   }
 *   cout.flush();   // 演示用：确保缓冲输出落盘（OJ 正常退出会自动 flush）
 * }
 * ============================================================
 */
