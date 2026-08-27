#include <bits/stdc++.h>
using namespace std;
using u64 = unsigned long long;

mt19937_64 rng(random_device{}());

template<class T, size_t N, size_t Mod = 3217>
struct Hash {
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
        return ;
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
/*
 * ============================================================
 * 名称：随机权值异或哈希（多重集合哈希） + 引用计数哈希表
 * 复杂度：给每个不同值分配随机权值 O(1)；前缀异或 O(1)/步；Hash 表插入/查询期望 O(1)
 * 用途：统计"每个元素出现次数均为偶数"的子数组个数 / 判断两个多重集合是否相等：
 *       给每个不同值一个随机 u64 权值 w[x]，区间 [l,r] 的多重集 xor 哈希 =
 *       w[a[l]]^...^w[a[r]]；全部元素出现偶数次 ⇔ 该异或为 0（随机权值避免撞车）；
 *       树同构 / 子树哈希同理（给每个子树随机权值向上合并）。
 * 说明：HashTable_PBDS.cpp 与 HashTable_Chain.cpp 是"哈希表"这一算法的两种
 *       实现：前者用 __gnu_pbds::gp_hash_table + splitmix64（抗卡），后者是
 *       手写链式引用计数哈希表（槽位池回收）。按需选用。
 * 来源：all.cpp 行 37044、37048、37050-37078、37101（原样保留；注释已统一移至文件尾部）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * int main() {
 *   int n;
 *   cin >> n;
 *   unordered_map<u64, u64> w;
 *   static Hash<u64, 100000> cnt;
 *   cnt.ins(0, 1);
 *   long long ans = 0;
 *   u64 s = 0;
 *   for (int i = 1; i <= n; ++i) {
 *     int x;
 *     cin >> x;
 *     if (!w.count(x)) w[x] = rng();
 *     s ^= w[x];
 *     ans += cnt.query(s);
 *     cnt.ins(s, 1);
 *   }
 *   cout << ans << '\n';
 * }
 * ============================================================
 */
