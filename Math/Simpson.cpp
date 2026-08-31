#include <bits/stdc++.h>
using namespace std;

// adaptive Simpson integration of f over [a, b]; eps = mixed relative/absolute tolerance
double simpson(double (*f)(double), double a, double b) {
  double m = (a + b) / 2;
  return (b - a) / 6 * (f(a) + 4 * f(m) + f(b));
}
double asr(double (*f)(double), double a, double b, double eps, double s) {
  double m = (a + b) / 2, sl = simpson(f, a, m), sr = simpson(f, m, b);
  if (fabs(sl + sr - s) <= 15 * eps) return sl + sr + (sl + sr - s) / 15; // Richardson extrapolation
  return asr(f, a, m, eps / 2, sl) + asr(f, m, b, eps / 2, sr);
}
double integrate(double (*f)(double), double a, double b, double eps = 1e-8) {
  return asr(f, a, b, eps, simpson(f, a, b));
}

/*
 * ============================================================
 * Name: adaptive Simpson integration
 * Complexity: depends on the function; usually O(log((b-a)/eps)) recursion
 *             levels
 * Usage: numerical definite integral of a smooth f over [a, b]; classic
 *        problems are shaded areas, circle-vs-function areas, and other cases
 *        where analytic integration is hard.
 * Principle: Simpson's rule (b-a)/6*(f(a) + 4f(m) + f(b)) is exact for cubics;
 *            after halving, when |sl + sr - s| <= 15*eps return the Richardson-
 *            extrapolated value, otherwise recurse (adaptive)
 * Notes: split manually around singularities / cusps; eps ~ 1e-6 to 1e-9 is
 *        recommended (too small may recurse too deep); f is a plain function
 *        (switch to a lambda + template if needed)
 * ============================================================
 */
