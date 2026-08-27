#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int N = 2e5 + 9;
int ch[N][26], lnk[N], len[N], sz[N], tot, lst;

void sam_init() {
  tot = lst = 1;
  memset(ch, 0, sizeof ch), memset(lnk, 0, sizeof lnk);
  memset(len, 0, sizeof len), memset(sz, 0, sizeof sz);
}

void sam_extend(int c) {
  int cur = ++tot, p = lst;
  len[cur] = len[lst] + 1, sz[cur] = 1;
  for (; p && !ch[p][c]; p = lnk[p]) ch[p][c] = cur;
  if (!p) lnk[cur] = 1;
  else {
    int q = ch[p][c];
    if (len[q] == len[p] + 1) lnk[cur] = q;
    else {
      int cp = ++tot;
      memcpy(ch[cp], ch[q], sizeof ch[q]);
      len[cp] = len[p] + 1, lnk[cp] = lnk[q];
      for (; p && ch[p][c] == q; p = lnk[p]) ch[p][c] = cp;
      lnk[q] = lnk[cur] = cp;
    }
  }
  lst = cur;
}

int buc[N], o[N];
void sam_count() {
  memset(buc, 0, sizeof buc);
  for (int i = 1; i <= tot; ++i) ++buc[len[i]];
  for (int i = 1; i <= tot; ++i) buc[i] += buc[i - 1];
  for (int i = 1; i <= tot; ++i) o[buc[len[i]]--] = i;
  for (int i = tot; i >= 1; --i) sz[lnk[o[i]]] += sz[o[i]];
}

/*
 * ============================================================
 * 名称：后缀自动机（SAM，Suffix Automaton）
 * 复杂度：构建 O(n * |Σ|)（转移表 ch[N][26]），节点数 ≤ 2n，边数 ≤ 3n
 * 用途：一个串的所有子串信息；经典结论：
 *       1) 本质不同子串数 = sum_{i=2..tot} (len[i] - len[lnk[i]])
 *       2) sam_count() 后 sz[i] = 状态 i 代表的 endpos 集合大小，
 *          即该状态对应子串在原串中的出现次数（注意根状态 1 无意义）
 *       3) 最小表示、最长公共子串、字典序第 k 小子串等均可在此骨架上扩展
 * 原理：在线构造；每个状态代表若干 endpos 等价类，link 指向
 *       len 严格更小的最长后缀状态（后缀链接树）
 * 注意：字符集为小写字母（26）；其他字符集改 ch 的第二维与 c 的取值；
 *       N 取 2 * |S| + 5 以上；多组数据先 sam_init()
 * 用法：sam_init(); for (char c : s) sam_extend(c - 'a'); sam_count();
 * ============================================================
 * 使用示例（编译时取消注释；统计每个状态出现次数与本质不同子串数）：
 * signed main() {
 *   string s;
 *   cin >> s;
 *   sam_init();
 *   for (char c : s) sam_extend(c - 'a');
 *   sam_count();
 *   long long cnt = 0;
 *   for (int i = 2; i <= tot; ++i) cnt += len[i] - len[lnk[i]];
 *   cout << cnt << '\n';
 * }
 * ============================================================
 */
