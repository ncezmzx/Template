#include <bits/stdc++.h>
using namespace std;
#define int long long

// 2-SAT: literals of variable i are 2i (true) and 2i+1 (false); edge u -> v = "u true implies v true"
struct two_sat {
  int _n, ecnt = 0;
  vector<int> hd, des, nxt;  // forward-star adjacency
  vector<bool> ans;

  two_sat(int n_ = 0) : _n(n_), hd(2 * n_, -1), ans(n_) { des.reserve(4 * n_), nxt.reserve(4 * n_); }
  void add_edge(int u, int v) { des.push_back(v), nxt.push_back(hd[u]), hd[u] = ecnt++; }
  // add clause (i is f) OR (j is g)
  void add_clause(int i, bool f, int j, bool g) {
    add_edge(2 * i + (f ? 0 : 1), 2 * j + (g ? 1 : 0));  // ¬(i,f) -> (j,g)
    add_edge(2 * j + (g ? 0 : 1), 2 * i + (f ? 1 : 0));  // ¬(j,g) -> (i,f)
  }
  void add_if(int i, bool f, int j, bool g) { add_clause(i, !f, j, g); }  // (i,f) -> (j,g)
  void add_not(int i, bool f) { add_clause(i, !f, i, !f); }               // ¬(i,f)
  void add_xor(int i, bool f, int j, bool g) {                            // exactly one of (i,f), (j,g)
    add_clause(i, f, j, g), add_clause(i, !f, j, !g);                      // (a∨b) ∧ (¬a∨¬b)
  }
  void add_eq(int i, bool f, int j, bool g) { add_xor(i, !f, j, g); }  // (i,f) <=> (j,g)

  // solve: satisfiable? when yes, answer()[i] gives variable i (iterative Tarjan SCC)
  bool satisfiable() {
    int m = 2 * _n, idx = 0, cl = 0;
    vector<int> dfn(m, 0), low(m, 0), col(m, 0), ins(m, 0), stk(m), cur(hd), fs;  // fs = explicit call stack
    for (int rt = 0; rt < m; rt++) {
      if (dfn[rt]) continue;
      int tp = 0;  // Tarjan stack
      dfn[rt] = low[rt] = ++idx, stk[tp++] = rt, ins[rt] = 1, fs.push_back(rt);
      while (!fs.empty()) {
        int v = fs.back();
        if (cur[v] != -1) {          // unexpanded outgoing edges remain
          int w = des[cur[v]];
          cur[v] = nxt[cur[v]];
          if (!dfn[w]) {
            dfn[w] = low[w] = ++idx, stk[tp++] = w, ins[w] = 1, fs.push_back(w);
          } else if (ins[w]) {
            low[v] = min(low[v], dfn[w]);
          }
        } else {                     // v fully expanded: pop SCC
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
      ans[i] = col[2 * i] > col[2 * i + 1];  // smaller Tarjan id = later in topo order; take source side
    }
    return true;
  }
  vector<bool> answer() { return ans; }
};

/*
 * ============================================================
 * ============================================================
 * Name: 2-SAT (implication graph + iterative Tarjan SCC, ACL twosat interface)
 * Complexity: O(n + m) (n variables, m clauses); fully iterative (deep graphs safe)
 * Usage: satisfiability of boolean formulas whose clauses have at most two
 *        literals; decides and produces one assignment. Interface aligned
 *        with the AtCoder Library: add_clause(i, f, j, g) adds
 *        (xi==f) OR (xj==g) (0-indexed variables); satisfiable() decides;
 *        answer() returns the assignment; plus composite helpers add_if /
 *        add_not / add_xor / add_eq
 * Principle: literal u = 2i+f is a vertex; clause (a OR b) adds edges
 *        not-a -> b and not-b -> a; literals in one SCC share their truth
 *        value; i and not-i in one SCC = unsatisfiable; otherwise set each
 *        variable by the literal on the "closer to the source" side in
 *        topological order — Tarjan emits component ids in reverse
 *        topological order, hence ans[i] = (col[2i] > col[2i+1])
 * Notes: satisfiable() re-runs Tarjan internally (repeated calls agree);
 *        do not read answer() before satisfiable(); variables 0-indexed
 * Source: modeled on AtCoder Library twosat.hpp (same interface, SCC via
 *         iterative Tarjan)
 * ============================================================
 * Example (uncomment to compile; Luogu P4782 style input):

 * signed main() {
 *   int n = 3, m = 3;                         // 3 variables, 3 clauses
 *   two_sat ts(n);
 *   ts.add_clause(0, true, 1, false);          // x0 ∨ ¬x1
 *   ts.add_clause(1, true, 2, true);           // x1 ∨ x2
 *   ts.add_clause(0, false, 2, false);         // ¬x0 ∨ ¬x2
 *   if (!ts.satisfiable()) cout << "NO\n";
 *   else {
 *     auto a = ts.answer();
 *     cout << "YES\n";                         // e.g. x0=0, x1=0, x2=1
 *     for (int i = 0; i < n; i++) cout << a[i] << " \n"[i == n - 1];
 *   }
 *   // composite clauses:
 *   two_sat t2(2);
 *   t2.add_if(0, true, 1, true);               // x0 -> x1
 *   t2.add_xor(0, true, 1, false);             // exactly one of x0, not-x1 (i.e. x0 == x1)
 *   cout << (t2.satisfiable() ? "SAT" : "UNSAT") << '\n';  // SAT (x0=x1=0 works)
 * }
 */
