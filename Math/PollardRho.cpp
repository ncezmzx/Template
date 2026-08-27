#include <bits/stdc++.h>
using namespace std;
#define int long long

int mul_mod(int x, int y, int md) { return (__int128) x * y % md; }

int qpow(int a, int b, int md) {
  int r = 1;
  for (; b; b >>= 1, a = mul_mod(a, a, md))
    if (b & 1) r = mul_mod(r, a, md);
  return r;
}

bool is_prime(int n) {
  if (n == 2) return true;
  if (n <= 1 || (n & 1 ^ 1)) return false;
  int u = n - 1, t = 0;
  while (u & 1 ^ 1) u >>= 1, ++t;
  auto chk = [&](int a) -> bool {
    int k = qpow(a % n, u, n);
    if (k <= 1) return true;
    for (int i = 0; i < t; ++i) {
      if (k == n - 1) return true;
      k = mul_mod(k, k, n);
    }
    return false;
  };
  static constexpr int bas[7] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
  for (int i : bas) {
    if (n % i == 0) return n == i;
    if (!chk(i)) return false;
  }
  return true;
}

int pollard_rho(int n) {
  if (n % 2 == 0) return 2;
  if (n % 3 == 0) return 3;
  static mt19937_64 rng(random_device{}());
  while (true) {
    int c = rng() % (n - 1) + 1;
    int x = rng() % (n - 1) + 1, y = x, d = 1;
    auto f = [&](int v) { return (mul_mod(v, v, n) + c) % n; };
    while (d == 1) {
      // 批量 Floyd：|x-y| 连乘 128 步模 n 后只做一次 gcd，摊薄逐步 gcd 的除法开销
      int q = 1;
      for (int i = 0; i < 128 && d == 1; ++i) {
        x = f(x), y = f(f(y));
        if (x == y) {
          d = __gcd(q, n);          // 指针相遇：用部分积抢救一次
          if (d == 1) d = n;           // 无效，换 c 重来
          break;
        }
        q = mul_mod(q, x > y ? x - y : y - x, n);
        if (!(i & 31)) d = __gcd(q, n);
      }
      if (d == 1) d = __gcd(q, n);
    }
    if (d != n) return d;
  }
}

void factorize(int n, vector<int>& fac) {
  if (n == 1) return;
  if (is_prime(n)) return fac.push_back(n), void();
  int d = pollard_rho(n);
  factorize(d, fac), factorize(n / d, fac);
}

/*
 * ============================================================
 * 名称：Pollard-Rho 大数质因数分解（Floyd 判圈，配套 Miller-Rabin）
 * 复杂度：期望 O(n^{1/4}) 找到一个非平凡因子；整体分解 O(n^{1/4} log n)
 * 用途：对 64 位范围内的大合数分解质因数（如 1e18 级别）；常与
 *       Miller-Rabin（is_prime 已内嵌）配合：先判素，再 Pollard-Rho 拆分
 * 原理：伪随机函数 f(x) = (x^2 + c) mod n 在模 n 下进入循环，用 Floyd
 *       判圈法取差与 n 求 gcd，命中非平凡因子；失败（d == n）时更换随机种子重试
 * 注意：#define int long long 使 int 为 64 位；mul_mod 用 __int128 防溢出；
 *       依赖 Gcd_Binary.cpp 或手写 gcd（此处用 __gcd 扩展，兼容 C++14）
 * 用法：factorize(n, fac) 后 fac 内含 n 的所有质因子（含重复、无序）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   int n;
 *   cin >> n;
 *   vector<int> fac;
 *   factorize(n, fac);
 *   for (int x : fac) cout << x << ' ';
 * }
 * ============================================================
 */
