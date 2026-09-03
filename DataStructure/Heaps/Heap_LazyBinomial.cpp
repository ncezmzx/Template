#include <bits/stdc++.h>
using namespace std;
#define int long long


template <size_t N> struct heap_lazy_binomial {
  int a[N], fa[N], vl[N], tp[N], rk[N], pos[N], msk[N];
  list<int> nd[N], sn[N];
  typename list<int>::iterator it[N];
  void add(list<int> &x, int y) { it[y] = x.insert(x.end(), y); }
  void cmin(int &x, int y) { x > y && (x = y); }
  int newnode(int x, int i) { return add(nd[i], i), tp[i] = vl[i] = x, pos[i] = msk[i] = i; }
  int top(int x) { return tp[x]; }
  void join(int x, int y) {
    cmin(tp[x], tp[y]), nd[x].splice(nd[x].end(), nd[y]);
  }
  void decrease_key(int h, int p, int v) {
    cmin(tp[h], vl[p = pos[p]] = v);
    for (int f = fa[p]; f && vl[f] > v; f = fa[p = f])
      swap(msk[p], msk[f]), swap(pos[msk[p]], pos[msk[f]]), swap(vl[p], vl[f]);
  }
  void erase(int h, int x) {
    decrease_key(h, x, LLONG_MIN);
    for (int y : sn[x = pos[x]]) add(nd[h], y), fa[y] = 0;
    nd[h].erase(it[x]), sn[x].clear();
    int mx = -1;
    for (int x : nd[h]) {
      while (int y = a[rk[x]]) {
        if (vl[x] > vl[y]) swap(x, y);
        add(sn[fa[y] = x], y), a[rk[x]++] = 0;
      }
      mx = max(mx, rk[x]), a[rk[x]] = x;
    }
    nd[h].clear(), tp[h] = LLONG_MAX;
    for (int i = 0; i <= mx; ++i)
      if (int &x = a[i]) add(nd[h], x), cmin(tp[h], vl[x]), x = 0;
  }
};

