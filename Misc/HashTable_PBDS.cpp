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
 * Name: custom hash table — __gnu_pbds::gp_hash_table + splitmix64 (anti-hack)
 * Complexity: expected O(1) amortized per insert/lookup/delete
 * Usage: O(1) hash tables when hack-attacks on hashes are a concern;
 *        gp_hash_table is open-addressing with much smaller constants than
 *        unordered_map; splitmix64 with a random FIXED_RANDOM seed defeats
 *        targeted constructions.
 *        Note: needs the pb_ds headers; prefer the two dedicated headers over
 *        <bits/extc++.h> (some MinGW builds miss iconv.h).
 * Notes: HashTable_PBDS.cpp and HashTable_Chain.cpp are two implementations
 *        of "hash tables": the former uses __gnu_pbds::gp_hash_table +
 *        splitmix64 (anti-hack), the latter a hand-written chained
 *        reference-counting table (slot-pool recycling). Pick as needed.
 * Source: all.cpp lines 41931-41953 (kept verbatim, comments translated)
 * ============================================================
 * Example (uncomment to compile):
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
