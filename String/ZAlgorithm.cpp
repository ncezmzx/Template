#include <bits/stdc++.h>
using namespace std;

// z[i] = s 与 s[i..] 的最长公共前缀长度（z[0] = |s|，与 ACL z_algorithm 一致）
vector<int> z_algorithm(const string& s) {
  int n = (int)s.size();
  if (n == 0) return {};
  vector<int> z(n);
  z[0] = 0;
  for (int i = 1, j = 0; i < n; i++) {  // j 为当前右端最远的 Z-box 起点
    int& k = z[i];
    k = (j + z[j] <= i) ? 0 : min(j + z[j] - i, z[i - j]);  // 先借用 [j, j+z[j]) 内的已有答案
    while (i + k < n && s[k] == s[i + k]) k++;  // 再暴力延伸
    if (j + z[j] < i + z[i]) j = i;
  }
  z[0] = n;
  return z;
}

// 泛型容器版（与 ACL 相同的模板签名，可用于 vector<int> 等）
template <class T>
vector<int> z_algorithm(const vector<T>& s) {
  int n = (int)s.size();
  if (n == 0) return {};
  vector<int> z(n);
  z[0] = 0;
  for (int i = 1, j = 0; i < n; i++) {
    int& k = z[i];
    k = (j + z[j] <= i) ? 0 : min(j + z[j] - i, z[i - j]);
    while (i + k < n && s[k] == s[i + k]) k++;
    if (j + z[j] < i + z[i]) j = i;
  }
  z[0] = n;
  return z;
}

/*
 * ============================================================
 * 名称：Z 函数 / 扩展 KMP（z_algorithm，对齐 ACL）
 * 复杂度：O(n)（每个位置均摊延伸一次）
 * 用途：z[i] = s 与 s[i..n) 的 LCP；配合拼接 "p # t" 可 O(|p|+|t|)
 *       求模式串 p 在文本 t 中每个位置的匹配长度（出现次数 / 位置）
 * 接口：z_algorithm(string) / z_algorithm(vector<T>)，0-indexed；
 *       约定 z[0] = n（ACL 同款；若需 z[0]=0 的约定自行置零）
 * 原理：维护右端点最靠右的 Z-box [j, j+z[j])；新位置 i 若落在 box 内，
 *       先取 s[i-j] 处已算好的 z 值（截断到 box 右端），再暴力延伸，
 *       每次延伸都推进全局右端点，总代价线性
 * 来源：AtCoder Library string.hpp z_algorithm 移植（逐行一致）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   string s = "abacaba";
 *   vector<int> z = z_algorithm(s);            // {7,0,1,0,3,0,1}
 *   for (int i = 0; i < (int)z.size(); i++) cout << z[i] << " \n"[i + 1 == z.size()];
 *   // 匹配：p 在 t 中的出现（拼接 p + '\x01' + t，z 值 == |p| 即命中）
 *   string p = "aba", t = "ababacaba";
 *   vector<int> z2 = z_algorithm(p + '\x01' + t);
 *   int cnt = 0;
 *   for (int i = (int)p.size() + 1; i < (int)z2.size(); i++) cnt += (z2[i] == (int)p.size());
 *   cout << cnt << '\n';                       // 3（t[0], t[2], t[6]）
 * }
 */
