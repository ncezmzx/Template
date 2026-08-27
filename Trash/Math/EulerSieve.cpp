#include <bits/stdc++.h>
using namespace std;
/*
 * 名称：线性筛 + 欧拉函数（O(n) 同时求素数表与 phi）
 * 复杂度：O(n)
 * 用途：求 1..n 的全部素数（存于 pri[1..c]）与欧拉函数 phi[i]（1..i 中与 i 互质的个数）
 * 使用示例：euler(n); 之后 phi[i]、pri[1..c]、c 均可用
 * 来源：all.cpp 25674-25711（原代码写在 main 内，此处按要求封装为 void euler(int n)，
 *       循环边界由 i < N 参数化为 i <= n；25711 的前缀和部分问题相关，已移到注释）
 */
#define int long long
constexpr int N = 1e5 + 9;
int phi[N], pri[N], c, vst[N];
void euler(int n) {
  phi[1] = 1;
  for (int i = 2; i <= n; ++i) {
    if (!vst[i]) pri[++c] = i, phi[i] = i - 1;
    for (int j = 1; j <= c && pri[j] * i <= n; ++j) {
      vst[i * pri[j]] = 1;
      if (i % pri[j] == 0) {
        phi[i * pri[j]] = pri[j] * phi[i];
        break;
      }
      phi[i * pri[j]] = phi[pri[j]] * phi[i];
    }
  }
}
// 原问题中的 phi 前缀和（问题相关，一般模板不需要）：
// for (int i = 1; i < N; ++i) phi[i] += phi[i - 1];  // 来源 all.cpp 25711
#ifdef DEMO
signed main() {
  euler(100000);
  cout << c << '\n';  // 1..1e5 素数个数
  cout << phi[100] << ' ' << phi[97] << '\n';  // 40 96
}
#endif
