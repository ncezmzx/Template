
#define int long long

int pw_(int x, int n, int mod) {
  int r = 1 % mod;
  x %= mod;
  if (x < 0) x += mod;
  while (n) {
    if (n & 1) r = r * x % mod;
    x = x * x % mod, n >>= 1;
  }
  return r;
}

vector<int> BM(const vector<int> &s, int mod) {
  int n = s.size(), L = 0, m = 0;
  vector<int> C(n + 1, 0), B(n + 1, 0), T;
  C[0] = B[0] = 1;
  int b = 1;
  for (int i = 0; i < n; ++i) {
    m++;
    int d = s[i] % mod;
    for (int j = 1; j <= L; ++j) d = (d + C[j] * s[i - j]) % mod;
    if (!d) continue;
    T = C;
    int coef = d * pw_(b, mod - 2, mod) % mod;
    for (int j = m; j <= n; ++j) C[j] = (C[j] - coef * B[j - m] % mod + mod) % mod;
    if (2 * L <= i) L = i + 1 - L, B = T, b = d, m = 0;
  }
  C.resize(L + 1);
  C.erase(C.begin());
  for (auto &x : C) x = (mod - x) % mod;
  return C;
}

vector<int> polymul(const vector<int> &a, const vector<int> &b, const vector<int> &rec, int mod) {
  int d = rec.size();
  vector<int> c(a.size() + b.size() - 1, 0);
  for (int i = 0; i < (int)a.size(); ++i)
    for (int j = 0; j < (int)b.size(); ++j) c[i + j] = (c[i + j] + a[i] * b[j]) % mod;
  for (int k = (int)c.size() - 1; k >= d; --k)
    for (int i = 1; i <= d; ++i) c[k - i] = (c[k - i] + c[k] * rec[i - 1]) % mod;
  c.resize(d);
  return c;
}

int kth_term(const vector<int> &s, int k, int mod) {
  vector<int> rec = BM(s, mod);
  int d = rec.size();
  if (d == 0) return 0;
  if (k < (int)s.size()) return s[k] % mod;
  vector<int> res(1, 1), base(2, 0);
  base[1] = 1;
  for (int e = k; e; e >>= 1) {
    if (e & 1) res = polymul(res, base, rec, mod);
    base = polymul(base, base, rec, mod);
  }
  int ans = 0;
  for (int i = 0; i < d && i < (int)s.size(); ++i) ans = (ans + res[i] * s[i]) % mod;
  return ans;
}

