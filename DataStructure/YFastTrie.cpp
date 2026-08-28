#include <bits/stdc++.h>
using namespace std;

struct yfast {
  using u64 = unsigned long long;
  static constexpr int W = 64, B = 64;

  unordered_map<u64, int> ch[W + 1];
  vector<int> lc, rc;
  vector<u64> mn, mx;
  int totn;

  unordered_map<u64, u64> prv, nxt;
  u64 head, tail;

  struct block {
    vector<u64> a;
  };
  vector<block> blk;
  vector<int> free_id;
  unordered_map<u64, int> min2id;

  yfast() : totn(0), head(0), tail(0) {
    new_node();
    ch[0][0] = 0;
  }

  int new_node() {
    lc.push_back(0), rc.push_back(0), mn.push_back(0), mx.push_back(0);
    return totn++;
  }
  static u64 pref(u64 x, int k) { return k ? x >> (W - k) : 0; }
  static int bit(u64 x, int d) { return (x >> (W - 1 - d)) & 1; }

  void trie_insert(u64 v) {
    int u = 0;
    mn[u] = mn[u] ? min(mn[u], v) : v;
    mx[u] = max(mx[u], v);
    for (int k = 1; k <= W; ++k) {
      u64 p = pref(v, k);
      auto it = ch[k].find(p);
      if (it == ch[k].end()) {
        int id = new_node();
        ch[k][p] = id;
        mn[id] = mx[id] = v;
        if (bit(v, k - 1)) rc[u] = id;
        else lc[u] = id;
        u = id;
      } else {
        u = it->second;
        mn[u] = min(mn[u], v);
        mx[u] = max(mx[u], v);
      }
    }
  }

  void trie_delete(u64 v) {
    int path[W + 1];
    path[0] = 0;
    for (int k = 1; k <= W; ++k) path[k] = ch[k][pref(v, k)];
    ch[W].erase(v);
    for (int k = W; k >= 1; --k) {
      int id = path[k];
      if (lc[id] || rc[id]) break;
      ch[k].erase(pref(v, k));
      if (k == 1) {
        if (bit(v, 0)) rc[0] = 0;
        else lc[0] = 0;
      } else {
        int par = path[k - 1];
        if (bit(v, k - 1)) rc[par] = 0;
        else lc[par] = 0;
      }
    }
    for (int k = W; k >= 1; --k) {
      if (!ch[k].count(pref(v, k))) continue;
      int id = path[k];
      mn[id] = mx[id] = 0;
      if (lc[id]) mn[id] = mn[lc[id]], mx[id] = mx[lc[id]];
      if (rc[id]) {
        mn[id] = mn[id] ? min(mn[id], mn[rc[id]]) : mn[rc[id]];
        mx[id] = max(mx[id], mx[rc[id]]);
      }
    }
    mn[0] = mx[0] = 0;
    if (lc[0]) mn[0] = mn[lc[0]], mx[0] = mx[lc[0]];
    if (rc[0]) {
      mn[0] = mn[0] ? min(mn[0], mn[rc[0]]) : mn[rc[0]];
      mx[0] = max(mx[0], mx[rc[0]]);
    }
  }

  int longest_prefix(u64 x) const {
    int lo = 0, hi = W;
    while (lo < hi) {
      int mid = (lo + hi + 1) >> 1;
      if (ch[mid].count(pref(x, mid))) lo = mid;
      else hi = mid - 1;
    }
    return lo;
  }

  pair<u64, u64> pred_succ(u64 x) const {
    if (ch[W].count(x)) return {prv.count(x) ? prv.at(x) : 0, nxt.count(x) ? nxt.at(x) : 0};
    int d = longest_prefix(x);
    int u = ch[d].at(pref(x, d));
    if (bit(x, d)) return {mx[u], nxt.count(mx[u]) ? nxt.at(mx[u]) : 0};
    return {prv.count(mn[u]) ? prv.at(mn[u]) : 0, mn[u]};
  }

