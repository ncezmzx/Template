#include <bits/stdc++.h>
using namespace std;
#define int long long


tuple<int, int, int> exgcd(int a, int b, int c) {
  if (!b) return {c / a, 0, a};
  auto [y, x, d] = exgcd(b, a % b, c);
  return {x, y - a / b * x, d};
}

tuple<int, int, int> exgcd(int a, int b) { return exgcd(a, b, __gcd(a, b)); }

