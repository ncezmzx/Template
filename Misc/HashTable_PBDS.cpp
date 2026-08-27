#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/hash_policy.hpp>
using namespace std;

struct splitmix64_hash {
  static uint64_t splitmix64(uint64_t x) {
    x += 0x9e3779b97f4a7c15;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
  }

  size_t operator()(uint64_t x) const {
    static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
    return splitmix64(x + FIXED_RANDOM);
  }
};

template <typename K, typename V, typename Hash = splitmix64_hash>
using HashMap = __gnu_pbds::gp_hash_table<K, V, Hash>;

static uint64_t splitmix64(uint64_t x) {
  x += 0x9e3779b97f4a7c15;
  x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
  x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
  return x ^ (x >> 31);
}
/*
 * ============================================================
 * 名称：自定义哈希表 —— __gnu_pbds::gp_hash_table + splitmix64（抗卡哈希）
 * 复杂度：期望 O(1) 均摊 单次插入/查询/删除
 * 用途：需要 O(1) 哈希表且担心被卡哈希时；gp_hash_table 为开放寻址实现，
 *       常数远小于 unordered_map；配合 splitmix64 的随机 FIXED_RANDOM 种子防针对性构造。
 *       注意：需要 pb_ds 头，建议用两个专门头而非 <bits/extc++.h>（部分 MinGW 缺 iconv.h）。
 * 说明：HashTable_PBDS.cpp 与 HashTable_Chain.cpp 是"哈希表"这一算法的两种
 *       实现：前者用 __gnu_pbds::gp_hash_table + splitmix64（抗卡），后者是
 *       手写链式引用计数哈希表（槽位池回收）。按需选用。
 * 来源：all.cpp 行 41931-41953（原样保留；注释已统一移至文件尾部）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * int main() {
 *   HashMap<int, int> mp;
 *   mp[1] = 2, mp[100] = 3;
 *   cout << mp[1] + mp[100] << '\n';
 *   cout << (mp.find(50) == mp.end()) << '\n';
 *   uint64_t a = 123, b = 456;
 *   cout << splitmix64(splitmix64(a) + b) << '\n';
 * }
 * ============================================================
 */
