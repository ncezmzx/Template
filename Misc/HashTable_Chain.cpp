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
 * Name: random-weight xor hashing (multiset hashing) + reference-counting hash table
 * Complexity: assigning a random weight to each distinct value O(1); prefix xor
 *             O(1) per step; hash-table insert / lookup expected O(1)
 * Usage: count subarrays where every element appears an even number of times,
 *        and decide whether two multisets are equal: assign each distinct value
 *        a random u64 weight w[x],
 *        so the multiset xor hash of [l, r] is w[a[l]] ^ ... ^ w[a[r]] and all
 *        counts are even iff that xor is 0 (random weights avoid collisions);
 *        tree isomorphism / subtree hashing work the same way, merging random
 *        weights upward.
 *        Also provides a hand-written chained reference-counting hash table
 *        (slot-pool recycling).
 * Notes: HashTable_PBDS.cpp and HashTable_Chain.cpp are two implementations of
 *        "hash tables": the former uses __gnu_pbds::gp_hash_table + splitmix64
 *        (anti-hack), the latter this hand-written chained table. Pick as
 *        needed
 * Source: all.cpp lines 37044, 37048, 37050-37078, 37101 (kept verbatim, comments translated)
 * ============================================================
 */
