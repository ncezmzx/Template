#include <bits/stdc++.h>
using namespace std;
#define int long long

// Aho-Corasick automaton: trie + fail pointers over lowercase letters
template <size_t N>
struct ac_automaton {
  int ch[N][26], fail[N], ed[N], cnt[N], inq[N], q[N];
  int tot, hd, tl;
  void init() {  // reset before each test case
    tot = 0;
    memset(ch[0], 0, sizeof ch[0]);
    ed[0] = 0;
  }
  void insert(const string& s) {
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
  // BFS fail pointers; missing children are patched to fail's child (O(1) match step)
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
  // number of distinct patterns occurring in s (marks ed = -1, rebuild to reuse)
  int query(const string& s) {
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
  // occurrence counting: after this, cnt[u] = occurrences of the pattern ending at u
  void count(const string& t) {
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

/*
 * ============================================================
 * Name: Aho-Corasick automaton (multi-pattern matching, Trie + fail pointers)
 * Complexity: build O(total pattern length); matching O(text length); count topo O(#nodes)
 * Usage: matching statistics of many patterns in a long text, wrapped as
 *        ac_automaton<N>, two modes (ed/cnt separated):
 *        1) query: number of distinct patterns hit by the text (deduplicated —
 *           inserting the same pattern twice still counts 1; after walking the
 *           fail chain, ed is set to -1 to prevent double counting);
 *        2) count: occurrences of each pattern: insert records ed[u] =
 *           terminal multiplicity, matching increments cnt[current node] per
 *           step, then a topological pass over the fail tree accumulates
 *           cnt[fail[u]] += cnt[u]; pattern p's occurrences = cnt[its terminal]
 * Principle: Trie builds the patterns; BFS builds fail (failure pointers,
 *        also patching missing children to fail's corresponding child so each
 *        match step is O(1)); fail forms a tree; counts accumulate bottom-up
 * Notes: lowercase alphabet (resize ch's second dimension and x for others);
 *        max nodes = total pattern length + 1; init() between test cases;
 *        query mutates ed
 * ============================================================
 * Example (uncomment to compile; count occurrences of each pattern):
 * static ac_automaton<1000005> ac;
 * signed main() {
 *   int n;
 *   cin >> n;
 *   ac.init();
 *   vector<int> term(n + 1);
 *   for (int i = 1; i <= n; ++i) {
 *     string s;
 *     cin >> s;
 *     ac.insert(s);
 *     int u = 0;
 *     for (char c : s) u = ac.ch[u][c - 'a'];
 *     term[i] = u;
 *   }
 *   ac.build();
 *   string t;
 *   cin >> t;
 *   ac.count(t);
 *   for (int i = 1; i <= n; ++i) cout << ac.cnt[term[i]] << '\n';
 * }
 * ============================================================
 */
