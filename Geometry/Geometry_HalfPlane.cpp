
const double EPS = 1e-9;
struct P {
  double x, y;
};
P operator+(P a, P b) { return {a.x + b.x, a.y + b.y}; }
P operator-(P a, P b) { return {a.x - b.x, a.y - b.y}; }
P operator*(P a, double k) { return {a.x * k, a.y * k}; }
double cross(P a, P b) { return a.x * b.y - a.y * b.x; }

struct HP {
  P p, v;
  double ang;
  HP(P p_ = {0, 0}, P v_ = {1, 0}) : p(p_), v(v_), ang(atan2(v_.y, v_.x)) {}
};
bool onleft(const HP &l, const P &t) { return cross(l.v, t - l.p) > 0; }
P line_inter(const HP &a, const HP &b) {
  double t = cross(b.p - a.p, b.v) / cross(a.v, b.v);
  return a.p + a.v * t;
}

vector<P> hpi(vector<HP> ls) {
  sort(ls.begin(), ls.end(), [](const HP &a, const HP &b) { return a.ang < b.ang; });
  int n = ls.size(), first = 0, last = 0;
  vector<HP> q(n);
  vector<P> p(n);
  q[0] = ls[0];
  for (int i = 1; i < n; i++) {
    while (first < last && !onleft(ls[i], p[last - 1])) --last;
    while (first < last && !onleft(ls[i], p[first])) ++first;
    q[++last] = ls[i];
    if (last - first >= 1 && fabs(cross(q[last].v, q[last - 1].v)) < EPS) {
      --last;
      if (onleft(q[last], ls[i].p)) q[last] = ls[i];
    }
    if (last - first >= 1) p[last - 1] = line_inter(q[last - 1], q[last]);
  }
  while (first < last && !onleft(q[first], p[last - 1])) --last;
  if (last - first <= 1) return {};
  p[last] = line_inter(q[last], q[first]);
  return vector<P>(p.begin() + first, p.begin() + last + 1);
}

double area(const vector<P> &p) {
  double s = 0;
  for (int i = 1, n = p.size(); i + 1 < n; ++i) s += cross(p[i] - p[0], p[i + 1] - p[0]);
  return fabs(s) / 2;
}

