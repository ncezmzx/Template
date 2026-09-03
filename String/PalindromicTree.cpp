#include <bits/stdc++.h>
using namespace std;


template <size_t N> struct palindromic_tree {
  int tot, last_;
  int ch[N][26], fail_[N], len_[N], cnt_[N];
  char s_[N];
  void init() {
    tot = 1, last_ = 0, s_[0] = '#';
    len_[0] = 0, len_[1] = -1;
    fail_[0] = 1, fail_[1] = 0;
    memset(ch[0], 0, sizeof ch[0]), memset(ch[1], 0, sizeof ch[1]);
  }
  int get_fail(int p, int i) {
    while (s_[i - len_[p] - 1] != s_[i]) p = fail_[p];
    return p;
  }
  void extend(int i) {
    int c = s_[i] - 'a', p = get_fail(last_, i);
    if (!ch[p][c]) {
      int now = ++tot;
      len_[now] = len_[p] + 2;
      fail_[now] = ch[get_fail(fail_[p], i)][c];
      memset(ch[now], 0, sizeof ch[now]);
      cnt_[now] = 0;
      ch[p][c] = now;
    }
    last_ = ch[p][c];
    cnt_[last_]++;
  }
  void build(const char *s) {
    init();
    for (int i = 1; s[i]; ++i) s_[i] = s[i], extend(i);
  }
  void tally() {
    for (int i = tot; i >= 2; --i) cnt_[fail_[i]] += cnt_[i];
  }
};

