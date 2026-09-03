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

