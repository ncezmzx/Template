#include <bits/stdc++.h>
using namespace std;
#define int long long


long long isqrt(long long x) {
  long long r = (long long)sqrtl((long double)x);
  while ((r + 1) * (r + 1) <= x) ++r;
  while (r * r > x) --r;
  return r;
}

pair<long long, long long> pell(long long D) {
  long long a0 = isqrt(D);
  vector<long long> A;
  long long m = 0, d = 1, a = a0;
  do {
    m = d * a - m;
    d = (D - m * m) / d;
    a = (a0 + m) / d;
    A.push_back(a);
  } while (a != 2 * a0);
  long long L = A.size();

  auto conv = [&](long long upto) {
    long long p = a0, q = 1;
    long long p1 = 1, q1 = 0;
    for (long long i = 1; i <= upto; ++i) {
      long long ai = A[(i - 1) % L];
      __int128 pn = (__int128)ai * p + p1, qn = (__int128)ai * q + q1;
      p1 = p, p = (long long)pn, q1 = q, q = (long long)qn;
    }
    return make_pair(p, q);
  };

  return L % 2 == 0 ? conv(L - 1) : conv(2 * L - 1);
}

