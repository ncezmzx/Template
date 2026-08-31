#include <bits/stdc++.h>
using namespace std;
#define int long long

// minimum directed spanning tree (Chu-Liu), O(nm); 0-indexed
constexpr int NV = 109;
struct edge {
  int u, v, w;
};

// min arborescence rooted at root (Chu-Liu/Edmonds); edges in e are rewritten in place;
// returns total weight, or -1 if none exists; vertices 0-indexed
long long zhu_liu(int n, int m, int root, vector<edge> &e) {
  static int id_[NV], vis_[NV], pre[NV], ine[NV];
  long long res = 0;
  for (;;) {
    fill(ine, ine + n, LLONG_MAX);
    for (int i = 0; i < m; ++i) // cheapest incoming edge per vertex
      if (e[i].u != e[i].v && e[i].w < ine[e[i].v]) ine[e[i].v] = e[i].w, pre[e[i].v] = e[i].u;
    for (int i = 0; i < n; ++i)
      if (i != root && ine[i] == LLONG_MAX) return -1; // some vertex unreachable
    int cnt_ = 0;
    fill(id_, id_ + n, -1), fill(vis_, vis_ + n, -1);
    for (int i = 0; i < n; ++i) {
      if (i == root) continue;
      res += ine[i];
      int v = i; // follow pre to find cycles (timestamps avoid loops)
      while (vis_[v] != i && id_[v] == -1 && v != root) vis_[v] = i, v = pre[v];
      if (v != root && id_[v] == -1) { // new cycle: assign one id (do-while over v->pre->...->v)
        int u = v;
        do {
          id_[u] = cnt_;
          u = pre[u];
        } while (u != v);
        ++cnt_;
      }
    }
    if (!cnt_) return res; // no cycles: done
    for (int i = 0; i < n; ++i)
      if (id_[i] == -1) id_[i] = cnt_++; // ids for vertices outside cycles
    for (int i = 0; i < m; ++i) {        // contract: entering edges lose the counted in-edge weight
      int v = e[i].v;
      e[i].u = id_[e[i].u], e[i].v = id_[e[i].v];
      if (e[i].u != e[i].v) e[i].w -= ine[v];
    }
    n = cnt_, root = id_[root];
  }
}

/*
 * ============================================================
 * Name: minimum directed spanning tree (Chu-Liu / Edmonds algorithm)
 * Complexity: O(nm)
 * Usage: minimum out-arborescence rooted at root (every non-root has exactly
 *        one in-edge and root reaches everything):
 *        zhu_liu(n, m, root, e) returns the total weight, or -1 if none exists.
 * Principle: greedy: take each vertex's cheapest in-edge; contract every in-
 *            edge cycle into a super-vertex, subtracting the vertex's old in-
 *            edge weight from edges entering the cycle (swap cost), and repeat
 *            until acyclic
 * Notes: e is rewritten in place (copy it first if you need the originals);
 *        self-loops are ignored automatically
 * ============================================================
 */
