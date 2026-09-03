#include <bits/stdc++.h>
using namespace std;


constexpr int N = 2e5 + 9;
int a[N], first[N], last[N];
int sv[N], sf[N], stk_top;
int ts[N], tmpfirst[N], timer;
int cur;

inline void radd(int i) {
  int v = a[i];
  if (first[v] == 0) first[v] = last[v] = i;
  else last[v] = i, cur = max(cur, last[v] - first[v]);
}
inline void ladd(int i) {
  int v = a[i];
  sv[++stk_top] = v, sf[stk_top] = first[v];
  if (first[v] == 0) first[v] = last[v] = i;
  else first[v] = i, cur = max(cur, last[v] - first[v]);
}
inline void lrollback() {
  for (; stk_top; --stk_top) first[sv[stk_top]] = sf[stk_top];
}

struct Qry {
  int l, r, id;
};

vector<int> rollback_mo(int n, const vector<int> &val, const vector<array<int, 2>> &qs) {

  vector<int> b(val.begin() + 1, val.end());
  sort(b.begin(), b.end()), b.erase(unique(b.begin(), b.end()), b.end());
  for (int i = 1; i <= n; ++i) a[i] = (int)(lower_bound(b.begin(), b.end(), val[i]) - b.begin()) + 1;
  int V = b.size(), q = qs.size();
  int B = max(1, (int)(n / max(1.0, sqrt((double)q))));
  vector<Qry> Q(q);
  for (int i = 0; i < q; ++i) Q[i] = {qs[i][0], qs[i][1], i};
  sort(Q.begin(), Q.end(), [&](const Qry &x, const Qry &y) {
    int bx = x.l / B, by = y.l / B;
    if (bx != by) return bx < by;
    return x.r < y.r;
  });
  vector<int> ans(q);
  for (int i = 0; i < q;) {
    int b = Q[i].l / B;
    int R = min(n, (b + 1) * B);
    for (int v = 1; v <= V; ++v) first[v] = last[v] = 0;
    cur = 0;
    int r = R;
    for (; i < q && Q[i].l / B == b; ++i) {
      if (Q[i].r <= R) {
        int best = 0, t = ++timer;
        for (int p = Q[i].l; p <= Q[i].r; ++p) {
          int v = a[p];
          if (ts[v] != t) ts[v] = t, tmpfirst[v] = p;
          else best = max(best, p - tmpfirst[v]);
        }
        ans[Q[i].id] = best;
      }
      else {
        while (r < Q[i].r) radd(++r);
        int saved = cur;
        stk_top = 0;
        int l = R + 1;
        while (l > Q[i].l) ladd(--l);
        ans[Q[i].id] = cur;
        lrollback();
        cur = saved;
      }
    }
  }
  return ans;
}

