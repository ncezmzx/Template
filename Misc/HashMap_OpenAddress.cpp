#include <bits/stdc++.h>
using namespace std;
using u64 = unsigned long long;
using i64 = long long;

// 主模板：仅声明，按需对键类型提供特化（见下方 i64 / u64 / string）
template <typename T> u64 custom_hash(const T &x);

constexpr u64 HM0 = 0x9e3779b97f4a3c15ull, HM1 = 0xc2b2ae3d27d4eb4full;
template <> u64 custom_hash<i64>(const i64 &x) {
  return static_cast<u64>(x + HM0) * HM1;
}
template <> u64 custom_hash<u64>(const u64 &x) {
  return (x ^ (x >> 33)) * HM1;
}
template <> u64 custom_hash<string>(const string &s) {
  u64 h = 0x9e3779b97f4a3c15ull;
  for (char c : s) h = (h * 1000003ull) ^ static_cast<u64>(c);
  return h;
}

// 开放寻址线性探测哈希表（高 B 位起始槽，K 需有 custom_hash 特化）
// erase 采用 backward-shift deletion：把同簇后继前移，保持探测链完整
template <typename K, typename V, int B = 20>
class HashMap {
  static constexpr int N = 1 << B;
  K *key{new K[N]};
  V *val{new V[N]};
  bool *vist{new bool[N]};
  int sz = 0;

  int idx(const K &x) const {
    int i = custom_hash(x) >> (64 - B);
    while (vist[i] && key[i] != x) {
      i = (i + 1) & (N - 1);
    }
    return i;
  }

 public:
  HashMap() { memset(vist, 0, N * sizeof(bool)); }
  ~HashMap() {
    delete[] key;
    delete[] val;
    delete[] vist;
  }
  HashMap(const HashMap &) = delete;             // 三块裸数组，禁止拷贝
  HashMap &operator=(const HashMap &) = delete;
  int size() const { return sz; }
  void clear() { memset(vist, 0, N * sizeof(bool)), sz = 0; }
  bool contains(const K &x) const { return vist[idx(x)]; }
  V &operator[](const K &x) {
    int i = idx(x);
    if (!vist[i]) {
      vist[i] = true;
      key[i] = x;
      val[i] = V{};
      ++sz;
    }
    return val[i];
  }
  void erase(const K &x) {
    int i = idx(x);
    if (!vist[i]) return;
    vist[i] = false, --sz;
    for (int j = i;;) {   // backward-shift：后继中"不必经过洞 i"的键逐个前移
      j = (j + 1) & (N - 1);
      if (!vist[j]) break;
      int h = int(custom_hash(key[j]) >> (64 - B));
      bool must_pass = (i < j) ? (h > i && h <= j) : (h > i || h <= j);  // h ∈ 环形 (i, j]
      if (!must_pass) {
        key[i] = key[j], val[i] = val[j], vist[i] = true, vist[j] = false;
        i = j;
      }
    }
  }
};

/*
 * ============================================================
 * 名称：开放寻址哈希表（线性探测 + backward-shift 删除）
 * 复杂度：期望 O(1) 查找/插入/删除（负载因子建议 < 0.7，B 按数据量调整）
 * 用途：operator[] / contains / erase / clear / size；
 *       键类型需提供 custom_hash 特化（自带 i64 / u64 / string）
 * 来源：用户提供代码；已修两处问题：
 *       1) 补缺失的主模板声明与 M0/M1 常数（原稿仅有 hash<i64> 特化，
 *          且 M0/M1 未定义，无法编译）；
 *       2) 原稿 erase 直接 vist=false 会截断线性探测链，导致后续
 *          冲突键 contains/[] 假阴性 —— 改为 backward-shift deletion
 * 对比：与 HashTable_Chain（链式 + 槽位池）相比，本表常数更小、缓存更友好
 *       （见 README 基准）；链式版支持计数语义（ins 正负抵消回收），按需选用
 * 注意：B=20 即 2^20 槽（约 12MB/对象）；元素数超过 0.7·2^B 后性能退化
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   HashMap<long long, int> mp;   // B=20；小数据量可 HashMap<long long, int, 17>
 *   mp[3] = 30, mp[1000000007] = 7;
 *   mp.erase(3);
 *   cout << mp.contains(3) << ' ' << mp[1000000007] << ' ' << mp.size() << '\n';
 *   HashMap<string, int> cnt;
 *   cnt["abc"] += 1, cnt["abd"] += 2;
 *   cout << cnt["abc"] << ' ' << cnt.size() << '\n';
 * }
 * ============================================================
 */
