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
int exgcd(int a, int b, int& x, int& y) {
  if (!b) return x = 1, y = 0, a;
  int g = exgcd(b, a % b, y, x);
  y -= a / b * x;
  return g;
}
int inv_mod(int a, int m) {  // requires gcd(a, m) = 1
  int x, y;
  exgcd(a, m, x, y);
  return (x % m + m) % m;
}

// BSGS: smallest x >= 0 with a^x = b (mod m), requires gcd(a, m) = 1; -1 if none; O(sqrt m)
int bsgs(int a, int b, int m) {
  if (m == 1) return 0;
  a %= m, b %= m;
  int k = (int)sqrtl(m) + 1;
  unordered_map<int, int> mp;
  mp.reserve(2 * k);
  int cur = 1;
  for (int j = 0; j < k; ++j) {  // baby steps: a^j (keep smallest j)
    if (!mp.count(cur)) mp[cur] = j;
    cur = cur * a % m;
  }
  int ainvk = pw(inv_mod(a, m), k, m);  // a^{-k}
  cur = b;
  for (int i = 0; i <= k; ++i) {  // giant steps: b*a^{-ik}; hit a^j -> x = ik + j
    auto it = mp.find(cur);
    if (it != mp.end()) return i * k + it->second;
    cur = cur * ainvk % m;
  }
  return -1;
}

// exBSGS: arbitrary gcd(a, m); -1 if unsolvable
// each round with g = gcd(a, m) > 1 needs g | b; divide both sides by g:
// (a/g)*a^{x-1} = b/g (mod m/g); accumulate d = prod(a/g);
// when gcd(a, m) = 1, solve a^t = b*d^{-1}, x = t + cnt
int exbsgs(int a, int b, int m) {
  if (m == 1) return 0;
  a %= m, b %= m;
  if (b == 1 % m) return 0;  // a^0 = 1
  int cnt = 0, d = 1;
  for (int g = __gcd(a, m); g > 1; g = __gcd(a, m)) {
    if (b % g) return -1;
    ++cnt;
    b /= g, m /= g;
    d = d * (a / g) % m;
    if (d == b) return cnt;  // a^{cnt} = b (mod original m)
  }
  int r = bsgs(a, b * inv_mod(d, m) % m, m);
  return r < 0 ? -1 : r + cnt;
}

/*
 * ============================================================
 * ============================================================
 * Name: BSGS / exBSGS (discrete logarithm)
 * Complexity: BSGS O(sqrt m); exBSGS O(sqrt m + log^2 m)
 * Usage: smallest x >= 0 with a^x = b (mod m):
 *        bsgs(a, b, m) requires gcd(a, m) = 1;
 *        exbsgs(a, b, m) is unrestricted (peels off gcds, reduces to BSGS)
 * Principle: x = i*k + j (k = ceil(sqrt m)): b*a^{-ik} = a^j — baby steps
 *        store a^j (keeping the smallest j) in a hash table, giant steps
 *        multiply b by a^{-k} per i; the first hit is the minimal solution;
 *        exBSGS divides both sides by g = gcd(a, m) each round (needs g | b),
 *        turning the equation into (a/g)*a^{x-1} = b/g (mod m/g), accumulating
 *        the factor into d; d == b gives x = cnt early; afterwards a and m
 *        are coprime, solve a^t = b*d^{-1} (mod m) and x = t + cnt
 * Notes: m = 1 or b = 1 returns 0; the solution is < m (order <= m);
 *        duplicates Math/CRT.cpp's exgcd (this file is self-contained)
 * ============================================================
 * Example (uncomment to compile):

 * signed main() {
 *   cout << bsgs(3, 1, 7) << '\n';      // 0（3^0 = 1）
 *   cout << bsgs(3, 6, 7) << '\n';      // 3（3^3 = 27 ≡ 6）
 *   cout << bsgs(2, 3, 7) << '\n';      // -1（2^x mod 7 ∈ {1,2,4}）
 *   cout << exbsgs(2, 8, 16) << '\n';   // 3（2^3 = 8）
 *   cout << exbsgs(4, 2, 12) << '\n';   // -1（4^x mod 12 ∈ {4, 4, ...}）
 * }
 */
