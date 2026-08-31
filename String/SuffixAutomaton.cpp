#include <bits/stdc++.h>
using namespace std;
#define int long long

// Suffix automaton over lowercase letters; states <= 2n
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
  // counting sort by len, then sz[u] = endpos size of state u (occurrences)
  void count() {
    memset(buc, 0, (tot + 1) * sizeof(int));
    for (int i = 1; i <= tot; ++i) ++buc[len[i]];
    for (int i = 1; i <= tot; ++i) buc[i] += buc[i - 1];
    for (int i = 1; i <= tot; ++i) o[buc[len[i]]--] = i;
    for (int i = tot; i >= 1; --i) sz[lnk[o[i]]] += sz[o[i]];
  }
};

/*
 * ============================================================
 * Name: suffix automaton (SAM)
 * Complexity: build O(n * |Sigma|) (transition table ch[N][26]); <= 2n states,
 *             <= 3n edges
 * Usage: all substring information of one string, `suffix_automaton<N>`.
 *        Classic facts:
 *        number of distinct substrings = sum_{i=2..tot} (len[i] - len[lnk[i]]);
 *        after count(), sz[i] is the size of state i's endpos set, i.e. the
 *        occurrence count of the substrings state i represents (the root state
 *        1 is meaningless);
 *        minimal representation, longest common substring and k-th smallest
 *        substring all extend this skeleton.
 *        Call pattern: sam.init(); for (char c : s) sam.extend(c - 'a');
 *        sam.count();
 * Principle: online construction; each state is an endpos equivalence class and
 *            lnk points to the longest-suffix state with strictly smaller len
 *            (the suffix-link tree)
 * Notes: lowercase alphabet (26); for other alphabets change ch's second
 *        dimension and c's values; take N >= 2*|S| + 5; init() between test
 *        cases
 * ============================================================
 */
