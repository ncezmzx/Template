#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>
using namespace std;


template <size_t N> struct pbds_heap {
  typedef __gnu_pbds::priority_queue<int, greater<int>> heap_t;
  heap_t pq[N];
  typename heap_t::point_iterator its[N];
  int newnode(int x, int i) { return its[i] = pq[i].push(x), i; }
  int top(int x) { return pq[x].top(); }
  void join(int x, int y) { pq[x].join(pq[y]); }
  void decrease_key(int h, int p, int v) { pq[h].modify(its[p], v); }
  void erase(int h, int x) { pq[h].erase(its[x]); }
};

