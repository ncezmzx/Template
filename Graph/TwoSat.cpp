#include <bits/stdc++.h>
using namespace std;
#define int long long


struct two_sat {
  int _n, ecnt = 0;
  vector<int> hd, des, nxt;
  vector<bool> ans;

  two_sat(int n_ = 0) : _n(n_), hd(2 * n_, -1), ans(n_) { des.reserve(4 * n_), nxt.reserve(4 * n_); }
  void add_edge(int u, int v) { des.push_back(v), nxt.push_back(hd[u]), hd[u] = ecnt++; }

  void add_clause(int i, bool f, int j, bool g) {
    add_edge(2 * i + (f ? 0 : 1), 2 * j + (g ? 1 : 0));
    add_edge(2 * j + (g ? 0 : 1), 2 * i + (f ? 1 : 0));
  }
  void add_if(int i, bool f, int j, bool g) { add_clause(i, !f, j, g); }
  void add_not(int i, bool f) { add_clause(i, !f, i, !f); }
  void add_xor(int i, bool f, int j, bool g) {
    add_clause(i, f, j, g), add_clause(i, !f, j, !g);
  }
  void add_eq(int i, bool f, int j, bool g) { add_xor(i, !f, j, g); }


  bool satisfiable() {
    int m = 2 * _n, idx = 0, cl = 0;
    vector<int> dfn(m, 0), low(m, 0), col(m, 0), ins(m, 0), stk(m), cur(hd), fs;
    for (int rt = 0; rt < m; rt++) {
      if (dfn[rt]) continue;
      int tp = 0;
      dfn[rt] = low[rt] = ++idx, stk[tp++] = rt, ins[rt] = 1, fs.push_back(rt);
      while (!fs.empty()) {
        int v = fs.back();
        if (cur[v] != -1) {
          int w = des[cur[v]];
          cur[v] = nxt[cur[v]];
          if (!dfn[w]) {
            dfn[w] = low[w] = ++idx, stk[tp++] = w, ins[w] = 1, fs.push_back(w);
          }
          else if (ins[w]) {
            low[v] = min(low[v], dfn[w]);
          }
        }
        else {
          fs.pop_back();
          if (!fs.empty()) low[fs.back()] = min(low[fs.back()], low[v]);
          if (low[v] == dfn[v]) {
            ++cl;
            for (int u = stk[--tp];; u = stk[--tp]) {
              ins[u] = 0, col[u] = cl;
              if (u == v) break;
            }
          }
        }
      }
    }
    for (int i = 0; i < _n; i++) {
      if (col[2 * i] == col[2 * i + 1]) return false;
      ans[i] = col[2 * i] > col[2 * i + 1];
    }
    return true;
  }
  vector<bool> answer() { return ans; }
};

