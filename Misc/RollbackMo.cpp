#include <bits/stdc++.h>
using namespace std;

// 回滚莫队（不删除莫队）：区间相同数最远距离（max_{i<j, a[i]==a[j]} j-i）
// 只支持"加入"、不支持"删除"的区间统计（区间众数/最大距离/历史最值等）
constexpr int N = 2e5 + 9;
int a[N], first[N], last[N];        // 当前窗口内每个值首次/末次出现位置
int sv[N], sf[N], stk_top;          // 左侧扩展的回滚栈（值 / 旧 first）
int ts[N], tmpfirst[N], timer;      // 块内暴力的时间戳数组
int cur;

inline void radd(int i) {           // 右侧加入（i 递增，永久）
  int v = a[i];
  if (first[v] == 0) first[v] = last[v] = i;
  else last[v] = i, cur = max(cur, last[v] - first[v]);
}
inline void ladd(int i) {           // 左侧加入（i 递减，临时，可回滚）
  int v = a[i];
  sv[++stk_top] = v, sf[stk_top] = first[v];
  if (first[v] == 0) first[v] = last[v] = i;
  else first[v] = i, cur = max(cur, last[v] - first[v]);
}
inline void lrollback() {           // 逆序恢复左侧加入
  for (; stk_top; --stk_top) first[sv[stk_top]] = sf[stk_top];
}

struct Qry {
  int l, r, id;
};
// val[1..n]（val[0] 占位）；qs = {{l, r}, ...}（1-indexed）
vector<int> rollback_mo(int n, const vector<int>& val, const vector<array<int, 2>>& qs) {
  // 离散化到 [1, V]
  vector<int> b(val.begin() + 1, val.end());
  sort(b.begin(), b.end()), b.erase(unique(b.begin(), b.end()), b.end());
  for (int i = 1; i <= n; ++i)
    a[i] = (int)(lower_bound(b.begin(), b.end(), val[i]) - b.begin()) + 1;
  int V = b.size(), q = qs.size();
  int B = max(1, (int)(n / max(1.0, sqrt((double)q))));  // 块长 n/√q
  vector<Qry> Q(q);
  for (int i = 0; i < q; ++i) Q[i] = {qs[i][0], qs[i][1], i};
  sort(Q.begin(), Q.end(), [&](const Qry& x, const Qry& y) {
    int bx = x.l / B, by = y.l / B;
    if (bx != by) return bx < by;
    return x.r < y.r;  // 同块内 r 只增不减（不可用奇偶优化）
  });
  vector<int> ans(q);
  for (int i = 0; i < q;) {
    int b = Q[i].l / B;
    int R = min(n, (b + 1) * B);  // 块右端点
    for (int v = 1; v <= V; ++v) first[v] = last[v] = 0;
    cur = 0;
    int r = R;
    for (; i < q && Q[i].l / B == b; ++i) {
      if (Q[i].r <= R) {  // 询问完全落在块内：直接暴力
        int best = 0, t = ++timer;
        for (int p = Q[i].l; p <= Q[i].r; ++p) {
          int v = a[p];
          if (ts[v] != t) ts[v] = t, tmpfirst[v] = p;
          else best = max(best, p - tmpfirst[v]);
        }
        ans[Q[i].id] = best;
      } else {
        while (r < Q[i].r) radd(++r);
        int saved = cur;
        stk_top = 0;
        int l = R + 1;
        while (l > Q[i].l) ladd(--l);
        ans[Q[i].id] = cur;
        lrollback();
        cur = saved;
      }
    }
  }
  return ans;
}

/*
 * ============================================================
 * 名称：回滚莫队（不删除莫队）
 * 复杂度：O(n√q + q·√n)（块长 n/√q）
 * 用途：统计量"能 O(1) 加入、难以或不能 O(1) 删除"的离线区间询问：
 *       示例为区间相同数最远距离（洛谷 P5906），同法可做区间众数
 *       出现次数等
 * 接口：rollback_mo(n, val[1..n], {{l, r}, ...}) → 每问最远同值距离
 * 原理：按 l 所在块分组；块内右指针只右移（加入永久保留），每个询问
 *       左指针从块右端向左临时扩展、答完逆序回滚——避免"删除"操作
 * 注意：同块内 r 单调增，不可加奇偶排序优化；值自动离散化；空窗口
 *       返回 0；左侧加入只改 first[]（逆序回滚才正确）
 * 来源：OI-Wiki《回滚莫队》（https://oi-wiki.org/misc/rollback-mo/）
 * ============================================================
 * 使用示例（编译时取消注释；洛谷 P5906）：
 * signed main() {
 *   vector<int> val{0, 1, 2, 1, 3, 2};  // n = 5: {1,2,1,3,2}
 *   auto r = rollback_mo(5, val, {{1, 3}, {2, 5}, {1, 5}});
 *   for (int x : r) cout << x << ' ';  // 2（1 相距 2） 3（2 相距 3） 3（2 相距 3）
 *   cout << '\n';
 * }
 * ============================================================
 */
