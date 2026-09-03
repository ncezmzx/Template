#include <bits/stdc++.h>
using namespace std;


template <size_t N, size_t SP> struct persistent_binary_trie {
  static constexpr int B = 24;
  int tot, cnt_, rt[N], tr_[SP][2], sz[SP];
  void init() { tot = 0, cnt_ = 0, rt[0] = 0, tr_[0][0] = tr_[0][1] = 0, sz[0] = 0; }
  int insert(int x) {
    int pre = rt[cnt_], u = ++tot;
    rt[++cnt_] = u;
    tr_[u][0] = tr_[pre][0], tr_[u][1] = tr_[pre][1], sz[u] = sz[pre] + 1;
    for (int cur = u, i = B - 1; i >= 0; --i) {
      int b = x >> i & 1, nxt = ++tot;
      pre = tr_[pre][b];
      tr_[nxt][0] = tr_[pre][0], tr_[nxt][1] = tr_[pre][1], sz[nxt] = sz[pre] + 1;
      tr_[cur][b] = nxt, cur = nxt;
    }
    return cnt_;
  }

  int qmax(int l, int r, int x) {
    int u = rt[r], v = rt[l - 1], res = 0;
    for (int i = B - 1; i >= 0; --i) {
      int b = (x >> i & 1) ^ 1;
      if (sz[tr_[u][b]] - sz[tr_[v][b]] > 0) res |= 1 << i;
      else b ^= 1;
      u = tr_[u][b], v = tr_[v][b];
    }
    return res;
  }

  int count_le(int l, int r, int x) {
    int u = rt[r], v = rt[l - 1], res = 0;
    for (int i = B - 1; i >= 0; --i) {
      int b = x >> i & 1;
      if (b) res += sz[tr_[u][0]] - sz[tr_[v][0]];
      if (sz[tr_[u][b]] - sz[tr_[v][b]] == 0) return res;
      u = tr_[u][b], v = tr_[v][b];
    }
    return res + sz[u] - sz[v];
  }
};

