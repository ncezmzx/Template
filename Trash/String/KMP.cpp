#include <bits/stdc++.h>
using namespace std;
#define int long long


constexpr int N = 1e6 + 9;
int n, nxt[N];
string s;


void build_nxt() {
  for (int j = 0, i = 2; i <= n; ++i) {
    while (j && s[j + 1] != s[i]) j = nxt[j];
    j = nxt[i] = j + (s[j + 1] == s[i]);
  }
}


int match(const string &t, const string &s) {
  int m = t.size() - 1, M = s.size() - 1, cnt = 0;
  for (int i = 1, j = 0; i <= M; ++i) {
    while (j && t[j + 1] != s[i]) j = nxt[j];
    if ((j += (t[j + 1] == s[i])) == m) ++cnt;
  }
  return cnt;
}