  void rep_link(u64 v) {
    auto [p, s] = pred_succ(v);
    prv[v] = p, nxt[v] = s;
    if (p) nxt[p] = v;
    else head = v;
    if (s) prv[s] = v;
    else tail = v;
  }
  void rep_unlink(u64 v) {
    u64 p = prv[v], s = nxt[v];
    if (p) nxt[p] = s;
    else head = s;
    if (s) prv[s] = p;
    else tail = p;
    prv.erase(v), nxt.erase(v);
  }

  int new_block(u64 minv) {
    int id;
    if (!free_id.empty()) {
      id = free_id.back();
      free_id.pop_back();
      blk[id].a.clear();
    } else {
      id = (int)blk.size();
      blk.emplace_back();
    }
    rep_link(minv);      // link predecessor/successor first (minv is not in the trie yet)
    trie_insert(minv);
    min2id[minv] = id;
    return id;
  }
  void del_block(int id, u64 minv) {
    min2id.erase(minv);
    trie_delete(minv);
    rep_unlink(minv);
    free_id.push_back(id);
  }
  void change_rep(int id, u64 old_min, u64 new_min) {
    trie_delete(old_min);
    rep_unlink(old_min);
    min2id.erase(old_min);
    rep_link(new_min);   // link before inserting (with new_min absent, pred_succ takes the longest-prefix branch)
    trie_insert(new_min);
    min2id[new_min] = id;
  }
  int block_of(u64 minv) const { return min2id.at(minv); }
  int tail_id() const { return tail ? block_of(tail) : -1; }

  int locate(u64 x, int& nid) const {
    if (!head) return nid = -1, -1;
    if (x < head) return nid = block_of(head), -1;
    auto [p, s] = pred_succ(x);
    u64 r = ch[W].count(x) ? x : p;
    if (!r) return nid = -1, -1;
    int id = block_of(r);
    nid = s ? block_of(s) : -1;
    return id;
  }

  int find_block(u64 x) const {
    int nid;
    int id = locate(x, nid);
    if (id >= 0 && binary_search(blk[id].a.begin(), blk[id].a.end(), x)) return id;
    if (nid >= 0 && binary_search(blk[nid].a.begin(), blk[nid].a.end(), x)) return nid;
    return -1;
  }

  void maybe_split(int id) {
    if ((int)blk[id].a.size() <= 2 * B) return;
    int mid = (int)blk[id].a.size() / 2;
    u64 new_min = blk[id].a[mid];
    int nid = new_block(new_min);
    blk[nid].a.assign(blk[id].a.begin() + mid, blk[id].a.end());
    blk[id].a.resize(mid);
  }

  void insert(u64 x) {
    if (find_block(x) >= 0) return;
    if (!head) {
      int id = new_block(x);
      blk[id].a.push_back(x);
      return;
    }
    int nid;
    int id = locate(x, nid);
    if (id < 0) id = nid;
    auto& a = blk[id].a;
    u64 old_min = a.front();
    a.insert(lower_bound(a.begin(), a.end(), x), x);
    if (x < old_min) change_rep(id, old_min, x);
    maybe_split(id);
  }

  void erase(u64 x) {
    int id = find_block(x);
    if (id < 0) return;
    auto& a = blk[id].a;
    auto it = lower_bound(a.begin(), a.end(), x);
    u64 old_min = a.front();
    bool was_min = it == a.begin();
    a.erase(it);
    if (a.empty()) return del_block(id, old_min), void();
    if (was_min) change_rep(id, old_min, a.front());
    if ((int)a.size() < B / 2) {
      u64 cur = a.front();
      u64 nb = nxt.count(cur) ? nxt.at(cur) : 0;
      if (nb) {
        int oid = block_of(nb);
        u64 oid_min = blk[oid].a.front();
        a.insert(a.end(), blk[oid].a.begin(), blk[oid].a.end());
        del_block(oid, oid_min);
        maybe_split(id);
      } else {
        u64 pp = prv.count(cur) ? prv.at(cur) : 0;
        if (pp) {
          int oid = block_of(pp);
          blk[oid].a.insert(blk[oid].a.end(), a.begin(), a.end());
          del_block(id, cur);
          maybe_split(oid);
        }
      }
    }
  }

