#include <bits/stdc++.h>
using namespace std;
using u64 = unsigned long long;
using i64 = long long;

// primary template: declaration only; specialize custom_hash per key type (i64 / u64 / string below)
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

// open-addressing linear-probing hash table (high-B-bit home slot; K needs a custom_hash specialization)
// erase uses backward-shift deletion: successors in the cluster move back, probe chains stay intact
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
  HashMap(const HashMap &) = delete;             // raw arrays, non-copyable
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
    for (int j = i;;) {   // backward-shift: move successors that need not pass hole i
      j = (j + 1) & (N - 1);
      if (!vist[j]) break;
      int h = int(custom_hash(key[j]) >> (64 - B));
      bool must_pass = (i < j) ? (h > i && h <= j) : (h > i || h <= j);  // h in cyclic (i, j]
      if (!must_pass) {
        key[i] = key[j], val[i] = val[j], vist[i] = true, vist[j] = false;
        i = j;
      }
    }
  }
};

/*
 * ============================================================
 * Name: open-addressing hash table (linear probing + backward-shift deletion)
 * Complexity: expected O(1) lookup / insert / delete (keep the load factor <
 *             0.7, and tune B per data volume)
 * Usage: operator[] / contains / erase / clear / size; key types need a
 *        custom_hash specialization (i64 / u64 / string are provided).
 * Source: user-provided code; two issues fixed:
 *        1) added the missing primary template declaration and M0/M1
 *           constants (the original only had the hash<i64> specialization
 *           and left M0/M1 undefined — it did not compile);
 *        2) the original erase set vist=false directly, truncating probe
 *           chains and causing false negatives in later contains/[] for
 *           colliding keys — replaced with backward-shift deletion
 * Comparison: against HashTable_Chain (chaining + slot pool) this table has
 *        smaller constants and is more cache-friendly (see README
 *        benchmarks); the chained version supports counting semantics (ins
 *        cancels +/- and recycles). Pick as needed
 * Notes: B = 20 means 2^20 slots (~12MB per object); performance degrades once
 *        the element count exceeds 0.7 * 2^B
 * ============================================================
 */
