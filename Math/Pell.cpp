#include <bits/stdc++.h>
using namespace std;
#define int long long

// fundamental solution of x^2 - D y^2 = 1 via continued fractions
long long isqrt(long long x) {
  long long r = (long long)sqrtl((long double)x);
  while ((r + 1) * (r + 1) <= x) ++r;
  while (r * r > x) --r;
  return r;
}
// D positive non-square; returns the fundamental solution (x1, y1)
pair<long long, long long> pell(long long D) {
  long long a0 = isqrt(D);
  vector<long long> A;  // A[i] = partial quotient a_{i+1}; period length L = A.size()
  long long m = 0, d = 1, a = a0;
  do {  // continued-fraction period of sqrt(D) ends at a == 2*a0
    m = d * a - m;
    d = (D - m * m) / d;
    a = (a0 + m) / d;
    A.push_back(a);
  } while (a != 2 * a0);
  long long L = A.size();
  // convergents p_i/q_i (p_{-2}=0, p_{-1}=1; q_{-2}=1, q_{-1}=0); __int128 guards overflow
  auto conv = [&](long long upto) {
    long long p = a0, q = 1;      // p_0, q_0
    long long p1 = 1, q1 = 0;     // p_{-1}, q_{-1}
    for (long long i = 1; i <= upto; ++i) {
      long long ai = A[(i - 1) % L];
      __int128 pn = (__int128)ai * p + p1, qn = (__int128)ai * q + q1;
      p1 = p, p = (long long)pn, q1 = q, q = (long long)qn;
    }
    return make_pair(p, q);
  };
  // even period L: fundamental solution (p_{L-1}, q_{L-1}); odd L: take convergent 2L-1
  return L % 2 == 0 ? conv(L - 1) : conv(2 * L - 1);
}

/*
 * ============================================================
 * Name: Pell equation (fundamental solution of x^2 - D y^2 = 1)
 * Complexity: O(sqrt D) (the continued-fraction period is <= 2 sqrt D * log;
 *             convergents O(L))
 * Usage: smallest positive solution (x1, y1) of x^2 - D y^2 = 1 (the
 *        fundamental solution); all positive solutions are (x1 + y1 sqrt D)^k.
 *        The negative Pell equation x^2 - D y^2 = -1 is solvable iff the
 *        continued-fraction period of sqrt D is odd, with fundamental solution
 *        (p_{L-1}, q_{L-1}).
 *        pell(D) -> pair(x1, y1); D is positive and not a perfect square.
 * Interface: pell(D) -> pair(x1, y1); D positive and not a perfect square
 * Principle: the continued fraction of sqrt D is periodic; an even period L
 *            gives (p_{L-1}, q_{L-1}) and an odd L gives (p_{2L-1}, q_{2L-1})
 * Notes: intermediate products use __int128; for large D the fundamental
 *        solution can exceed long long (D = 1021 has 74 digits), which needs
 *        big integers; D a perfect square is meaningless
 * Source: OI-Wiki "Pell equation" (https://oi-wiki.org/math/number-theory/pell-equation/)
 * ============================================================
 */
