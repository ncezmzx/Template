
constexpr int N = 5e5 + 9, MQ = 5e5 + 9;
int a[N], cnt_[N], cur, ans[MQ];

struct Qry {
  int l, r, t, id;
};
inline void mo_add(int i) { cur += cnt_[a[i]]++ == 0; }
inline void mo_del(int i) { cur -= --cnt_[a[i]] == 0; }

vector<int> mo_distinct(int n, const vector<int> &val, vector<array<int, 2>> qs) {
  int q = qs.size(), B = max(1, (int)(n / max(1.0, sqrt((double)q))));
  vector<Qry> Q(q);
  for (int i = 0; i < q; ++i) Q[i] = {qs[i][0], qs[i][1], 0, i};
  sort(Q.begin(), Q.end(), [&](const Qry &x, const Qry &y) {
    int bx = x.l / B, by = y.l / B;
    if (bx != by) return bx < by;
    return bx & 1 ? x.r > y.r : x.r < y.r;
  });
  for (int i = 1; i <= n; ++i) a[i] = val[i], cnt_[a[i]] = 0;
  cur = 0;
  int l = 1, r = 0;
  vector<int> res(q);
  for (auto &Q_ : Q) {
    while (l > Q_.l) mo_add(--l);
    while (r < Q_.r) mo_add(++r);
    while (l < Q_.l) mo_del(l++);
    while (r > Q_.r) mo_del(r--);
    res[Q_.id] = cur;
  }
  return res;
}

struct Upd {
  int pos, val;
};

vector<int> mo_distinct_modify(int n, const vector<int> &val, const vector<array<int, 2>> qs,
                               const vector<array<int, 2>> ups) {
  int q = qs.size(), t = ups.size();
  vector<array<int, 2>> up(ups);
  int B = max(1, (int)pow(n, 2.0 / 3));
  vector<Qry> Q(q);
  for (int i = 0; i < q; ++i) Q[i] = {qs[i][0], qs[i][1], t, i};
  sort(Q.begin(), Q.end(), [&](const Qry &x, const Qry &y) {
    int bx = x.l / B, by = y.l / B;
    if (bx != by) return bx < by;
    int rx = x.r / B, ry = y.r / B;
    if (rx != ry) return rx < ry;
    return x.t < y.t;
  });
  for (int i = 1; i <= n; ++i) a[i] = val[i];
  for (int i = 0; i < N; ++i) cnt_[i] = 0;
  cur = 0;
  int l = 1, r = 0, now = 0;
  vector<int> res(q);
  for (auto &Q_ : Q) {
    while (now < Q_.t) {
      int p = up[now][0];
      if (l <= p && p <= r) mo_del(p), swap(a[p], up[now][1]), mo_add(p);
      else swap(a[p], up[now][1]);
      ++now;
    }
    while (now > Q_.t) {
      --now;
      int p = up[now][0];
      if (l <= p && p <= r) mo_del(p), swap(a[p], up[now][1]), mo_add(p);
      else swap(a[p], up[now][1]);
    }
    while (l > Q_.l) mo_add(--l);
    while (r < Q_.r) mo_add(++r);
    while (l < Q_.l) mo_del(l++);
    while (r > Q_.r) mo_del(r--);
    res[Q_.id] = cur;
  }
  return res;
}

