
struct P {
  double x, y;
};
P operator-(P a, P b) { return {a.x - b.x, a.y - b.y}; }
double dist2(P a, P b) {
  P d = a - b;
  return d.x * d.x + d.y * d.y;
}
double dist(P a, P b) { return sqrt(dist2(a, b)); }

struct C {
  P c;
  double r;
};
C circle2(P a, P b) { return {(P){(a.x + b.x) / 2, (a.y + b.y) / 2}, dist(a, b) / 2}; }
P circumcenter(P a, P b, P c) {
  double a1 = b.x - a.x, b1 = b.y - a.y, a2 = c.x - a.x, b2 = c.y - a.y;
  double d1 = a1 * a1 + b1 * b1, d2 = a2 * a2 + b2 * b2, d = 2 * (a1 * b2 - a2 * b1);
  return {(P){a.x + (d1 * b2 - d2 * b1) / d, a.y + (a1 * d2 - a2 * d1) / d}};
}
C circle3(P a, P b, P c) {
  P o = circumcenter(a, b, c);
  return {o, dist(o, a)};
}
bool outside(P p, const C &c) { return dist2(p, c.c) > c.r * c.r + 1e-7; }

C min_circle(vector<P> p) {
  int n = p.size();
  if (n == 0) return {{0, 0}, -1};
  shuffle(p.begin(), p.end(), mt19937(0x5f5f));
  C c = {p[0], 0};
  for (int i = 1; i < n; ++i)
    if (outside(p[i], c)) {
      c = {p[i], 0};
      for (int j = 0; j < i; ++j)
        if (outside(p[j], c)) {
          c = circle2(p[i], p[j]);
          for (int k = 0; k < j; ++k)
            if (outside(p[k], c)) c = circle3(p[i], p[j], p[k]);
        }
    }
  return c;
}

