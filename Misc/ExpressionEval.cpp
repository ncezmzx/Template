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
      long long e = parse_pow(); // right-associative
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
    if (pos >= (int)s.size()) return 0; // guard truncated input (never hit on valid input)
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

  long long solve(const string &str) {
    s = str, pos = 0;
    long long res = parse_expr();
    while (pos < (int)s.size() && s[pos] == ' ') ++pos;
    return res;
  }
};

/*
 * ============================================================
 * Name: expression evaluation (recursive descent: + - * / % ^ parentheses, unary minus, variables)
 * Complexity: O(expression length)
 * Usage: arithmetic expression parsing: the four operations, modulo, power (^,
 *        right-associative), parentheses, unary +/-, and letter variables
 *        (assigned via the var map).
 * Principle: recursive descent: expr -> term (+|-) term; term -> pow (*|/|%)
 *            pow; pow -> uni (^ pow), so ^ is right-associative; uni -> (+|-)
 *            uni | pri; pri -> (expr) | variable | number
 * Notes: the expression must contain no spaces (strip them first); powers
 *        multiply step by step, so large exponents overflow or time out (use
 *        fast exponentiation instead); divide / modulo by zero is UB; undefined
 *        variables read as 0
 * ============================================================
 */
