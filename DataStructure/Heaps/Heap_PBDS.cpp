#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>
using namespace std;

// pairing heap via __gnu_pbds::priority_queue (mergeable)
template <size_t N>
struct pbds_heap {
  typedef __gnu_pbds::priority_queue<int, greater<int>> heap_t;
  heap_t pq[N];
  typename heap_t::point_iterator its[N];
  int newnode(int x, int i) { return its[i] = pq[i].push(x), i; }
  int top(int x) { return pq[x].top(); }
  void join(int x, int y) { pq[x].join(pq[y]); }  // y becomes empty
  void decrease_key(int h, int p, int v) { pq[h].modify(its[p], v); }
  void erase(int h, int x) { pq[h].erase(its[x]); }
};
/*
 * ============================================================
 * Name: pbds pairing heap (__gnu_pbds implementation, mergeable)
 * Complexity: newnode/top/join O(1) amortized; decrease_key/erase O(log n) amortized
 * Usage: newnode/top/join/decrease_key/erase; min-heap (greater<int>),
 *        wrapped as pbds_heap<N>; heaps are identified by their index
 * Source: Luogu article "In Praise of the Priority Queue"
 *         (the Luogu blog article "In Praise of the Priority Queue") section 1, wrapped into a struct
 * Notes: requires pb_ds (#include <ext/pb_ds/priority_queue.hpp>); its[]
 *        holds iterators, invalidated after erase/modify; after join(x, y)
 *        heap y is empty; element type fixed to int
 * ============================================================
 * Example (uncomment to compile):
 * static pbds_heap<1009> hp;
 * signed main() {
 *   int h = hp.newnode(5, 1);
 *   hp.join(h, hp.newnode(3, 2));
 *   hp.join(h, hp.newnode(8, 3));
 *   hp.join(h, hp.newnode(1, 4));
 *   hp.join(h, hp.newnode(7, 5));
 *   cout << hp.top(h) << '\n';      // 1
 *   hp.decrease_key(h, 2, 0);       // 3 -> 0
 *   cout << hp.top(h) << '\n';      // 0
 *   hp.erase(h, 2);                 // remove the 0
 *   cout << hp.top(h) << '\n';      // 1
 * }
 * ============================================================
 */
