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


struct cpx {
  int x, y;
};
int W;
cpx cmul(const cpx &a, const cpx &b, int p) {
  return {(a.x * b.x + a.y * b.y % p * W) % p, (a.x * b.y + a.y * b.x) % p};
}
int sqrt_mod(int a, int p) {
  a %= p;
  if (a == 0) return 0;
  if (pw(a, (p - 1) / 2, p) != 1) return -1;
  int b;
  for (b = 1;; ++b) {
    W = ((b * b - a) % p + p) % p;
    if (W && pw(W, (p - 1) / 2, p) == p - 1) break;
  }
  cpx r{1, 0}, c{b, 1};
  for (int e = (p + 1) / 2; e; e >>= 1) {
    if (e & 1) r = cmul(r, c, p);
    c = cmul(c, c, p);
  }
  return min(r.x, p - r.x);
}

