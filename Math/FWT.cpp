#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int md = 998244353;
int qpow(int a, int b) {
  int r = 1;
  for (; b; b >>= 1, a = a * a % md)
    if (b & 1) r = r * a % md;
  return r;
}

void fwt_or(int a[], int n, bool inv) {
  for (int len = 1; len < n; len <<= 1)
    for (int i = 0; i < n; i += len << 1)
      for (int j = 0; j < len; ++j) {
        int u = a[i + j], v = a[i + j + len];
        if (!inv) a[i + j + len] = (v + u) % md;
        else a[i + j + len] = (v - u + md) % md;
      }
}

void fwt_and(int a[], int n, bool inv) {
  for (int len = 1; len < n; len <<= 1)
    for (int i = 0; i < n; i += len << 1)
      for (int j = 0; j < len; ++j) {
        int u = a[i + j], v = a[i + j + len];
        if (!inv) a[i + j] = (u + v) % md;
        else a[i + j] = (u - v + md) % md;
      }
}

void fwt_xor(int a[], int n, bool inv) {
  for (int len = 1; len < n; len <<= 1)
    for (int i = 0; i < n; i += len << 1)
      for (int j = 0; j < len; ++j) {
        int u = a[i + j], v = a[i + j + len];
        a[i + j] = (u + v) % md;
        a[i + j + len] = (u - v + md) % md;
      }
  if (inv) {
    int iv = qpow(n, md - 2);
    for (int i = 0; i < n; ++i) a[i] = a[i] * iv % md;
  }
}

/*
 * ============================================================
 * 名称：快速沃尔什变换（FWT：子集卷积 or / and / xor）
 * 复杂度：O(n log n)，n 为 2 的幂（数组长度）
 * 用途：三种位运算卷积：c[k] = sum_{i|j=k} a[i]*b[j]（or）、
 *       c[k] = sum_{i&j=k} ...（and）、c[k] = sum_{i^j=k} ...（xor）
 * 用法：a、b 补零到长度 n（2 的幂）；
 *       fwt_or(a, n, 0), fwt_or(b, n, 0); for 逐位乘; fwt_or(a, n, 1) → a 即卷积
 *       and / xor 同理；xor 的逆变换自带除以 n
 * 说明：与 NTT.cpp 互补：NTT 做加法卷积，FWT 做位运算卷积；
 *       模数可换，逆变换需要 2 的逆元（xor 用 qpow(n) 处理）
 * ============================================================
 * 使用示例（编译时取消注释；求 xor 卷积 c = a ⊗ b）：
 * signed main() {
 *   int n = 1 << 3, a[8] = {1, 2, 0, 0, 0, 0, 0, 0}, b[8] = {3, 4, 0, 0, 0, 0, 0, 0};
 *   fwt_xor(a, n, 0), fwt_xor(b, n, 0);
 *   for (int i = 0; i < n; ++i) a[i] = a[i] * b[i] % md;
 *   fwt_xor(a, n, 1);
 *   for (int i = 0; i < n; ++i) cout << a[i] << ' ';
 * }
 * ============================================================
 */
