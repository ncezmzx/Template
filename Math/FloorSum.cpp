#include <bits/stdc++.h>
using namespace std;
#define int long long


int floor_sum(int n, int m, int a, int b) {
  unsigned long long ans = 0;
  if (a < 0) {
    int a2 = a % m;
    if (a2 < 0) a2 += m;
    ans -= 1ULL * n * (n - 1) / 2 * ((a2 - a) / m);
    a = a2;
  }
  if (b < 0) {
    int b2 = b % m;
    if (b2 < 0) b2 += m;
    ans -= 1ULL * n * ((b2 - b) / m);
    b = b2;
  }
  unsigned long long un = n, um = m, ua = a, ub = b;
  while (true) {
    if (ua >= um) {
      ans += un * (un - 1) / 2 * (ua / um);
      ua %= um;
    }
    if (ub >= um) {
      ans += un * (ub / um);
      ub %= um;
    }
    unsigned long long y_max = ua * un + ub;
    if (y_max < um) break;
    un = y_max / um;
    ub = y_max % um;
    swap(ua, um);
  }
  return (int)ans;
}

