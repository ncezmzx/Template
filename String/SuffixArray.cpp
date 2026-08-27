#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int N = 2e5 + 9;
int n, a[N], rk[N], sa[N], id[N], buc[N], h[N], sw;   // sw：本轮第二关键字偏移（-1 表示无）
inline bool samerank(int x, int y) {
  if (rk[x] != rk[y]) return false;   // 第二关键字内联比较，免去 pair 构造
  if (sw < 0) return true;
  int px = x + sw <= n ? rk[x + sw] : 0, py = y + sw <= n ? rk[y + sw] : 0;
  return px == py;
}
void bucsort(int m) {
  memset(buc + 1, 0, m * sizeof(int));
  for (int i = 1; i <= n; ++i) ++buc[rk[i]];
  for (int i = 1; i <= m; ++i) buc[i] += buc[i - 1];
  for (int i = n; i >= 1; --i) sa[buc[rk[id[i]]]--] = id[i];   // id 已按第二关键字有序
  id[sa[1]] = 1;
  for (int i = 2; i <= n; ++i)
    id[sa[i]] = id[sa[i - 1]] + !samerank(sa[i - 1], sa[i]);
  memcpy(rk + 1, id + 1, n * sizeof(int));
}
void build_sa() {
  for (int i = 1; i <= n; ++i) rk[i] = a[i], sa[i] = id[i] = i;
  sw = -1;
  bucsort(n);
  for (int w = 1, p = 0, m = rk[sa[n]]; w < n; w <<= 1, m = rk[sa[n]], p = 0) {
    for (int j = n - w + 1; j <= n; ++j) id[++p] = j;
    for (int j = 1; j <= n; ++j)
      if (sa[j] > w) id[++p] = sa[j] - w;
    sw = w;
    bucsort(m);
    if (rk[sa[n]] == n) break;
  }
}
void build_height() {
  for (int i = 1, k = 0; i <= n; ++i) {
    if (k) --k;
    while (a[i + k] == a[sa[rk[i] - 1] + k]) ++k;
    h[rk[i]] = k;
  }
}
/*
 * ============================================================
 * 名称：后缀数组 SA（倍增 + 基数排序）与 height 数组
 * 复杂度：O(n log n) 建 SA，O(n) 建 height
 * 用途：后缀排序、LCP 查询、本质不同子串数、子串出现次数统计。
 *       注意：a[1..n] 需为 ≥1 的整数序列（字符串需先离散化映射），首次 bucsort 桶上限为 n，
 *       初始值需 ≤ n；哨兵 a[n+1] = 0（全局数组默认 0，height 循环依赖它终止比较）。
 * 来源：all.cpp 行 27981-28028（原样保留；注释已统一移至文件尾部）
 * ============================================================
 * 使用示例（编译时取消注释；字符串映射成整数序列）：
 * string str = "banana";
 * n = str.size();
 * // 字符映射成 ≥1 的整数; 首次 bucsort 桶上限为 n, 保证值 ≤ n, 故离散化:
 * vector<int> val(256, 0);
 * for (int i = 0; i < n; ++i) val[str[i]] = 1;
 * int tot = 0;
 * for (int c = 0; c < 256; ++c) if (val[c]) val[c] = ++tot;
 * for (int i = 1; i <= n; ++i) a[i] = val[str[i - 1]];
 * a[n + 1] = 0;                    // 哨兵 (全局默认已是 0, 显式写更稳)
 * build_sa();
 * build_height();
 * for (int i = 1; i <= n; ++i)
 *   cout << sa[i] << ' ' << h[i] << ' ' << str.substr(sa[i] - 1) << '\n';
 * // 本质不同子串数 = n * (n + 1) / 2 - sum(h[2..n])
 * ============================================================
 */
