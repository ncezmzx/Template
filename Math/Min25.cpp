#include <bits/stdc++.h>
using namespace std;
using u64 = unsigned long long;
using i64 = long long;

// closed form of sum_{i=1..n} i^m (m <= 3; T needs modular mul/div, e.g. modint / __int128)
template <class T>
T power_sum(i64 n, int m) {
  assert(m >= 0 && m <= 4);
  if (m == 0) return T(n);
  T x = T(n);
  if (m == 1) return x * (x + 1) / 2;
  if (m == 2) return x * (x + 1) * (2 * x + 1) / 6;
  if (m == 3) { T s = x * (x + 1) / 2; return s * s; }
  // m == 4: n(n+1)(2n+1)(3n^2+3n-1)/30
  return x * (x + 1) * (2 * x + 1) * (3 * x * x + 3 * x - 1) / 30;
}

template <typename T, int M, typename F>
class Min25 {
  using i64 = int64_t;

  std::array<T, M> a;
  F f;

  i64 n;
  int lim, pcnt, cnt;

  std::vector<int> prime, mpf, le, ge;
  std::vector<i64> lis;
  std::vector<std::array<T, M>> s_prime, G;
  std::vector<T> F_prime_id, F_prime_p;

  void sieve(int n) {
    for (int i = 2; i <= n; ++i) {
      if (mpf[i] == 0) {
        mpf[i] = ++pcnt;
        prime.push_back(i);
        s_prime.push_back({});

        T power = 1;
        for (int m = 0; m < M; ++m) {
          s_prime[pcnt][m] = s_prime[pcnt - 1][m] + power;
          power *= i;
        }
      }
      for (int j = 1; j <= mpf[i] && i * prime[j] <= n; ++j) {
        mpf[i * prime[j]] = j;
      }
    }
  }
  void init() {
    for (i64 i = 1; i <= n; i = n / (n / i) + 1) {
      i64 j = n / i;

      lis[++cnt] = j;
      if (j <= lim) {
        le[j] = cnt;
      } else {
        ge[n / j] = cnt;
      }
      for (int m = 0; m < M; ++m) {
        G[cnt][m] = power_sum<T>(j, m) - 1;
      }
    }
  }
  int idx(i64 v) { return v <= lim ? le[v] : ge[n / v]; }
  void calcFprime() {
    for (int k = 1; k <= pcnt; ++k) {
      int p = prime[k];

      i64 sqr_p = (i64)p * p;
      for (int i = 1; lis[i] >= sqr_p; ++i) {
        const int id = idx(lis[i] / p);

        T power = 1;
        for (int m = 0; m < M; ++m) {
          G[i][m] -= power * (G[id][m] - s_prime[k - 1][m]);
          power *= p;
        }
      }
    }
    for (int i = 1; i <= cnt; ++i) {
      for (int m = 0; m < M; ++m) {
        F_prime_id[i] += a[m] * G[i][m];
      }
    }
    for (int i = 1; i <= pcnt; ++i) {
      for (int m = 0; m < M; ++m) {
        F_prime_p[i] += a[m] * s_prime[i][m];
      }
    }
  }
  T get(i64 n, int k) {
    if (k > pcnt || n < prime[k] || n <= 1) {   // test k > pcnt first (avoids OOB prime[k])
      return 0;
    }
    int id = idx(n);
    T res = F_prime_id[id] - F_prime_p[k - 1];
    for (int i = k; i <= pcnt; ++i) {
      int p = prime[i];
      i64 pw = p, pw2 = (i64)p * p;
      if (pw2 > n) {
        break;
      }
      for (int c = 1; pw2 <= n; ++c, pw = pw2, pw2 *= p) {
        res += f(p, c) * get(n / pw, i + 1) + f(p, c + 1);
      }
    }
    return res;
  }

 public:
  Min25(std::array<T, M> a, F f) : a(a), f(f) {}
  T operator()(i64 n) {
    this->n = n;
    lim = sqrtl(n) + 5;

    const int N = std::min(lim + 4000, lim * 3);
    pcnt = 0;
    mpf.assign(N + 1, 0);
    prime.resize(1);
    s_prime.resize(1);
    sieve(N);

    cnt = 0;
    G.resize(lim * 2);
    lis.resize(lim * 2);
    le.resize(lim + 1);
    ge.resize(n / (lim + 1) + 1);
    init();

    F_prime_id.assign(cnt + 1, 0);
    F_prime_p.assign(pcnt + 1, 0);
    calcFprime();
    return get(n, 1) + 1;
  }
};

/*
 * ============================================================
 * Name: Min25 sieve (prefix sums of multiplicative functions, f(p) polynomial)
 * Complexity: ~O(n^{3/4} / log n); space O(sqrt n)
 * Usage: sum_{i=1..n} f(i) for multiplicative f whose values on primes are a
 *        polynomial in p of degree < M (coefficient array a, so f(p) = sum_m
 *        a[m]*p^m); f(p^c) comes from the callback f(p, c).
 * Depends: power_sum (closed form of sum i^m, m <= 4; T must support modular
 *        multiplication/division, e.g. modint)
 * Source: user-provided code; fixed the OOB prime[k] read in get() when
 *         k > pcnt and added the missing power_sum dependency
 * Notes: T needs default construction (zero), int construction, + - * / and
 *        compound assignment; each object may call operator() only once, since
 *        internal state is built for a single solve;
 *        depends on power_sum (closed form of sum i^m for m <= 4)
 * ============================================================
 */
