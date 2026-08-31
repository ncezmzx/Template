#include <bits/stdc++.h>
using namespace std;
#define int long long

// WQS binary search + Kruskal: MST with exactly k special edges (c == 0)
template <size_t N> struct wqs_mst {
  struct e {
    int x, y, z, c;
    bool operator<(const e r) const { return z == r.z ? c < r.c : z < r.z; }
  } es[N];
  int n, m, k, fa[N];
  int spi[N], nsi[N], cs, cn; // special/normal edge index tables, sorted once
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
  // Kruskal with penalty x subtracted from every special edge;
  // returns {cost (special edges charged z - x), number of special edges used}
  pair<int, int> calc(int x) {
    ensure_sorted(); // uniform -x keeps in-group order, one sort suffices
    iota(fa, fa + n, 0);
    int p = 0, q = 0;
    for (int i = 1, j = 1, c = 0; c < n - 1 && (i <= cs || j <= cn);) {
      bool sp;
      if (i > cs) sp = false;
      else if (j > cn) sp = true;
      else sp = es[spi[i]].z - x <= es[nsi[j]].z; // tie: special edge first
      int id = sp ? spi[i++] : nsi[j++];
      int a = get(es[id].x), b = get(es[id].y);
      if (a == b) continue;
      fa[a] = b, ++c, p += es[id].z - (es[id].c ? 0 : x), q += !es[id].c;
    }
    return {p, q};
  }
};

/*
 * ============================================================
 * Name: WQS binary search + Kruskal (MST with exactly k special edges)
 * Complexity: O(m log m * logV); each calc(x) runs one linear-merge Kruskal
 * Usage: MST using exactly k special edges, `wqs_mst<N>`: fill es[1..m] (c = 0
 *        marks a special edge), set n and m, then binary-search the penalty:
 *        subtract x from every special edge in calc(x), find the largest
 *        penalty l with q >= k special edges, and the answer is p(l) + l * k.
 * Source: all.cpp lines 28716-28755 (wrapped into a struct, logic unchanged)
 * Notes: ties prefer the special edge (c = 0); fa is 0-indexed; for negative
 *        weights adjust the search bounds
 * ============================================================
 */
