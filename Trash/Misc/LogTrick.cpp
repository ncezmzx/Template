// ============================================================
// 名称：log trick —— 区间 gcd 段合并（珂朵莉树/ODT 式 gcd 修改）
// 复杂度：每次 perform 均摊 O(段数 · log V)；gcd 只会把值变小，值相同的连续段可整体
//         维护，每段值严格单调不增、最多变化 O(log V) 次，段合并总次数均摊很小
// 用途：区间"每个数取 gcd(a[i], x)"修改（a[i] = __gcd(a[i], z)），配合区间赋值/查询；
//       典型题：区间取 gcd + 区间赋值 + 单点输出
// 使用示例：见 #ifdef DEMO
// 来源：all.cpp 48974-48992（map/split/assign/perform 原样提取，mp 为全局 map<int,int>）；
//       原题初始化（all.cpp 49000-49001）：mp[1] = -1, mp[n] = -1; 再逐位 mp[i] = a[i]；
//       元素级 gcd 写法见 all.cpp 48959：if (a[i] > z) a[i] = __gcd(a[i], z);
// 注：经典"遍历右端点维护 map<gcd, 数量>"（O(n log V) 求所有子区间 gcd 个数）在 all.cpp
//     中未找到，此处为源文件实际使用的 ODT 段合并写法（同为 log trick 思想）
// ============================================================
#include <bits/stdc++.h>
using namespace std;

// —— 原样提取：all.cpp 48974-48992 ——
// 珂朵莉树（ODT）式段表：map<int,int> mp，键为段起点 l，值为段的值 v（段为 [l, next(l)-1]）
map<int, int> mp;
auto split(int x) {
  auto it = prev(mp.upper_bound(x));
  mp[x] = it->second;
}
void assign(int l, int r, int x) {
  split(l), split(r + 1);
  auto it = mp.find(l);
  while (it->first != r + 1) it = mp.erase(it);
  mp[l] = x;
}
void perform(int l, int r, int x) {
  split(l), split(r + 1);
  auto it = mp.find(l);
  while (it->first != r + 1) {
    if (it->second > x) it->second = __gcd(it->second, x);
    it = next(it);
  }
}

#ifdef DEMO
int main() {
  int n = 6;
  mp[1] = -1, mp[n] = -1;              // 原题初始化写法（all.cpp 49000）
  for (int i = 1; i <= n; ++i) mp[i] = 12;  // 初始 a[1..6] = 12
  perform(2, 5, 8);                    // a[i] = gcd(a[i], 8)
  for (int i = 1; i <= n; ++i) split(i), split(i + 1), cout << mp[i] << " \n"[i == n];
                                       // 输出：12 4 4 4 4 12
  assign(1, 3, 7);                     // 区间赋值 [1,3] = 7
  for (int i = 1; i <= n; ++i) split(i), split(i + 1), cout << mp[i] << " \n"[i == n];
                                       // 输出：7 7 7 4 4 12
}
#endif
