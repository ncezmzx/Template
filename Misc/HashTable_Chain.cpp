#include <bits/stdc++.h>
using namespace std;
using u64 = unsigned long long;

mt19937_64 rng(random_device{}());

template <class T, size_t N, size_t Mod = 3217> struct Hash {
  unordered_map<T, int> mp;
  int hd[Mod], stk[N], tp, cnt[N], nxt[N];
  T w[N];
  Hash() {
    for (int i = 1; i < N; ++i) stk[++tp] = i;
  }
  void ins(T x, int y) {
    int u = x % Mod;
    for (int i = hd[u], pr = 0; i; pr = i, i = nxt[i]) {
      if (w[i] == x) {
        if (!(cnt[i] += y)) {
          stk[++tp] = i;
          if (pr) nxt[pr] = nxt[i];
          else hd[u] = nxt[i];
        }
        return;
      }
    }
    int i = stk[tp--];
    nxt[i] = hd[u], hd[u] = i, w[i] = x, cnt[i] += y;
  }
  int query(T x) {
    for (int i = hd[x % Mod]; i; i = nxt[i])
      if (w[i] == x) return cnt[i];
    return 0;
  }
};

