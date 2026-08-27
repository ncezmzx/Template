#include <bits/stdc++.h>
using namespace std;

// 回文树 / 回文自动机（PAM）：每个节点 = 一个本质不同回文子串
// 串存在 s_[1..n_]（s_[0] 为哨兵）；本质不同回文总数不超过 n
constexpr int N = 1e6 + 9;
int tot, last_;
int ch[N][26], fail_[N], len_[N], cnt_[N];
char s_[N];

void pam_init() {
  tot = 1, last_ = 0, s_[0] = '#';
  len_[0] = 0, len_[1] = -1;  // 0 偶根（空串），1 奇根（虚拟 len -1）
  fail_[0] = 1, fail_[1] = 0;
  memset(ch[0], 0, sizeof ch[0]), memset(ch[1], 0, sizeof ch[1]);
}
int get_fail(int p, int i) {  // 沿 fail 找 s[i] 能向两侧扩展的最长回文后缀
  while (s_[i - len_[p] - 1] != s_[i]) p = fail_[p];
  return p;
}
void extend(int i) {  // 处理 s_[i]（1-indexed），返回是否产生新回文
  int c = s_[i] - 'a', p = get_fail(last_, i);
  if (!ch[p][c]) {
    int now = ++tot;
    len_[now] = len_[p] + 2;
    fail_[now] = ch[get_fail(fail_[p], i)][c];  // 先算 fail 再挂载（不会指向自身）
    memset(ch[now], 0, sizeof ch[now]);
    cnt_[now] = 0;
    ch[p][c] = now;
  }
  last_ = ch[p][c];
  cnt_[last_]++;
}
void build(const char* s) {  // s 为 1-indexed C 串（s[0] 任意非字母占位）
  pam_init();
  for (int i = 1; s[i]; ++i) s_[i] = s[i], extend(i);  // 边拷贝边扩展
}
void tally() {  // fail 树子树求和：cnt_[i] = 节点 i 代表的回文的出现次数
  for (int i = tot; i >= 2; --i) cnt_[fail_[i]] += cnt_[i];
}

/*
 * ============================================================
 * 名称：回文树 / 回文自动机（PAM）
 * 复杂度：build O(n)（字符集均摊）；tally O(n)；空间 O(n·26)
 * 用途：在线维护本质不同回文子串（总数 = tot-1）、每个回文的
 *       出现次数（tally 后的 cnt_）、以每个位置结尾的最长回文
 *       后缀（extend 后的 last_ 链）；比 Manacher 更结构化
 * 接口：build(s) 建树（s[0] 为非字母哨兵，如 '#' + 串体）；
 *       tot - 1 = 本质不同回文数；tally() 后 cnt_[u] 为 u 代表
 *       回文的出现次数；len_[u] 为其长度
 * 原理：两根（偶根空串 / 奇根虚拟 len -1）承接所有回文奇偶长度；
 *       每个新位置沿 last 的 fail 链找可扩展的最长回文后缀，
 *       新回文的 fail 再由父的 fail 链求得；fail 树上子树和
 *       即每个回文的出现次数
 * 注意：本质不同回文个数 <= n（下界证明著名）；多次使用需
 *       build 重置；cnt_ 需 tally 后才是完整出现次数
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   build("#abba");                       // 串 "abba"（s_[0] = '#' 哨兵）
 *   cout << tot - 1 << '\n';              // 4（a, b, bb, abba）
 *   tally();
 *   // 节点 2..tot：遍历输出每个回文长度与出现次数
 *   for (int u = 2; u <= tot; ++u) cout << len_[u] << ':' << cnt_[u] << ' ';
 *   cout << '\n';                         // 1:2 1:2 2:1 4:1（a×2, b×2, bb, abba）
 * }
 */
