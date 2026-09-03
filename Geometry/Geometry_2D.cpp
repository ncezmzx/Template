#define int long long

struct P {
  int x, y;
};
P operator-(P a, P b) { return {a.x - b.x, a.y - b.y}; }
bool operator<(P a, P b) { return a.x != b.x ? a.x < b.x : a.y < b.y; }
bool operator==(P a, P b) { return a.x == b.x && a.y == b.y; }
int cross(P a, P b) { return a.x * b.y - a.y * b.x; }
int dot(P a, P b) { return a.x * b.x + a.y * b.y; }
int cr(P a, P b, P c) { return cross(b - a, c - a); }

bool on_seg(P a, P b, P p) {
  return cr(a, b, p) == 0 && dot(p - a, p - b) <= 0;
}
bool seg_inter(P a, P b, P c, P d) {
  int d1 = cr(a, b, c), d2 = cr(a, b, d), d3 = cr(c, d, a), d4 = cr(c, d, b);
  if (d1 * d2 < 0 && d3 * d4 < 0) return true;
  return (!d1 && on_seg(a, b, c)) || (!d2 && on_seg(a, b, d)) || (!d3 && on_seg(c, d, a)) || (!d4 && on_seg(c, d, b));
}
int area2(const vector<P> &p) {
  int s = 0;
  for (int i = 0, n = p.size(); i < n; ++i) s += cross(p[i], p[(i + 1) % n]);
  return s;
}
int pip(const vector<P> &p, P q) {
  int cnt = 0, n = p.size();
  for (int i = 0; i < n; ++i) {
    P a = p[i], b = p[(i + 1) % n];
    if (on_seg(a, b, q)) return 1;
    if ((a.y > q.y) != (b.y > q.y)) {
      int d = cross(b - a, q - a);
      if (b.y > a.y ? d > 0 : d < 0) ++cnt;
    }
  }
  return cnt & 1 ? 2 : 0;
}

vector<P> convex_hull(vector<P> p) {
  sort(p.begin(), p.end());
  p.erase(unique(p.begin(), p.end()), p.end());
  int m = p.size(), k = 0;
  if (m <= 2) return p;
  vector<P> h(2 * m + 1);
  for (int i = 0; i < m; ++i) {
    while (k > 1 && cr(h[k - 2], h[k - 1], p[i]) <= 0) --k;
    h[k++] = p[i];
  }
  for (int i = m - 2, t = k; i >= 0; --i) {
    while (k > t && cr(h[k - 2], h[k - 1], p[i]) <= 0) --k;
    h[k++] = p[i];
  }
  h.resize(k - 1);
  if ((int)h.size() == 2 && h[0] == h[1]) h.resize(1);
  return h;
}

long long diameter2(const vector<P> &h) {
  int n = h.size();
  if (n == 1) return 0;
  if (n == 2) {
    P d = h[0] - h[1];
    return dot(d, d);
  }
  long long res = 0;
  for (int i = 0, j = 2; i < n; ++i) {
    P e = h[(i + 1) % n] - h[i];
    while (abs(cross(e, h[(j + 1) % n] - h[i])) > abs(cross(e, h[j] - h[i]))) j = (j + 1) % n;
    P d1 = h[i] - h[j], d2 = h[(i + 1) % n] - h[j];
    res = max({res, dot(d1, d1), dot(d2, d2)});
  }
  return res;
}

constexpr int CN = 1e6 + 9;
P tmp_[CN];
long long d2_(P a, P b) {
  P d = a - b;
  return dot(d, d);
}
long long closest2(P *a, int l, int r) {
  if (r - l <= 1) return LLONG_MAX;
  if (r - l == 2) {
    if (a[l].y > a[l + 1].y) swap(a[l], a[l + 1]);
    return d2_(a[l], a[l + 1]);
  }
  int m = (l + r) / 2;
  int xm = a[m].x;
  long long d = min(closest2(a, l, m), closest2(a, m, r));
  inplace_merge(a + l, a + m, a + r, [](P u, P v) { return u.y != v.y ? u.y < v.y : u.x < v.x; });
  int c = 0;
  for (int i = l; i < r; ++i)
    if ((a[i].x - xm) * (a[i].x - xm) < d) tmp_[c++] = a[i];
  for (int i = 0; i < c; ++i)
    for (int j = i + 1; j < c && (tmp_[j].y - tmp_[i].y) * (tmp_[j].y - tmp_[i].y) < d; ++j)
      d = min(d, d2_(tmp_[i], tmp_[j]));
  return d;
}
long long closest_pair2(vector<P> p) {
  sort(p.begin(), p.end());
  return closest2(p.data(), 0, p.size());
}

