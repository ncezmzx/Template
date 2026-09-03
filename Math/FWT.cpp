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
          a[i + j + len] = v >= md ? v - md : v;
        }
        else {
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
        }
        else {
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

