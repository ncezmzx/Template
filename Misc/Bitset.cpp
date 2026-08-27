#include <bits/stdc++.h>
using namespace std;
#define int long long
constexpr int N = 3e5 + 9;
string s, t;
bitset<N> a[26];
/*
 * ============================================================
 * 名称：bitset 技巧 —— 通配符字符串匹配（26 个 bitset 按字符建位 + 逐位与 + 左移）
 * 复杂度：O(|S| * |T| / 64)（每次 ans &= a[c] 与 ans <<= 1 均为 O(|T|/64)）
 * 用途：模式串 t 含 '*'（可匹配任意一个字符）时求所有匹配起点；用 bitset 并行维护
 *       "每个位置是否仍可能"的布尔匹配，为每个字符 c 建 bitset a[c]，每读入 s 的
 *       一个字符就 ans &= a[c] 并左移一位。demo 输入仅限小写字母与 '*'。
 * 来源：all.cpp 行 33335-33356（原样保留；注释已统一移至文件尾部）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   cin >> s >> t;
 *   for (int i = 0; i < t.size(); ++i) {
 *     if (t[i] == '*') {
 *       for (int j = 0; j < 26; ++j) a[j][i] = 1;
 *     }
 *     else a[t[i] - 'a'][i] = 1;
 *   }
 *   bitset<N> ans;
 *   ans.set();
 *   for (int i = 0; i < s.size(); ++i) {
 *     if (s[i] != '*') ans &= a[s[i] - 'a'];
 *     ans <<= 1;
 *   }
 *   for (int i = 1; i <= t.size(); ++i)
 *     if (ans[i]) cout << i - s.size() + 1 << ' ';
 * }
 * ============================================================
 */
