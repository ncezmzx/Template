#include <bits/stdc++.h>
using namespace std;
#define int long long

constexpr int N = 1e6 + 9;
int ch[N][26], fail[N], ed[N], cnt[N], tot;

void ac_insert(const string& s) {
  int u = 0;
  for (char c : s) {
    int x = c - 'a';
    if (!ch[u][x]) ch[u][x] = ++tot;
    u = ch[u][x];
  }
  ++ed[u];
}

void ac_build() {
  queue<int> q;
  for (int i = 0; i < 26; ++i)
    if (ch[0][i]) q.push(ch[0][i]);
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    for (int i = 0; i < 26; ++i) {
      if (ch[u][i]) fail[ch[u][i]] = ch[fail[u]][i], q.push(ch[u][i]);
      else ch[u][i] = ch[fail[u]][i];
    }
  }
}

int ac_query(const string& s) {
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

int inq[N], q[N], hd, tl;
void ac_count(const string& t) {
  int u = 0;
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

/*
 * ============================================================
 * 名称：AC 自动机（多模式串匹配，Trie + fail 指针）
 * 复杂度：构建 O(总模式串长度)；匹配 O(文本长度)；ac_count 拓扑 O(节点数)
 * 用途：多模式串在长文本中的匹配统计，两种用法（ed/cnt 职责分离）：
 *       1) ac_query：统计文本命中的**模式串种类数**（去重，重复插入同串
 *          仍计 1；沿 fail 链累加后把 ed 置 -1 防重复）
 *       2) ac_count：统计**每个模式串的出现次数**：插入时 ed[u] 记终点多重度，
 *          匹配时每步 cnt[当前节点]++（出现位置计数），拓扑沿 fail 树累加
 *          cnt[fail[u]] += cnt[u]；最后模式串 p 的出现次数 = cnt[其终点节点]
 * 原理：Trie 建模式串；BFS 建 fail（失配指针，同时把空儿子补成 fail 的对应
 *       儿子，匹配每步 O(1)）；fail 构成 fail 树，出现次数自底向上累加
 * 注意：字符集为小写字母（改 ch 第二维与 x 取值适配）；节点数上限 = 模式串
 *       总长 + 1；多组数据清空 ch/fail/ed/cnt/tot/inq；ac_query 会修改 ed
 * ============================================================
 * 使用示例（编译时取消注释；统计每个模式串出现次数）：
 * signed main() {
 *   int n;
 *   cin >> n;
 *   vector<int> term(n + 1);
 *   for (int i = 1; i <= n; ++i) {
 *     string s;
 *     cin >> s;
 *     ac_insert(s);
 *     int u = 0;
 *     for (char c : s) u = ch[u][c - 'a'];
 *     term[i] = u;
 *   }
 *   ac_build();
 *   string t;
 *   cin >> t;
 *   ac_count(t);
 *   for (int i = 1; i <= n; ++i) cout << cnt[term[i]] << '\n';
 * }
 * ============================================================
 */
