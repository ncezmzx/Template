#include <bits/stdc++.h>
using namespace std;
std::map<int, int> odt;

auto spl(int p) {
  auto it = prev(odt.upper_bound(p));
  return odt.insert(it, {p, it->second});
}
void assign(int l, int r, int x) {
  auto itl = spl(l), itr = spl(++r);
  odt.erase(itl, itr), odt[l] = x;
}
int query() {
  int t = 0;
  for (auto it = odt.begin(); next(it) != odt.end(); ++it)
    t += it->second * (next(it)->first - it->first);
  return t;
}
/*
 * ============================================================
 * 名称：珂朵莉树 / 老司机树（Chtholly Tree / ODT，基于 std::map 的区间赋值）
 * 复杂度：数据随机时总复杂度 O(n log n) 摊还；区间赋值为主的操作很适合
 * 用途：区间赋值（assign）+ 按段统计（如本题的 值×段长 求和）
 * 来源：all.cpp 第 46715-46741 行（原样保留；注释已统一移至文件尾部）
 * ============================================================
 * 使用示例（编译时取消注释）：
 *   signed main() {
 *     std::cin.tie(nullptr)->sync_with_stdio(false);
 *     int n = 5;
 *     odt[1] = 1, odt[n + 1] = 0;
 *     assign(2, 4, 0);
 *     std::cout << query() << '\n'; // 1+0+0+0+1 = 2
 *     assign(3, 5, 2);
 *     std::cout << query() << '\n'; // 1+0+2+2+2 = 7
 *     return 0;
 *   }
 * ============================================================
 */
