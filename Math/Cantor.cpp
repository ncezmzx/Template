#include <bits/stdc++.h>
using namespace std;
#define int long long

// 康托展开：长度 n 的排列 a[]（互不相同的 1..n 或任意排列）的名次（0 起，[0, n!)）
// 树状数组 O(n log n)；返回值对 mod 取模（n! 增长极快，通常 mod 998244353）
constexpr int N = 1e6 + 9;
int n_, tr_[N];
void tr_add(int x, int v) {
  for (; x <= n_; x += x & -x) tr_[x] += v;
}
int tr_sum(int x) {
  int r = 0;
  for (; x; x -= x & -x) r += tr_[x];
  return r;
}
int cantor(const vector<int>& a, int mod) {
  n_ = a.size();
  vector<int> f(n_ + 1, 1 % mod);  // f[j] = j! mod
  for (int i = 1; i <= n_; ++i) f[i] = f[i - 1] * i % mod;
  memset(tr_, 0, sizeof(int) * (n_ + 1));
  for (int i = 1; i <= n_; ++i) tr_add(i, 1);  // 可用数字集合
  int res = 0;
  for (int i = 0; i < n_; ++i) {
    res = (res + tr_sum(a[i] - 1) % mod * f[n_ - 1 - i]) % mod;
    tr_add(a[i], -1);
  }
  return res;
}

// 逆康托展开：名次 k（0 <= k < n!，可为 mod 后原值）→ 排列；树状数组上倍增找第 k 小
vector<int> uncanor(int k, int n) {
  n_ = n;
  vector<int> fac(n, 1);
  for (int i = 1; i < n; ++i) fac[i] = fac[i - 1] * i;  // n <= 20（n! <= 20! < 2^63）
  memset(tr_, 0, sizeof(int) * (n_ + 1));
  for (int i = 1; i <= n_; ++i) tr_add(i, 1);
  vector<int> a;
  for (int i = n - 1; i >= 0; --i) {
    int t = k / fac[i];  // 比当前位置小的可用数字个数
    k %= fac[i];
    int lo = 1, pos = 0;  // 树状数组倍增查第 t+1 小
    for (int j = 20; j >= 0; --j)
      if (pos + (1 << j) <= n_ && tr_[pos + (1 << j)] <= t) pos += 1 << j, t -= tr_[pos];
    ++pos;
    tr_add(pos, -1), a.push_back(pos);
  }
  return a;
}

/*
 * ============================================================
 * 名称：康托展开 / 逆康托展开
 * 复杂度：两者均 O(n log n)
 * 用途：排列 ⇄ 名次（字典序排名，0 起）：
 *       cantor(a, mod)：排列 a 的名次 mod mod；
 *       uncanor(k, n)：名次 k（0 <= k < n!）还原排列
 * 原理：名次 = Σ_i (第 i 位之前比 a_i 小的未用数字个数)·(n-1-i)!；
 *       树状数组维护未用集合；逆展开用树状数组上倍增（O(log n)）
 *       定位第 t+1 小的未用数字
 * 注意：a 为 1..n 的排列（或值互异，按相对大小）；逆展开 n <= 20
 *       （n! < 2^63）；名次 mod 时逆展开需原始 k
 * ============================================================
 * 使用示例（编译时取消注释；洛谷 P5367）：
 * signed main() {
 *   vector<int> a{3, 1, 2};                    // n = 3 的排列
 *   cout << cantor(a, 998244353) << '\n';      // 4（{123,132,213,231,312,321} 中第 5 个 → 4）
 *   auto b = uncanor(4, 3);
 *   for (int i = 0; i < 3; ++i) cout << b[i] << " \n"[i == 2];  // 3 1 2
 * }
 */
