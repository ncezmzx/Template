#include <bits/stdc++.h>
using namespace std;
#define int long long


template <size_t N> struct heap_binomial {
  int a[N], fa[N], vl[N], tp[N], rk[N], pos[N], msk[N];
  list<int> nd[N], sn[N];
  typename list<int>::iterator it[N];
  void add(list<int> &x, int y) { it[y] = x.insert(x.end(), y); }
  void cmin(int &x, int y) { x > y && (x = y); }
  int newnode(int x, int i) { return add(nd[i], i), tp[i] = vl[i] = x, pos[i] = msk[i] = i; }
  int top(int x) { return tp[x]; }
  void join(int x, list<int> &ndy) {
    int mx = -1;
    for (int h : nd[x]) mx = max(mx, rk[h]), a[rk[h]] = h;
    for (int h : ndy) {
      while (int y = a[rk[h]]) {
        if (vl[h] > vl[y]) swap(h, y);
        add(sn[fa[y] = h], y), a[rk[h]++] = 0;
      }
      mx = max(mx, rk[h]), a[rk[h]] = h;
    }
    nd[x].clear(), tp[x] = LLONG_MAX;
    for (int i = 0; i <= mx; ++i)
      if (int &h = a[i]) add(nd[x], h), cmin(tp[x], vl[h]), h = 0;
  }
  void join(int x, int y) { join(x, nd[y]); }
  void decrease_key(int h, int p, int v) {
    cmin(tp[h], vl[p = pos[p]] = v);
    for (int f = fa[p]; f && vl[f] > v; f = fa[p = f])
      swap(msk[p], msk[f]), swap(pos[msk[p]], pos[msk[f]]), swap(vl[p], vl[f]);
  }
  void erase(int h, int x) {
    decrease_key(h, x, LLONG_MIN);
    for (int y : sn[x = pos[x]]) fa[y] = 0;
    nd[h].erase(it[x]), join(h, sn[x]);
  }
};

