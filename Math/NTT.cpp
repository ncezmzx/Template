#include <bits/stdc++.h>
using namespace std;
namespace Poly {
  constexpr int Mod = 998244353;
  constexpr int Pr = 3;
  constexpr int iPr = 332748118;
  int qpow(int x, long long y) {
    long long r = 1, a = x;
    while (y) {
      if (y & 1) r = r * a % Mod;
      a = a * a % Mod;
      y >>= 1;
    }
    return (int)r;
  }
  struct poly : vector<int> {
    poly() {}
    poly(const initializer_list<int> &s) : vector<int>(s) {}
    poly(int n) { resize(n); }
    poly(int n, int val) { resize(n, val); }
    template <typename Iter>
    poly(Iter first, Iter last) : vector<int>(first, last) {}
  };
  inline int addmod(int a, int b) {
    a += b;
    if (a >= Mod) a -= Mod;
    return a;
  }
  inline int submod(int a, int b) {
    a -= b;
    if (a < 0) a += Mod;
    return a;
  }
  inline int mulmod(long long a, long long b) { return (int)((a * b) % Mod); }
  static vector<int> rev;
  static vector<int> roots{0, 1};
  void ensure_roots(int n) {
    if ((int)roots.size() >= n) return;
    int k = __builtin_ctz((int)roots.size());
    roots.resize(n);
    while ((1 << k) < n) {
      int z = qpow(Pr, (Mod - 1) >> (k + 1));
      for (int i = 1 << (k - 1); i < (1 << k); i++) {
        roots[i << 1] = roots[i];
        roots[i << 1 | 1] = mulmod(roots[i], z);
      }
      k++;
    }
  }
  void prepare_rev(int n) {
    if ((int)rev.size() == n) return;
    rev.assign(n, 0);
    int lg = __builtin_ctz(n);
    for (int i = 0; i < n; i++) rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (lg - 1));
  }
  void ntt(poly &a, int n, bool invert) {
    prepare_rev(n);
    ensure_roots(n);
    a.resize(n);
    for (int i = 0; i < n; i++)
      if (i < rev[i]) swap(a[i], a[rev[i]]);
    for (int len = 1; len < n; len <<= 1) {
      int step = len << 1;
      for (int i = 0; i < n; i += step) {
        for (int j = 0; j < len; j++) {
          int u = a[i + j];
          int v = mulmod(a[i + j + len], roots[len + j]);
          a[i + j] = addmod(u, v);
          a[i + j + len] = submod(u, v);
        }
      }
    }
    if (invert) {
      reverse(a.begin() + 1, a.end());
      int inv_n = qpow(n, Mod - 2);
      for (int i = 0; i < n; i++) a[i] = mulmod(a[i], inv_n);
    }
  }
  void NTT(poly &a, int ty) {
    int n = a.size();
    int need = 1;
    while (need < n) need <<= 1;
    ntt(a, need, ty == 0);
  }
  poly convolution(const poly &A, const poly &B, int need) {
    if (need <= 0) return poly(0);
    int an = (int)A.size(), bn = (int)B.size();
    if (min(an, bn) == 0) return poly(need, 0);
    if ((long long)an * bn <= 1024) {
      poly res(min(need, an + bn - 1));
      for (int i = 0; i < an; i++) {
        for (int j = 0; j < bn && i + j < need; j++) {
          res[i + j] = (res[i + j] + (long long)A[i] * B[j]) % Mod;
        }
      }
      if ((int)res.size() < need) res.resize(need, 0);
      return res;
    }
    int need_n = min(need, an + bn - 1);
    int n = 1;
    while (n < an + bn - 1) n <<= 1;
    poly a(n), b(n);
    for (int i = 0; i < an; i++) a[i] = A[i];
    for (int i = 0; i < bn; i++) b[i] = B[i];
    ntt(a, n, false);
    ntt(b, n, false);
    for (int i = 0; i < n; i++) a[i] = mulmod(a[i], b[i]);
    ntt(a, n, true);
    a.resize(need_n);
    if (need_n < need) a.resize(need, 0);
    return a;
  }
  poly operator+(poly a, poly b) {
    poly r(max(a.size(), b.size()));
    a.resize(r.size());
    b.resize(r.size());
    for (int i = 0; i < (int)r.size(); i++) r[i] = (a[i] + b[i]) % Mod;
    return r;
  }
  poly operator-(poly a, poly b) {
    poly r(max(a.size(), b.size()));
    a.resize(r.size());
    b.resize(r.size());
    for (int i = 0; i < (int)r.size(); i++) r[i] = (a[i] - b[i] + Mod) % Mod;
    return r;
  }
  poly operator*(const poly &a, const poly &b) {
    int n = (int)a.size() - 1, m = (int)b.size() - 1;
    if (n < 0 || m < 0) return poly(0);
    int lg = 1;
    while (lg <= n + m) lg <<= 1;
    poly A = a, B = b;
    A.resize(lg), B.resize(lg);
    ntt(A, lg, false);
    ntt(B, lg, false);
    for (int i = 0; i < lg; i++) A[i] = mulmod(A[i], B[i]);
    ntt(A, lg, true);
    int inv = qpow(lg, Mod - 2);
    A.resize(n + m + 1);
    for (int i = 0; i <= n + m; i++) A[i] = mulmod(A[i], 1);
    return A;
  }
  poly operator*(poly a, int b) {
    for (int i = 0; i < (int)a.size(); i++) a[i] = a[i] * b % Mod;
    return a;
  }
  poly operator>>(const poly &a, int b) {
    if (b >= (int)a.size()) return poly(0);
    poly r((int)a.size() - b);
    for (int i = b; i < (int)a.size(); i++) r[i - b] = a[i];
    return r;
  }
  poly operator<<(const poly &a, int b) {
    poly r((int)a.size() + b);
    for (int i = b; i < (int)r.size(); i++) r[i] = a[i - b];
    return r;
  }
  void operator~(poly &a) {
    while (a.size() > 1 && !a.back()) a.pop_back();
  }
  poly inv(poly a, int n = 0) {
    if (!n) n = a.size();
    a.resize(n);
    poly r(1);
    r[0] = qpow((int)a[0], Mod - 2);
    int cur = 1;
    while (cur < n) {
      int nxt = cur << 1;
      poly a_cut(min((int)a.size(), nxt));
      for (int i = 0; i < (int)a_cut.size(); i++) a_cut[i] = a[i];
      poly tmp = convolution(a_cut, r, nxt);
      for (int i = 0; i < (int)tmp.size(); i++) tmp[i] = submod(0, tmp[i]);
      tmp[0] = addmod(tmp[0], 2);
      poly nr = convolution(tmp, r, nxt);
      r = nr;
      r.resize(nxt);
      cur = nxt;
    }
    r.resize(n);
    return r;
  }
  poly deri(const poly &a) {
    int n = a.size();
    if (n <= 1) return poly(0);
    poly b(n - 1);
    for (int i = 1; i < n; i++) b[i - 1] = mulmod(a[i], i);
    return b;
  }
  poly inte(const poly &a) {
    int n = a.size();
    poly b(n + 1);
    for (int i = 0; i < n; i++) b[i + 1] = mulmod(a[i], qpow(i + 1, Mod - 2));
    return b;
  }
  poly ln(const poly &a, int n = 0) {
    if (!n) n = a.size();
    poly der = deri(a);
    poly inva = inv(a, n);
    poly prod = convolution(der, inva, n - 1);
    poly res = inte(prod);
    res.resize(n);
    return res;
  }
  poly exp(const poly &a, int n = 0) {
    if (!n) n = a.size();
    poly res(1);
    res[0] = 1;
    int cur = 1;
    while (cur < n) {
      int nxt = cur << 1;
      poly lnres = ln(res, nxt);
      poly diff(nxt);
      for (int i = 0; i < nxt; i++) {
        int ai = i < (int)a.size() ? a[i] : 0;
        int li = i < (int)lnres.size() ? lnres[i] : 0;
        diff[i] = submod(ai, li);
      }
      diff[0] = addmod(diff[0], 1);
      poly nxtres = convolution(res, diff, nxt);
      res = nxtres;
      res.resize(nxt);
      cur = nxt;
    }
    res.resize(n);
    return res;
  }
}
using namespace Poly;
/*
 * ============================================================
 * 名称：NTT + 多项式全家桶（namespace Poly）
 * 复杂度：ntt O(n log n)，卷积 O(n log n)，inv / ln / exp O(n log n)，deri / inte O(n)
 * 用途：模 998244353 下的多项式卷积、求逆、ln、exp、求导、积分、左右移位、去尾零。
 *       注意：inv / ln 要求常数项非零；exp 要求常数项为 0（形式幂级数语义）。
 * 来源：all.cpp 行 39879-40104（原样保留；注释已统一移至文件尾部）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   poly a = {1, 2, 3}, b = {4, 5};
 *   poly c = convolution(a, b, 4);  // {4, 13, 22, 15}
 *   for (int x : c) cout << x << ' ';
 *   cout << '\n';
 *   poly f = {0, 1, 1};      // x + x^2
 *   poly g = exp(f, 4);      // exp(x + x^2) 的前 4 项
 *   for (int x : g) cout << x << ' ';
 *   cout << '\n';
 *   poly h = ln({1, 1, 0}, 3);  // ln(1 + x) 的前 3 项: 0, 1, md - 1/2
 *   for (int x : h) cout << x << ' ';
 *   cout << '\n';
 *   poly d = deri(a), it = inte(d);  // 求导 / 积分
 *   for (int x : it) cout << x << ' ';
 *   cout << '\n';
 * }
 * ============================================================
 */
