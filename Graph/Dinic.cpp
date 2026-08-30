#include <bits/stdc++.h>
using namespace std;

template <class T> struct dinic {
  vector<int> cur, hd, nxt, to, dis;
  vector<T> cap;
  static constexpr T inf = numeric_limits<T>::max();
  int tot;
  dinic(int _n, int _m) : cur(_n + 2), hd(_n + 2), nxt(_m * 2 + 2), to(_m * 2 + 2), cap(_m * 2 + 2), dis(_n + 2), tot(1) {}
  void add(int u, int v, T w) {
    nxt[++tot] = hd[u], hd[u] = tot, to[tot] = v, cap[tot] = w;
    nxt[++tot] = hd[v], hd[v] = tot, to[tot] = u, cap[tot] = 0;
  }
  T dfs(int u, int t, T flow) {
    if (u == t) return flow;
    T ret = 0;
    int du = dis[u] + 1;   // hoisted level, avoids per-edge recomputation
    for (int i = cur[u]; i && flow; i = nxt[i]) {
      cur[u] = i;
      int v = to[i];
      T c = min(cap[i], flow);
      if (c > 0 && dis[v] == du) {
        T tmp = dfs(v, t, c);
        ret += tmp, cap[i] -= tmp, cap[i ^ 1] += tmp, flow -= tmp;
      }
    }
    if (!ret) dis[u] = -1;
    return ret;
  }
  T calc(int s, int t) {
    T ret = 0;
    vector<int> q(dis.size());   // flat array queue replaces per-phase std::queue
    while (true) {
      fill(dis.begin(), dis.end(), -1);
      int qh = 0, qt = 0;
      q[qt++] = s, dis[s] = 0;
      cur = hd;
      while (qh < qt) {
        int u = q[qh++], du = dis[u] + 1;
        for (int i = hd[u]; i; i = nxt[i]) {
          int v = to[i];
          if (dis[v] == -1 && cap[i] > 0) dis[v] = du, q[qt++] = v;
        }
      }
      if (dis[t] == -1) return ret;
      ret += dfs(s, t, inf);
    }
  }
};

/*
 * ============================================================
 * Name: Dinic maximum flow (current-arc optimization)
 * Complexity: O(V^2 E) worst case, much faster in practice
 * Usage: `dinic<T>`: construct dinic<T>(n, m) (m = number of add calls), add(u,
 *        v, w), then calc(s, t);
 *        use T = long long for large capacities.
 * Implementation: BFS layers + DFS blocking flow with current-arc pointers;
 *        a flat array queue replaces std::queue
 * Source: all.cpp (vector version dinic<T>; wrapped, logic unchanged)
 * ============================================================
 */
