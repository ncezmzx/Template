
double simpson(double (*f)(double), double a, double b) {
  double m = (a + b) / 2;
  return (b - a) / 6 * (f(a) + 4 * f(m) + f(b));
}
double asr(double (*f)(double), double a, double b, double eps, double s) {
  double m = (a + b) / 2, sl = simpson(f, a, m), sr = simpson(f, m, b);
  if (fabs(sl + sr - s) <= 15 * eps) return sl + sr + (sl + sr - s) / 15;
  return asr(f, a, m, eps / 2, sl) + asr(f, m, b, eps / 2, sr);
}
double integrate(double (*f)(double), double a, double b, double eps = 1e-8) {
  return asr(f, a, b, eps, simpson(f, a, b));
}

