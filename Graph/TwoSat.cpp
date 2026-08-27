#include <bits/stdc++.h>
using namespace std;
#define int long long

// 2-SAT：变量 i 的两literal为 2i（真）与 2i+1（假）；蕴含边 u -> v 表示"u 真 则 v 真"
struct two_sat {
  int _n, ecnt = 0;
  vector<int> hd, des, nxt;  // 链式前向星
  vector<bool> ans;

  two_sat(int n_ = 0) : _n(n_), hd(2 * n_, -1), ans(n_) { des.reserve(4 * n_), nxt.reserve(4 * n_); }
  void add_edge(int u, int v) { des.push_back(v), nxt.push_back(hd[u]), hd[u] = ecnt++; }
  // 加入子句 (i 选 f) 或 (j 选 g)；f/g 为真值
  void add_clause(int i, bool f, int j, bool g) {
    add_edge(2 * i + (f ? 0 : 1), 2 * j + (g ? 1 : 0));  // ¬(i,f) -> (j,g)
    add_edge(2 * j + (g ? 0 : 1), 2 * i + (f ? 1 : 0));  // ¬(j,g) -> (i,f)
  }
  void add_if(int i, bool f, int j, bool g) { add_clause(i, !f, j, g); }  // (i,f) -> (j,g)
  void add_not(int i, bool f) { add_clause(i, !f, i, !f); }               // ¬(i,f)
  void add_xor(int i, bool f, int j, bool g) {                            // (i,f) 与 (j,g) 恰一真
    add_clause(i, f, j, g), add_clause(i, !f, j, !g);                      // (a∨b) ∧ (¬a∨¬b)
  }
  void add_eq(int i, bool f, int j, bool g) { add_xor(i, !f, j, g); }  // (i,f) <=> (j,g)

  // 求解：返回是否有解；有解时 answer()[i] 为变量 i 的取值（迭代 Tarjan 求 SCC）
  bool satisfiable() {
    int m = 2 * _n, idx = 0, cl = 0;
    vector<int> dfn(m, 0), low(m, 0), col(m, 0), ins(m, 0), stk(m), cur(hd), fs;  // fs 为迭代调用栈
    for (int rt = 0; rt < m; rt++) {
      if (dfn[rt]) continue;
      int tp = 0;  // Tarjan 栈
      dfn[rt] = low[rt] = ++idx, stk[tp++] = rt, ins[rt] = 1, fs.push_back(rt);
      while (!fs.empty()) {
        int v = fs.back();
        if (cur[v] != -1) {          // 还有未扩展的出边
          int w = des[cur[v]];
          cur[v] = nxt[cur[v]];
          if (!dfn[w]) {
            dfn[w] = low[w] = ++idx, stk[tp++] = w, ins[w] = 1, fs.push_back(w);
          } else if (ins[w]) {
            low[v] = min(low[v], dfn[w]);
          }
        } else {                     // v 扩展完毕：回退 + 弹 SCC
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
      ans[i] = col[2 * i] > col[2 * i + 1];  // Tarjan 编号越小的 SCC 越靠拓扑序末端，取靠源点侧
    }
    return true;
  }
  vector<bool> answer() { return ans; }
};

/*
 * ============================================================
 * 名称：2-SAT（蕴含图 + 迭代 Tarjan SCC，对齐 ACL twosat 接口）
 * 复杂度：O(n + m)（n 变量、m 子句）；全程非递归（深图不爆栈）
 * 用途：布尔方程组合 satisfiability：每个子句至多含两个 literal，
 *       判定可满足性并给出一组解；接口与 AtCoder Library 一致：
 *       add_clause(i, f, j, g) 加子句 (xi==f) ∨ (xj==g)（变量 0-indexed）
 *       satisfiable() 判定；answer() 返回一组解
 *       另附常用复合子句 add_if / add_not / add_xor / add_eq
 * 原理：literal u=2i+f 建点；子句 (a ∨ b) 加边 ¬a→b、¬b→a；
 *       SCC 内 literal 同真假；i 与 ¬i 同 SCC 则无解；
 *       有解时每点取"拓扑序更靠源点"侧的 literal 为真——Tarjan 的
 *       分量编号按拓扑序反序生成，故 ans[i] = (col[2i] > col[2i+1])
 * 注意：satisfiable() 内部会重跑 Tarjan（多次调用结果一致）；
 *       调用 satisfiable() 前不要读 answer()；0-indexed 变量
 * 来源：参照 AtCoder Library twosat.hpp（接口一致，SCC 换为迭代 Tarjan）
 * ============================================================
 * 使用示例（编译时取消注释；洛谷 P4782 式输入）：
 * signed main() {
 *   int n = 3, m = 3;                         // 3 变量 3 子句
 *   two_sat ts(n);
 *   ts.add_clause(0, true, 1, false);          // x0 ∨ ¬x1
 *   ts.add_clause(1, true, 2, true);           // x1 ∨ x2
 *   ts.add_clause(0, false, 2, false);         // ¬x0 ∨ ¬x2
 *   if (!ts.satisfiable()) cout << "NO\n";
 *   else {
 *     auto a = ts.answer();
 *     cout << "YES\n";                         // 例如 x0=0, x1=0, x2=1
 *     for (int i = 0; i < n; i++) cout << a[i] << " \n"[i == n - 1];
 *   }
 *   // 复合子句：
 *   two_sat t2(2);
 *   t2.add_if(0, true, 1, true);               // x0 -> x1
 *   t2.add_xor(0, true, 1, false);             // x0 与 ¬x1 恰一真（即 x0 == x1）
 *   cout << (t2.satisfiable() ? "SAT" : "UNSAT") << '\n';  // SAT（x0=x1=0 即可）
 * }
 */
