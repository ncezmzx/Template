
constexpr int N = 2e5 + 9;
struct Node {
  int a, b, c;
};
Node v_[N], tmp_[N];
int bit_[N], C_;
void bit_add(int x, int w) {
  for (; x <= C_; x += x & -x) bit_[x] += w;
}
long long bit_sum(int x) {
  long long r = 0;
  for (; x; x -= x & -x) r += bit_[x];
  return r;
}
long long ans_;

void cdq(int l, int r) {
  if (r - l <= 1) return;
  int m = (l + r) / 2;
  cdq(l, m), cdq(m, r);
  int p = l;
  for (int t = m; t < r; ++t) {
    while (p < m && v_[p].b <= v_[t].b) bit_add(v_[p].c, 1), ++p;
    ans_ += bit_sum(v_[t].c);
  }
  for (int t = l; t < p; ++t) bit_add(v_[t].c, -1);
  int i = l, j = m, k = l;
  while (i < m && j < r) tmp_[k++] = v_[i].b <= v_[j].b ? v_[i++] : v_[j++];
  while (i < m) tmp_[k++] = v_[i++];
  while (j < r) tmp_[k++] = v_[j++];
  copy(tmp_ + l, tmp_ + r, v_ + l);
}

long long count_3d(vector<array<int, 3>> pts) {
  int n = pts.size();
  vector<int> cs;
  for (auto &p : pts) cs.push_back(p[2]);
  sort(cs.begin(), cs.end()), cs.erase(unique(cs.begin(), cs.end()), cs.end());
  C_ = cs.size();
  for (int i = 0; i < n; ++i)
    v_[i] = {pts[i][0], pts[i][1], (int)(lower_bound(cs.begin(), cs.end(), pts[i][2]) - cs.begin()) + 1};

  stable_sort(v_, v_ + n, [](const Node &x, const Node &y) {
    return x.a != y.a ? x.a < y.a : (x.b != y.b ? x.b < y.b : x.c < y.c);
  });
  ans_ = 0;
  cdq(0, n);
  return ans_;
}

