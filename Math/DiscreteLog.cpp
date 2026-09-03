#include <bits/stdc++.h>
using namespace std;
#define int long long

int pw(int x, int n, int m) {
  int r = 1 % m;
  x %= m;
  if (x < 0) x += m;
  while (n) {
    if (n & 1) r = r * x % m;
    x = x * x % m, n >>= 1;
  }
  return r;
}
int exgcd(int a, int b, int &x, int &y) {
  if (!b) return x = 1, y = 0, a;
  int g = exgcd(b, a % b, y, x);
  y -= a / b * x;
  return g;
}
int inv_mod(int a, int m) {
  int x, y;
  exgcd(a, m, x, y);
  return (x % m + m) % m;
}


int bsgs(int a, int b, int m) {
  if (m == 1) return 0;
  a %= m, b %= m;
  int k = (int)sqrtl(m) + 1;
  unordered_map<int, int> mp;
  mp.reserve(2 * k);
  int cur = 1;
  for (int j = 0; j < k; ++j) {
    if (!mp.count(cur)) mp[cur] = j;
    cur = cur * a % m;
  }
  int ainvk = pw(inv_mod(a, m), k, m);
  cur = b;
  for (int i = 0; i <= k; ++i) {
    auto it = mp.find(cur);
    if (it != mp.end()) return i * k + it->second;
    cur = cur * ainvk % m;
  }
  return -1;
}


int exbsgs(int a, int b, int m) {
  if (m == 1) return 0;
  a %= m, b %= m;
  if (b == 1 % m) return 0;
  int cnt = 0, d = 1;
  for (int g = __gcd(a, m); g > 1; g = __gcd(a, m)) {
    if (b % g) return -1;
    ++cnt;
    b /= g, m /= g;
    d = d * (a / g) % m;
    if (d == b) return cnt;
  }
  int r = bsgs(a, b * inv_mod(d, m) % m, m);
  return r < 0 ? -1 : r + cnt;
}

