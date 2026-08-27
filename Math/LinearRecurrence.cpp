#include <bits/stdc++.h>
using namespace std;
#define int long long

int pw_(int x, int n, int mod) {
  int r = 1 % mod;
  x %= mod;
  if (x < 0) x += mod;
  while (n) {
    if (n & 1) r = r * x % mod;
    x = x * x % mod, n >>= 1;
  }
  return r;
}

// Berlekamp-Massey：由数列 s（模素数 mod 域）求最短线性递推 c1..cd：
// s_n = c1·s_{n-1} + c2·s_{n-2} + ... + cd·s_{n-d}；需 |s| >= 2d 项
vector<int> berlekamp_massey(const vector<int>& s, int mod) {
  int n = s.size(), L = 0, m = 0;
  vector<int> C(n + 1, 0), B(n + 1, 0), T;
  C[0] = B[0] = 1;
  int b = 1;
  for (int i = 0; i < n; ++i) {
    m++;
    int d = s[i] % mod;
    for (int j = 1; j <= L; ++j) d = (d + C[j] * s[i - j]) % mod;
    if (!d) continue;
    T = C;
    int coef = d * pw_(b, mod - 2, mod) % mod;
    for (int j = m; j <= n; ++j) C[j] = (C[j] - coef * B[j - m] % mod + mod) % mod;
    if (2 * L <= i) L = i + 1 - L, B = T, b = d, m = 0;
  }
  C.resize(L + 1);
  C.erase(C.begin());
  for (auto& x : C) x = (mod - x) % mod;  // 约定 d = s_i + ΣC_j·s_{i-j} ≡ 0 → c_j = -C_j
  return C;
}

// 多项式乘法模特征多项式（deg < d），朴素 O(d²)
vector<int> polymul(const vector<int>& a, const vector<int>& b, const vector<int>& rec, int mod) {
  int d = rec.size();
  vector<int> c(a.size() + b.size() - 1, 0);
  for (int i = 0; i < (int)a.size(); ++i)
    for (int j = 0; j < (int)b.size(); ++j) c[i + j] = (c[i + j] + a[i] * b[j]) % mod;
  for (int k = (int)c.size() - 1; k >= d; --k)  // x^k = Σ rec[i]·x^{k-i}（rec 从 x^0 高? 见注）
    for (int i = 1; i <= d; ++i) c[k - i] = (c[k - i] + c[k] * rec[i - 1]) % mod;
  c.resize(d);
  return c;
}

// 求第 k 项（0 起）：kth_term(s, k, mod)，s 需含足够多项（>= 2·递推阶）
int kth_term(const vector<int>& s, int k, int mod) {
  vector<int> rec = berlekamp_massey(s, mod);
  int d = rec.size();
  if (d == 0) return 0;  // 恒零列
  if (k < (int)s.size()) return s[k] % mod;
  vector<int> res(1, 1), base(2, 0);
  base[1] = 1;  // 多项式 x
  for (int e = k; e; e >>= 1) {  // x^k mod f(x)
    if (e & 1) res = polymul(res, base, rec, mod);
    base = polymul(base, base, rec, mod);
  }
  int ans = 0;
  for (int i = 0; i < d && i < (int)s.size(); ++i) ans = (ans + res[i] * s[i]) % mod;
  return ans;
}

/*
 * ============================================================
 * 名称：Berlekamp-Massey + Kitamasa（线性递推求第 k 项）
 * 复杂度：BM O(n²)；kth_term O(d² log k)（d 为递推阶）
 * 用途：给数列 s 的前若干项（模素数 mod），自动求出最短线性递推，
 *       再 O(d² log k) 求第 k 项（k 可达 1e18）；
 *       典型：矩阵快速幂的替代（数据满足线性递推时常数更小）
 * 原理：BM 增量维护两个多项式 C（特征式）与 B（上次失配式），
 *       失配时用 B/b 修正 C 并在长度过半时更新；求第 k 项即计算
 *       x^k mod f(x)（f 为特征多项式 x^d - Σ ci·x^{d-i}），
 *       答案 = Σ (x^k mod f)_i · s_i
 * 注意：mod 需为素数（BM 内用逆元）；s 各项需先化到 [0, mod)；
 *       s 至少 2d 项才能保证递推正确；
 *       polymul 的 rec 约定：rec[i-1] 对应 x^{d-i} 的系数（即 c_i）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   vector<int> s{1, 1, 2, 3, 5, 8, 13, 21};  // 斐波那契
 *   cout << kth_term(s, 10, 1e9 + 7) << '\n';  // 89
 *   cout << kth_term(s, 50, 1e9 + 7) << '\n';  // 365010934（s[50] = fib(51) = 20365011074）
 * }
 */
