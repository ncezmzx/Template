#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int N = 1e6 + 9;
int p[2 * N];
string s, t;
/*
 * ============================================================
 * 名称：Manacher 回文串
 * 复杂度：O(n)
 * 用途：求最长回文子串、统计各类回文子串。
 * 来源：all.cpp 行 57102-57123（原样保留；注释已统一移至文件尾部）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   cin >> s;
 *   // 构造: 首尾加哨兵 '&'/'?', 字符间插入 '|' (来源 all.cpp 57105-57107)
 *   t = "&|";
 *   for (auto c : s) t += c, t += '|';
 *   t += '?';
 *   int m = t.size() - 1;
 *   // 半径计算 (来源 all.cpp 57109-57114, 保留原文; 计数行注释掉)
 *   for (int i = 1, mid = 0, r = 0; i <= m; ++i) {
 *     p[i] = (i > r ? 1 : min(r - i + 1, p[2 * mid - i]));
 *     while (t[i + p[i]] == t[i - p[i]]) ++p[i];
 *     // ++pal[p[i] - 1];   // 原题统计: pal[l] = 长度为 l 的回文子串个数 (见下注释示例)
 *     if (p[i] + i - 1 > i) mid = i, r = p[i] + i - 1;
 *   }
 *   // 模板输出: 最长回文子串长度 = max(p[i]) - 1
 *   int ans = 0;
 *   for (int i = 1; i <= m; ++i) ans = max(ans, p[i]);
 *   cout << ans - 1 << '\n';
 *   return 0;
 * }
 * 原题计数示例 (来源 all.cpp 57112, 57115-57122; 需先定义 md/n/k/pal/ans 及 qpow)：
 * 需要: constexpr int md = 19930726;  int n, k, pal[N], ans = 1;
 * int qpow(int a, int b) { int r = 1; for (; b; b >>= 1, a = a * a % md) if (b & 1) r = r * a % md; return r; }
 * 在半径循环内插入: ++pal[p[i] - 1];
 * 循环结束后:
 * int s = 0;
 * for (int i = n; k && i >= 1; --i) {
 *   if (i % 2 == 0) continue;
 *   int t = min(k, s += pal[i]);
 *   k -= t;
 *   (ans *= qpow(i, t)) %= md;
 * }
 * if (k) cout << -1; else cout << ans;
 * ============================================================
 */
