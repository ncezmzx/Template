#define int long long

template <size_t N> struct suffix_automaton {
  int ch[N][26], lnk[N], len[N], sz[N];
  int buc[N], o[N];
  int tot, lst;
  void init() {
    tot = lst = 1;
    memset(ch[1], 0, sizeof ch[1]);
    lnk[1] = len[1] = sz[1] = 0;
  }
  void extend(int c) {
    int cur = ++tot, p = lst;
    len[cur] = len[lst] + 1, sz[cur] = 1;
    memset(ch[cur], 0, sizeof ch[cur]);
    for (; p && !ch[p][c]; p = lnk[p]) ch[p][c] = cur;
    if (!p) lnk[cur] = 1;
    else {
      int q = ch[p][c];
      if (len[q] == len[p] + 1) lnk[cur] = q;
      else {
        int cp = ++tot;
        memcpy(ch[cp], ch[q], sizeof ch[q]);
        len[cp] = len[p] + 1, lnk[cp] = lnk[q], sz[cp] = 0;
        for (; p && ch[p][c] == q; p = lnk[p]) ch[p][c] = cp;
        lnk[q] = lnk[cur] = cp;
      }
    }
    lst = cur;
  }

  void count() {
    memset(buc, 0, (tot + 1) * sizeof(int));
    for (int i = 1; i <= tot; ++i) ++buc[len[i]];
    for (int i = 1; i <= tot; ++i) buc[i] += buc[i - 1];
    for (int i = 1; i <= tot; ++i) o[buc[len[i]]--] = i;
    for (int i = tot; i >= 1; --i) sz[lnk[o[i]]] += sz[o[i]];
  }
};

