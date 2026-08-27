#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int N = 2e5 + 9;
int n, a[N];

int lc[N], rc[N], fa[N], stk[N];
void cartesian_build() {
  int tp = 0;
  for (int i = 1; i <= n; ++i) {
    int last = 0;
    while (tp && a[stk[tp]] > a[i]) last = stk[tp--];
    if (tp) rc[stk[tp]] = i, fa[i] = stk[tp];
    lc[i] = last;
    if (last) fa[last] = i;
    stk[++tp] = i;
  }
}

int euler[N * 2], dep[N * 2], fst[N], ecnt;
void euler_dfs(int x, int d) {
  fst[x] = ++ecnt;
  euler[ecnt] = x, dep[ecnt] = d;
  if (lc[x]) {
    euler_dfs(lc[x], d + 1);
    euler[++ecnt] = x, dep[ecnt] = d;
  }
  if (rc[x]) {
    euler_dfs(rc[x], d + 1);
    euler[++ecnt] = x, dep[ecnt] = d;
  }
}

constexpr int B = 16;
short f[1 << (B - 1)][B][B];
char done[1 << (B - 1)];
int bmask[N / B + 5], mnv[N / B + 5], mnp[N / B + 5];
int stp[20][N / B + 5], lg2[N / B + 5];

void pm1_build(int m) {
  int bcnt = (m + B - 1) / B;
  for (int b = 0; b < bcnt; ++b) {
    int L = b * B + 1, R = min(m, (b + 1) * B), len = R - L + 1;
    int mask = 0;
    for (int i = 0; i < len - 1; ++i)
      if (dep[L + i + 1] > dep[L + i]) mask |= 1 << i;
    bmask[b] = mask;
    if (!done[mask]) {
      done[mask] = 1;
      for (int i = 0; i < len; ++i) {
        f[mask][i][i] = i;
        for (int j = i + 1; j < len; ++j)
          f[mask][i][j] =
              dep[L + f[mask][i][j - 1]] < dep[L + j] ? f[mask][i][j - 1] : j;
      }
    }
    int p = f[mask][0][len - 1];
    mnv[b] = dep[L + p], mnp[b] = L + p;
    stp[0][b] = b;
  }
  for (int k = 1; (1 << k) <= bcnt; ++k)
    for (int i = 0; i + (1 << k) <= bcnt; ++i) {
      int x = stp[k - 1][i], y = stp[k - 1][i + (1 << (k - 1))];
      stp[k][i] = mnv[x] <= mnv[y] ? x : y;
    }
  for (int i = 1; i <= bcnt; ++i) lg2[i] = __lg(i);
}

int pm1_query(int l, int r) {
  int bl = (l - 1) / B, br = (r - 1) / B;
  int L1 = bl * B + 1, L2 = br * B + 1;
  int res = L1 + f[bmask[bl]][l - L1][br == bl ? r - L1 : B - 1];
  if (bl != br) {
    int rp = L2 + f[bmask[br]][0][r - L2];
    if (dep[rp] < dep[res]) res = rp;
    if (bl + 1 <= br - 1) {
      int k = lg2[br - bl - 1];
      int x = stp[k][bl + 1], y = stp[k][br - (1 << k)];
      int b = mnv[x] <= mnv[y] ? x : y;
      if (mnv[b] < dep[res]) res = mnp[b];
    }
  }
  return res;
}

void rmq_build() {
  memset(lc, 0, sizeof lc);
  memset(rc, 0, sizeof rc);
  memset(fa, 0, sizeof fa);
  memset(fst, 0, sizeof fst);
  memset(done, 0, sizeof done);
  cartesian_build();
  ecnt = 0;
  euler_dfs(stk[1], 1);
  pm1_build(ecnt);
}

int rmq_query(int u, int v) {
  int l = fst[u], r = fst[v];
  if (l > r) swap(l, r);
  return euler[pm1_query(l, r)];
}

/*
 * ============================================================
 * 名称：线性 RMQ（O(n) 预处理，O(1) 查询；来源 OI-Wiki topic/rmq）
 * 复杂度：预处理 O(n)，查询 O(1)
 * 用途：静态数组区间最小值查询（返回下标）：rmq_build() 后
 *       rmq_query(u, v) 返回 a 中 [u, v] 最小值的位置（u ≤ v）
 * 原理（OI-Wiki 线性 RMQ 三步）：
 *   1) 建笛卡尔树（小根）：区间最小值 = 两端点在笛卡尔树上的 LCA；
 *   2) 欧拉序把 LCA 转成"深度序列的 ±1 RMQ"（相邻深度差恰为 ±1）；
 *   3) ±1 RMQ 用分块 + 状压：块长 B = 16，块内只有 2^(B-1) = 32768 种
 *      ±1 形态，对每种形态暴力预处理块内任意区间最值位置（16MB）；
 *      块间建稀疏表（块数 n/B，预处理 O(n)）
 * 注意：值相等时返回最左的最小值位置（笛卡尔树用严格 > 弹出保证）；
 *       dep 数组为 ±1 序列，f 表按形态复用；数组下标 1..n
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   cin >> n;
 *   for (int i = 1; i <= n; ++i) cin >> a[i];
 *   rmq_build();
 *   int q;
 *   cin >> q;
 *   while (q--) {
 *     int l, r;
 *     cin >> l >> r;
 *     cout << a[rmq_query(l, r)] << '\n';
 *   }
 * }
 * ============================================================
 */
