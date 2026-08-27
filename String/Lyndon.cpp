#include <bits/stdc++.h>
using namespace std;

// Lyndon 分解（Duval 算法）：s = w1 w2 ... wk，wi 为 Lyndon 串且 w1 >= w2 >= ... >= wk
// Lyndon 串：字典序严格小于其所有非平凡后缀（等价：小于其所有旋转）
vector<pair<int, int>> duval(const string& s) {  // 返回因子区间 [a, b)
  int n = (int)s.size(), i = 0;
  vector<pair<int, int>> res;
  while (i < n) {
    int j = i + 1, k = i;
    while (j < n && s[k] <= s[j]) {  // s[k] 与 s[j] 比较决定周期推进
      if (s[k] < s[j]) k = i;        // 破坏周期：新因子可能更长
      else ++k;                      // 周期内字符：继续比对
      ++j;
    }
    while (i <= k) {  // 一个周期长的片段作为一个因子
      res.push_back({i, i + j - k});
      i += j - k;
    }
  }
  return res;
}

// 最小表示：字符串循环同构中字典序最小的起始下标；O(n)
int min_representation(const string& s) {
  int n = (int)s.size(), i = 0, j = 1, k = 0;
  while (i < n && j < n && k < n) {
    int a = s[(i + k) % n], b = s[(j + k) % n];
    if (a == b) ++k;
    else {
      if (a > b) i += k + 1;  // i 起始的 k+1 个都可能更大，跳过
      else j += k + 1;
      if (i == j) ++j;
      k = 0;
    }
  }
  return min(i, j) % n;
}

/*
 * ============================================================
 * 名称：Lyndon 分解（Duval）+ 最小表示
 * 复杂度：两者均 O(n)，常数极小
 * 用途：duval(s) 把 s 划分为字典序非增的 Lyndon 因子——
 *       求 runs / 本质不同子串计数、第 k 小子串、后缀数组辅助；
 *       min_representation(s) 循环移位的最小字典序起点
 *       （判断循环同构 / 循环节问题）
 * 原理：Duval 维护一个近似 Lyndon 的"准因子"[i, j) 与内部比对
 *       指针 k：s[k] < s[j] 时周期延续，> 时整体截断出一个
 *       周期长的 Lyndon 因子；最小表示用双起点 i/j 竞争 + k 比对
 * 注意：duval 返回 [a, b) 区间序列，拼接还原原串；
 *       因子个数为 O(n)；最小表示需对空串保护
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   auto f = duval("ababa");                // "ab" | "ab" | "a"（Lyndon 因子非增）
 *   for (auto& p : f) cout << p.first << ',' << p.second << ' ';
 *   cout << '\n';                           // 0,2 2,4 4,5
 *   cout << min_representation("abab") << '\n';  // 0（旋转 {abab, baba}，最小 abab）
 *   cout << min_representation("baab") << '\n';  // 1（s+s 中起点 1 = "aabb" 最小）
 * }
 */
