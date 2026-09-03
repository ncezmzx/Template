#include <bits/stdc++.h>
using namespace std;
#define int long long


template <size_t N> struct wqs_mst {
  struct e {
    int x, y, z, c;
    bool operator<(const e r) const { return z == r.z ? c < r.c : z < r.z; }
  } es[N];
  int n, m, k, fa[N];
  int spi[N], nsi[N], cs, cn;
  bool es_sorted = false;
  int get(int x) { return x == fa[x] ? x : fa[x] = get(fa[x]); }
  void ensure_sorted() {
    if (es_sorted) return;
    cs = cn = 0;
    for (int i = 1; i <= m; ++i) (es[i].c ? nsi[++cn] : spi[++cs]) = i;
    auto byz = [this](int i, int j) { return es[i].z != es[j].z ? es[i].z < es[j].z : es[i].c < es[j].c; };
    sort(spi + 1, spi + cs + 1, byz), sort(nsi + 1, nsi + cn + 1, byz);
    es_sorted = true;
  }


  pair<int, int> calc(int x) {
    ensure_sorted();
    iota(fa, fa + n, 0);
    int p = 0, q = 0;
    for (int i = 1, j = 1, c = 0; c < n - 1 && (i <= cs || j <= cn);) {
      bool sp;
      if (i > cs) sp = false;
      else if (j > cn) sp = true;
      else sp = es[spi[i]].z - x <= es[nsi[j]].z;
      int id = sp ? spi[i++] : nsi[j++];
      int a = get(es[id].x), b = get(es[id].y);
      if (a == b) continue;
      fa[a] = b, ++c, p += es[id].z - (es[id].c ? 0 : x), q += !es[id].c;
    }
    return {p, q};
  }
};

