#include <bits/stdc++.h>
using namespace std;
using u64 = unsigned long long;
using i64 = long long;

// Σ_{i=1..n} i^m 的闭式（m <= 3；T 需支持模意义乘除，如 modint / __int128）
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
    if (k > pcnt || n < prime[k] || n <= 1) {   // k > pcnt 先判，防 prime[k] 越界读
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
 * 名称：Min25 筛（积性函数前缀和，f(p) 为素数处多项式）
 * 复杂度：O(n^{3/4} / log n) 级；空间 O(sqrt n)
 * 用途：求 Σ_{i=1..n} f(i)，f 为积性函数且 f(p) 是 p 的次数 < M 的
 *       多项式（系数数组 a：f(p) = Σ_m a[m]·p^m）；f(p^c) 由回调 f(p, c) 给出
 * 依赖：power_sum（Σ i^m 闭式，m <= 4；T 需支持模意义乘除，如 modint）
 * 来源：用户提供代码；已修 get() 中 k > pcnt 时 prime[k] 的越界读，
 *       并补齐缺失的 power_sum 依赖
 * 注意：T 需支持默认构造（零元）、int 构造、+ - * / 与复合赋值；
 *       一个对象只能调用一次 operator()（内部状态按单次求解建立）
 * ============================================================
 * 使用示例（编译时取消注释；Σ_{i=1..n} φ(i)，n = 10 时答案 32）：
 * #include <Math/ModInt.cpp>   // T 用 Montgomery modint
 * signed main() {
 *   using T = mint;
 *   // f(p) = p - 1 → a = {-1, +1}；f(p^c) = p^c - p^(c-1)
 *   auto fpc = [](int p, int c) -> T {
 *     T pw = 1, pw1 = 1;
 *     for (int i = 0; i < c; ++i) pw *= p;
 *     for (int i = 0; i < c - 1; ++i) pw1 *= p;
 *     return pw - pw1;
 *   };
 *   Min25<T, 2> min25({T(-1), T(1)}, fpc);
 *   long long n = 10;
 *   cout << raw(min25(n)) << '\n';   // 32
 * }
 * ============================================================
 */
