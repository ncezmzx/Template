// ============================================================
// 名称: KMP 前缀函数 / 失配表 (nxt)
// 复杂度: 构建 O(n), 匹配 O(n + m)
// 用途: 模式串匹配、最小循环节、border 统计
// 使用示例: 见下方 #ifdef DEMO 的 main (字符串均为 1-indexed,
//          即 s = ' ' + s, s[0] 为占位字符)
// 来源: all.cpp 行 57134-57137 (构建 nxt)
//       all.cpp 行 11199-11202 (匹配循环)
//       all.cpp 行 6476-6499 (KMP 自动机变体, 注释)
// ============================================================
#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int N = 1e6 + 9;
int n, nxt[N];   // nxt[i]: s[1..i] 的最长 border 长度 (失配跳转)
string s;        // 模式串, 使用时 s = ' ' + s

// 构建 nxt (来源 all.cpp 57134-57137, 保留原文):
// s 为 1-indexed, s[0] 占位; n = |s| - 1
void build_nxt() {
  for (int j = 0, i = 2; i <= n; ++i) {
    while (j && s[j + 1] != s[i]) j = nxt[j];
    j = nxt[i] = j + (s[j + 1] == s[i]);
  }
}

// 匹配 (来源 all.cpp 11199-11202, 保留原文):
// t 为模式串 (1-indexed, nxt 需已对 t 构建), s 为文本 (1-indexed)
// 返回 t 在 s 中出现的次数
int match(const string &t, const string &s) {
  int m = t.size() - 1, M = s.size() - 1, cnt = 0;
  for (int i = 1, j = 0; i <= M; ++i) {
    while (j && t[j + 1] != s[i]) j = nxt[j];
    if ((j += (t[j + 1] == s[i])) == m) ++cnt;  // 命中完整模式串 (可改为记录位置 i)
  }
  return cnt;
}

// ========== 变体: KMP 自动机 to[i][c] (来源 all.cpp 6476-6499, 注释) ==========
// 原题为二进制串 ('0'/'1'), to[N][2]; 其他字符集把 2 换成字符集大小即可。
// 构建 fail 后, to[i][c] 表示状态 i 读入字符 c 后的新状态, 匹配时每步 O(1):
//
// int fail[N], to[N][2];
// string s;                       // 1-indexed
// s = ' ' + s;
// for (int i = 2, j = 0; i <= n; ++i) {
//   while (j && s[j + 1] != s[i]) j = fail[j];
//   fail[i] = j += s[j + 1] == s[i];
// }
// for (int i = 0; i <= n; ++i) {
//   for (int j = 0; j < 2; ++j) to[i][j] = to[fail[i]][j];
//   if (i < n) to[i][s[i + 1] - '0'] = i + 1;
// }
// // 匹配: j = 0; for (char c : text) { j = to[j][c - '0']; if (j == n) { 匹配到; } }
// // 注: 原题中 dep[]/f[] 是问题相关统计, 已省略。

#ifdef DEMO
signed main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  string txt, pat;
  cin >> txt >> pat;             // 先文本后模式
  n = pat.size(), s = ' ' + pat; // nxt 对全局 s (模式) 构建
  build_nxt();
  cout << match(s, ' ' + txt) << '\n';  // 模式在文本中的出现次数
  return 0;
}
#endif
