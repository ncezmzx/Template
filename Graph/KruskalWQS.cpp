#include <bits/stdc++.h>
using namespace std;

#define int long long
constexpr int N = 1e5 + 9;
struct e {
  int x, y, z, c;
  bool operator<(const e r) const {
    return z == r.z ? c < r.c : z < r.z;
  }
} es[N];
int n, m, k, fa[N];
int get(int x) { return x == fa[x] ? x : fa[x] = get(fa[x]); }
pair<int, int> calc(int x) {
  iota(fa, fa + n, 0);
  for (int i = 1; i <= m; ++i)
    if (!es[i].c) es[i].z -= x;
  sort(es + 1, es + m + 1);
  int p = 0, q = 0;
  for (int i = 1, c = 0; c < n - 1 && i <= m; ++i) {
    if (get(es[i].x) == get(es[i].y)) continue;
    fa[get(es[i].x)] = get(es[i].y);
    ++c, p += es[i].z, q += !es[i].c;
  }
  for (int i = 1; i <= m; ++i)
    if (!es[i].c) es[i].z += x;
  return {p, q};
}

/*
 * ============================================================
 * 名称：WQS 二分 + Kruskal（恰好选 k 条特殊边的最小生成树）
 * 复杂度：O(m log m * logV)，每次 calc(x) 跑一遍 Kruskal
 * 用途：求"恰好包含 k 条特殊边"的最小生成树（WQS 二分 / 凸优化：每条特殊边减惩罚 x
 *       跑 Kruskal，二分出使 q >= k 的最大惩罚 l，答案 = p(l) + l * k；
 *       边权可负时二分上下界需按题目调整；c=0 特殊边同权时优先；fa 为 0-indexed）
 * 来源：all.cpp 行 28716-28755（原样保留；注释已统一移至文件尾部）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   cin >> n >> m >> k;
 *   for (int i = 1; i <= m; ++i) cin >> es[i].x >> es[i].y >> es[i].z >> es[i].c;
 *   int l = -1000, r = 1000;                 // 惩罚上下界，按题目边权范围调整
 *   while (l < r) {
 *     int mid = (l + r) >> 1;
 *     if (calc(mid).second >= k) r = mid;    // 特殊边过多 → 惩罚不够 → 调大 x
 *     else l = mid + 1;
 *   }
 *   auto [p, q] = calc(l);
 *   cout << p + l * k;                       // 还原：p 中每条特殊边已含 -l，共 k 条
 *   cout.flush();                            // 演示用：确保缓冲输出落盘（OJ 正常退出会自动 flush）
 * }
 * ============================================================
 */
