#include <bits/stdc++.h>
using namespace std;
#define int long long

struct expr_eval {
  string s;
  int pos;
  map<string, long long> var;

  long long parse_expr() {
    long long res = parse_term();
    while (pos < (int)s.size() && (s[pos] == '+' || s[pos] == '-')) {
      char op = s[pos++];
      long long rhs = parse_term();
      res = op == '+' ? res + rhs : res - rhs;
    }
    return res;
  }

  long long parse_term() {
    long long res = parse_pow();
    while (pos < (int)s.size() && (s[pos] == '*' || s[pos] == '/' || s[pos] == '%')) {
      char op = s[pos++];
      long long rhs = parse_pow();
      res = op == '*' ? res * rhs : op == '/' ? res / rhs : res % rhs;
    }
    return res;
  }

  long long parse_pow() {
    long long res = parse_uni();
    if (pos < (int)s.size() && s[pos] == '^') {
      ++pos;
      long long e = parse_pow();  // 右结合
      long long r = 1;
      for (long long i = 0; i < e; ++i) r *= res;
      return r;
    }
    return res;
  }

  long long parse_uni() {
    if (pos < (int)s.size() && s[pos] == '-') {
      ++pos;
      return -parse_uni();
    }
    if (pos < (int)s.size() && s[pos] == '+') {
      ++pos;
      return parse_uni();
    }
    return parse_pri();
  }

  long long parse_pri() {
    if (pos >= (int)s.size()) return 0;   // 防御残缺输入：避免读越界（合法输入不会走到）
    if (s[pos] == '(') {
      ++pos;
      long long res = parse_expr();
      ++pos;
      return res;
    }
    if (isalpha(s[pos])) {
      string name;
      while (pos < (int)s.size() && isalpha(s[pos])) name += s[pos++];
      return var[name];
    }
    long long res = 0;
    while (pos < (int)s.size() && isdigit(s[pos])) res = res * 10 + (s[pos++] - '0');
    return res;
  }

  long long solve(const string& str) {
    s = str, pos = 0;
    long long res = parse_expr();
    while (pos < (int)s.size() && s[pos] == ' ') ++pos;
    return res;
  }
};

/*
 * ============================================================
 * 名称：表达式求值（递归下降，支持 + - * / % ^ 括号 一元负号 变量）
 * 复杂度：O(表达式长度)
 * 用途：算术表达式解析：四则运算、取模、幂（^ 右结合）、括号、
 *       一元正负号、字母变量（var 映射赋值）
 * 原理：递归下降：expr -> term (+|-) term；term -> pow (*|/|%) pow；
 *       pow -> uni (^ pow)（右结合）；uni -> (+|-) uni | pri；
 *       pri -> (expr) | 变量 | 数字
 * 注意：表达式需无空格（或用前先去除）；幂运算逐次相乘（指数大时溢出
 *       或超时，改快速幂）；除零/模零为 UB；未定义变量返回 0
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   expr_eval ev;
 *   ev.var["x"] = 3;
 *   cout << ev.solve("1+2*3") << '\n';        // 7
 *   cout << ev.solve("(1+2)*3") << '\n';      // 9
 *   cout << ev.solve("2^3^2") << '\n';        // 512（右结合）
 *   cout << ev.solve("-5%3") << '\n';         // -2
 *   cout << ev.solve("x*x+1") << '\n';        // 10
 * }
 * ============================================================
 */
