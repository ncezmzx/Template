#include <bits/stdc++.h>
using namespace std;
#define int long long

int pw(int x, int n, int p) {
  int r = 1 % p;
  x %= p;
  while (n) {
    if (n & 1) r = r * x % p;
    x = x * x % p, n >>= 1;
  }
  return r;
}

// 分解 n（返回素因子，试除法，n <= 1e12 可行）
vector<int> factorize(int n) {
  vector<int> fs;
  for (int i = 2; i * i <= n; ++i)
    if (n % i == 0) {
      fs.push_back(i);
      while (n % i == 0) n /= i;
    }
  if (n > 1) fs.push_back(n);
  return fs;
}

// 最小原根（p 为素数；g 是原根 iff 对 p-1 的每个素因子 q，g^{(p-1)/q} != 1 (mod p)）
int min_primitive_root(int p) {
  if (p == 2) return 1;
  vector<int> fs = factorize(p - 1);
  for (int g = 2;; ++g)
    if (all_of(fs.begin(), fs.end(), [&](int q) { return pw(g, (p - 1) / q, p) != 1; })) return g;
}

// 所有原根 = g^k（gcd(k, p-1) = 1），共 phi(p-1) 个
vector<int> primitive_roots(int p) {
  vector<int> res;
  if (p == 2) return {1};
  int g = min_primitive_root(p);
  for (int k = 1, cur = g; k < p - 1; ++k, cur = cur * g % p)
    if (__gcd(k, p - 1) == 1) res.push_back(cur);
  sort(res.begin(), res.end());
  return res;
}

/*
 * ============================================================
 * 名称：原根（最小原根 / 全体原根）
 * 复杂度：试除分解 p-1 为 O(√p)；枚举判定 O(√p + ω(p)·log p)
 * 用途：min_primitive_root(p) 求 p（素数）的最小原根 g；
 *       primitive_roots(p) 列出全部 phi(p-1) 个原根；
 *       原根用途：NTT 模数的 g、离散对数（指标）等
 * 原理：g 是模 p 原根 iff g 的阶为 p-1，iff 对 p-1 的每个素因子 q，
 *       g^{(p-1)/q} != 1 (mod p)；最小原根普遍很小（< 300 量级）
 * 注意：p 需为素数（p = 2 返回 1）；p-1 分解用试除，p <= 1e12 可行
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   cout << min_primitive_root(998244353) << '\n';  // 3
 *   cout << min_primitive_root(7) << '\n';          // 3（3,5 也是原根）
 *   cout << primitive_roots(7).size() << '\n';      // 2（3 与 5）
 * }
 */
