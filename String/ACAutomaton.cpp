#include <bits/stdc++.h>
using namespace std;
#define int long long


template <size_t N> struct ac_automaton {
  int ch[N][26], fail[N], ed[N], cnt[N], inq[N], q[N];
  int tot, hd, tl;
  void init() {
    tot = 0;
    memset(ch[0], 0, sizeof ch[0]);
    ed[0] = 0;
  }
  void insert(const string &s) {
    int u = 0;
    for (char c : s) {
      int x = c - 'a';
      if (!ch[u][x]) {
        ch[u][x] = ++tot;
        memset(ch[tot], 0, sizeof ch[tot]);
        ed[tot] = 0;
      }
      u = ch[u][x];
    }
    ++ed[u];
  }

  void build() {
    hd = tl = 0;
    for (int i = 0; i < 26; ++i)
      if (ch[0][i]) q[++tl] = ch[0][i];
    while (hd < tl) {
      int u = q[++hd];
      for (int i = 0; i < 26; ++i) {
        if (ch[u][i]) fail[ch[u][i]] = ch[fail[u]][i], q[++tl] = ch[u][i];
        else ch[u][i] = ch[fail[u]][i];
      }
    }
  }

  int query(const string &s) {
    int u = 0, res = 0;
    for (char c : s) {
      u = ch[u][c - 'a'];
      for (int v = u; v && ed[v] != -1; v = fail[v]) {
        if (ed[v] > 0) ++res;
        ed[v] = -1;
      }
    }
    return res;
  }

  void count(const string &t) {
    int u = 0;
    memset(cnt, 0, (tot + 1) * sizeof(int));
    memset(inq, 0, (tot + 1) * sizeof(int));
    for (char c : t) ++cnt[u = ch[u][c - 'a']];
    hd = tl = 0;
    for (int i = 1; i <= tot; ++i) ++inq[fail[i]];
    for (int i = 1; i <= tot; ++i)
      if (!inq[i]) q[++tl] = i;
    while (hd < tl) {
      int u = q[++hd];
      cnt[fail[u]] += cnt[u];
      if (!--inq[fail[u]]) q[++tl] = fail[u];
    }
  }
};

