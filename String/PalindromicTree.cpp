#include <bits/stdc++.h>
using namespace std;

// Palindromic tree (Eertree / PAM): one node per distinct palindromic substring
template <size_t N>
struct palindromic_tree {
  int tot, last_;
  int ch[N][26], fail_[N], len_[N], cnt_[N];
  char s_[N];  // s_[1..n_], s_[0] = sentinel
  void init() {
    tot = 1, last_ = 0, s_[0] = '#';
    len_[0] = 0, len_[1] = -1;  // node 0 = even root (empty), node 1 = odd root (len -1)
    fail_[0] = 1, fail_[1] = 0;
    memset(ch[0], 0, sizeof ch[0]), memset(ch[1], 0, sizeof ch[1]);
  }
  int get_fail(int p, int i) {  // longest palindromic suffix extendable by s_[i]
    while (s_[i - len_[p] - 1] != s_[i]) p = fail_[p];
    return p;
  }
  void extend(int i) {  // append s_[i] (1-indexed)
    int c = s_[i] - 'a', p = get_fail(last_, i);
    if (!ch[p][c]) {
      int now = ++tot;
      len_[now] = len_[p] + 2;
      fail_[now] = ch[get_fail(fail_[p], i)][c];  // fail computed before linking
      memset(ch[now], 0, sizeof ch[now]);
      cnt_[now] = 0;
      ch[p][c] = now;
    }
    last_ = ch[p][c];
    cnt_[last_]++;
  }
  void build(const char* s) {  // s is 1-indexed (s[0] = non-letter sentinel)
    init();
    for (int i = 1; s[i]; ++i) s_[i] = s[i], extend(i);
  }
  void tally() {  // fail-tree suffix sums: cnt_[u] = occurrences of node u's palindrome
    for (int i = tot; i >= 2; --i) cnt_[fail_[i]] += cnt_[i];
  }
};

/*
 * ============================================================
 * Name: palindromic tree / eertree (PAM)
 * Complexity: build O(n) (alphabet amortized); tally O(n); space O(n*26)
 * Usage: online maintenance of distinct palindromic substrings (total =
 *        tot-1), occurrence counts per palindrome (cnt_ after tally), and
 *        the longest palindromic suffix ending at each position (last_ chain
 *        after extend); more structured than Manacher. Wrapped as
 *        palindromic_tree<N>
 * Interface: build(s) (s[0] is a non-letter sentinel, e.g. '#' + the string);
 *        tot - 1 = number of distinct palindromes; after tally(), cnt_[u] is
 *        the occurrence count of u's palindrome; len_[u] its length
 * Principle: two roots (even root = empty string, odd root = virtual len -1)
 *        absorb both parities; each new position walks last's fail chain to
 *        the longest extendable palindromic suffix; the new palindrome's fail
 *        comes from the parent's fail chain; subtree sums on the fail tree
 *        give occurrence counts
 * Notes: at most n distinct palindromes (classic bound); rebuild between
 *        uses; cnt_ is only complete after tally()
 * ============================================================
 * Example (uncomment to compile):
 * static palindromic_tree<1000005> pam;
 * signed main() {
 *   pam.build("#abba");                   // the string "abba" (s_[0] = '#' sentinel)
 *   cout << pam.tot - 1 << '\n';          // 4（a, b, bb, abba）
 *   pam.tally();
 *   // nodes 2..tot: print each palindrome's length and count
 *   for (int u = 2; u <= pam.tot; ++u) cout << pam.len_[u] << ':' << pam.cnt_[u] << ' ';
 *   cout << '\n';                         // 1:2 1:2 2:1 4:1（a×2, b×2, bb, abba）
 * }
 */
