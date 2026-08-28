#include <bits/stdc++.h>
using namespace std;

// adaptive Simpson integration of f over [a, b]; eps = mixed relative/absolute tolerance
double simpson(double (*f)(double), double a, double b) {
  double m = (a + b) / 2;
  return (b - a) / 6 * (f(a) + 4 * f(m) + f(b));
}
double asr(double (*f)(double), double a, double b, double eps, double s) {
  double m = (a + b) / 2, sl = simpson(f, a, m), sr = simpson(f, m, b);
  if (fabs(sl + sr - s) <= 15 * eps) return sl + sr + (sl + sr - s) / 15;  // Richardson extrapolation
  return asr(f, a, m, eps / 2, sl) + asr(f, m, b, eps / 2, sr);
}
double integrate(double (*f)(double), double a, double b, double eps = 1e-8) {
  return asr(f, a, b, eps, simpson(f, a, b));
}

/*
 * ============================================================
 * ============================================================
 * Name: adaptive Simpson integration
 * Complexity: depends on the function; usually O(log((b-a)/eps)) recursion levels
 * Usage: numerical definite integral of f over [a, b] (smooth f; classic
 *        problems: shaded areas, circle-vs-function areas, and other cases
 *        where analytic integration is hard)
 * Principle: Simpson's rule (b-a)/6*(f(a)+4f(m)+f(b)) is exact for cubics;
 *        after halving, when the error estimate |sl+sr-s| <= 15*eps holds,
 *        return the Richardson-extrapolated value, otherwise recurse
 *        (adaptive)
 * Notes: split manually around singularities/cusps; eps ~ 1e-6 to 1e-9
 *        recommended (too small may recurse too deep); f is a plain function
 *        (or switch to a lambda + template)
 * ============================================================
 * Example (uncomment to compile):

 * double f(double x) { return sqrt(4 - x * x); }  // upper half-circle of radius 2
 * signed main() {
 *   cout << integrate(f, 0, 2) << '\n';           // 3.14159 (= pi, quarter of the circle area)
 *   cout << integrate([](double x) { return x * x * x; }, 0, 1) << '\n';  // 0.25 (exact for cubics)
 * }
 */