  bool find(u64 x) const { return find_block(x) >= 0; }
  u64 minimum() const { return head; }
  u64 maximum() const {
    return tail ? blk[block_of(tail)].a.back() : 0;
  }

  u64 predecessor(u64 x) const {
    if (!head) return 0;
    int nid;
    int id = locate(x, nid);
    if (id < 0) return 0;
    const auto& a = blk[id].a;
    auto it = lower_bound(a.begin(), a.end(), x);
    if (it != a.begin()) return *prev(it);
    u64 p = prv.count(a.front()) ? prv.at(a.front()) : 0;
    return p ? blk[block_of(p)].a.back() : 0;
  }
  u64 successor(u64 x) const {
    if (!head) return 0;
    int nid;
    int id = locate(x, nid);
    if (id < 0) return nid >= 0 ? blk[nid].a.front() : 0;
    const auto& a = blk[id].a;
    auto it = upper_bound(a.begin(), a.end(), x);
    if (it != a.end()) return *it;
    u64 s = nxt.count(a.front()) ? nxt.at(a.front()) : 0;
    return s ? blk[block_of(s)].a.front() : 0;
  }
};

/*
 * ============================================================
 * Name: y-fast trie (x-fast trie + blocking); source: Luogu o99sh6m1 (sketch, self-implemented)
 * Complexity: find/predecessor/successor/max/min O(log omega) (omega = 64);
 *             insert/delete amortized O(log omega)
 * Principle:
 *   1) x-fast trie: a 64-bit prefix trie with one hash table per level
 *      (prefix -> node); nodes store subtree min/max; combined with an
 *      ordered doubly-linked list of representatives, a longest-prefix
 *      binary search + sibling-subtree extrema yields a key whose rank
 *      differs from x's by at most 1 (equivalent to the van Emde Boas-style
 *      iteration, O(log omega) hash lookups)
 *   2) y-fast trie: elements are grouped into blocks (sorted vector + binary
 *      search inside, target block size B = 64); an x-fast trie maintains
 *      the blocks' minima (representatives); a block exceeding 2B splits in
 *      two, a block below B/2 merges with a neighbor (amortized O(1))
 * Usage: dynamic set of 64-bit integers (find/pred/succ/min/max/insert/
 *        delete), smaller constants than std::set (O(log n) and cache-
 *        unfriendly); suited to very heavy workloads (~1e6 operations in OI)
 * Notes: 0 is reserved as "not stored" (predecessor/successor/max/min return
 *        0 when empty); tune the block threshold B per data scale (larger B =
 *        less blocking overhead, slower in-block binary search)
 * ============================================================
 * Example (uncomment to compile):
 * signed main() {
 *   yfast st;
 *   for (yfast::u64 x : {5ull, 3ull, 9ull, 1ull, 7ull}) st.insert(x);
 *   cout << st.minimum() << ' ' << st.maximum() << '\n';       // 1 9
 *   cout << st.find(7) << ' ' << st.find(4) << '\n';           // 1 0
 *   cout << st.predecessor(6) << ' ' << st.successor(6) << '\n';  // 5 7
 *   st.erase(5);
 *   cout << st.predecessor(6) << ' ' << st.successor(4) << '\n';  // 3 7
 *   for (yfast::u64 x : {3ull, 1ull, 9ull, 7ull}) st.erase(x);
 *   cout << st.minimum() << '\n';                              // 0 (empty)
 * }
 * ============================================================
 */
