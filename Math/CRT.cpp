#define int long long

int exgcd(int a, int b, int &x, int &y) {
  if (!b) return x = 1, y = 0, a;
  int g = exgcd(b, a % b, y, x);
  y -= a / b * x;
  return g;
}

pair<int, int> inv_gcd(int a, int b) {
  a %= b;
  if (a < 0) a += b;
  int x, y, g = exgcd(a, b, x, y);
  if (g != 1) x = (x % (b / g) + (b / g)) % (b / g);
  else x = (x % b + b) % b;
  return {g, x};
}
int safe_mod(int a, int m) {
  a %= m;
  return a < 0 ? a + m : a;
}

pair<int, int> crt(vector<int> r, vector<int> m) {
  int r0 = 0, m0 = 1;
  for (int i = 0; i < (int)r.size(); i++) {
    int r1 = safe_mod(r[i], m[i]), m1 = m[i];
    if (m0 < m1) swap(r0, r1), swap(m0, m1);
    if (m0 % m1 == 0) {
      if (r0 % m1 != r1) return {0, 0};
      continue;
    }

    int g, im;
    tie(g, im) = inv_gcd(m0, m1);
    if ((r1 - r0) % g) return {0, 0};
    int u1 = m1 / g;
    int t = (r1 - r0) / g % u1 * im % u1;
    r0 += t * m0;
    m0 *= u1;
    if (r0 < 0) r0 += m0;
  }
  return {r0, m0};
}

