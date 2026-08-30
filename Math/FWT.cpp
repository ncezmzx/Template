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
        if (!inv) {
          int v = a[i + j + len] + a[i + j];
          a[i + j + len] = v >= md ? v - md : v;   // conditional subtract avoids %
        } else {
          int v = a[i + j + len] - a[i + j];
          a[i + j + len] = v < 0 ? v + md : v;
        }
      }
}

void fwt_and(int a[], int n, bool inv) {
  for (int len = 1; len < n; len <<= 1)
    for (int i = 0; i < n; i += len << 1)
      for (int j = 0; j < len; ++j) {
        if (!inv) {
          int v = a[i + j] + a[i + j + len];
          a[i + j] = v >= md ? v - md : v;
        } else {
          int v = a[i + j] - a[i + j + len];
          a[i + j] = v < 0 ? v + md : v;
        }
      }
}

void fwt_xor(int a[], int n, bool inv) {
  for (int len = 1; len < n; len <<= 1)
    for (int i = 0; i < n; i += len << 1)
      for (int j = 0; j < len; ++j) {
        int u = a[i + j], v = a[i + j + len];
        int s = u + v, d = u - v;
        a[i + j] = s >= md ? s - md : s;
        a[i + j + len] = d < 0 ? d + md : d;
      }
  if (inv) {
    int iv = qpow(n, md - 2);
    for (int i = 0; i < n; ++i) a[i] = a[i] * iv % md;
  }
}

/*
 * ============================================================
 * Name: fast Walsh-Hadamard transform (FWT: or / and / xor subset convolutions)
 * Complexity: O(n log n), n a power of two (array length)
 * Usage: three bitwise convolutions: c[k] = sum_{i|j=k} a[i]*b[j] (or),
 *        sum_{i&j=k} (and), sum_{i^j=k} (xor).
 *        pad a, b to a power of two n; fwt_or(a, n, 0) and fwt_or(b, n, 0);
 *        multiply pointwise; fwt_or(a, n, 1) leaves the convolution in a; and /
 *        xor are analogous.
 * Notes: the xor inverse transform divides by n itself; the modulus is
 *        swappable, but the inverse needs the inverse of 2 (xor handles it via
 *        qpow(n)); complements NTT.cpp, which does additive convolution
 * ============================================================
 */
