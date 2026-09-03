#include <bits/stdc++.h>
using namespace std;


constexpr int N = 2e5 + 9, M = 4e5 + 9;

struct euler_undirected {
  int n, ecnt = 1, hd[N], nxt[M], to[M], deg[N];
  bool used[M];
  void init(int n_) {
    n = n_, ecnt = 1;
    fill(hd + 1, hd + n + 1, 0), fill(deg + 1, deg + n + 1, 0);
    fill(used, used + M, false);
  }
  void add(int u, int v) {
    ++ecnt, to[ecnt] = v, nxt[ecnt] = hd[u], hd[u] = ecnt, ++deg[u];
    ++ecnt, to[ecnt] = u, nxt[ecnt] = hd[v], hd[v] = ecnt, ++deg[v];
  }
  int odd_cnt() {
    int c = 0;
    for (int i = 1; i <= n; ++i) c += deg[i] & 1;
    return c;
  }


  vector<int> path(int s) {
    vector<int> stk, res;
    stk.push_back(s);
    while (!stk.empty()) {
      int v = stk.back();
      int &e = hd[v];
      while (e && used[e]) e = nxt[e];
      if (e) {
        used[e] = used[e ^ 1] = true;
        stk.push_back(to[e]);
        e = nxt[e];
      }
      else res.push_back(v), stk.pop_back();
    }
    reverse(res.begin(), res.end());
    return res;
  }
};

struct euler_directed {
  int n, ecnt = 1, hd[N], nxt[M], to[M], in_[N], out_[N];
  void init(int n_) {
    n = n_, ecnt = 1;
    fill(hd + 1, hd + n + 1, 0), fill(in_ + 1, in_ + n + 1, 0), fill(out_ + 1, out_ + n + 1, 0);
  }
  void add(int u, int v) { ++ecnt, to[ecnt] = v, nxt[ecnt] = hd[u], hd[u] = ecnt, ++out_[u], ++in_[v]; }
  bool has_path(int &s) {
    int a = 0, b = 0;
    s = 0;
    for (int i = 1; i <= n; ++i) {
      int d = out_[i] - in_[i];
      if (d > 1 || d < -1) return false;
      if (d == 1) ++a, s = i;
      if (d == -1) ++b;
      if (!s && (out_[i] || in_[i])) s = i;
    }
    if (a == 0 && b == 0) return true;
    return a == 1 && b == 1;
  }

  vector<int> path(int s) {
    vector<int> stk, res;
    stk.push_back(s);
    while (!stk.empty()) {
      int v = stk.back();
      if (hd[v]) {
        int e = hd[v];
        hd[v] = nxt[e];
        stk.push_back(to[e]);
      }
      else res.push_back(v), stk.pop_back();
    }
    reverse(res.begin(), res.end());
    return res;
  }
};

