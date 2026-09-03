#include <bits/stdc++.h>
using namespace std;
#define int long long


template <size_t N> struct heap_binary_ext {
  static constexpr size_t M = N << 1;
  int ivl[M], bel[M], vl[M];
  vector<vector<int>> buf;
  int fa[N], rk[N];
  void cmin(int &x, int y) { x > y && (x = y); }
  struct eraseable_heap {
    heap_binary_ext *H;
    priority_queue<int> pq, dl;
    void push(int x, int i) { pq.push(H->ivl[i] = x); }
    int top() {
      while (!dl.empty() && !pq.empty() && pq.top() == dl.top()) pq.pop(), dl.pop();
      return pq.empty() ? LLONG_MAX : pq.top();
    }
    void erase(int x) {
      if (dl.push(H->ivl[x]), (dl.size() << 1) >= pq.size()) {
        vector<int> ret;
        while (true) {
          int t = top();
          if (t == LLONG_MAX) break;
          ret.push_back(t), pq.pop();
        }
        for (int x : ret) pq.push(x);
      }
    }
  };
  struct fastpush_heap {
    heap_binary_ext *H;
    eraseable_heap pq;
    vector<int> nbuf;
    int sz, tp;
    fastpush_heap() : sz(0), tp(LLONG_MAX) {}
    int qmin(vector<int> &v) {
      int x = LLONG_MAX;
      for (int y : v) H->cmin(x, H->vl[y]);
      return x;
    }
    static void erase(vector<int> &v, int t) { v.erase(find(v.begin(), v.end(), t)); }
    void push(int x, int i) {
      H->cmin(tp, H->vl[i] = x), H->bel[i] = -1;
      nbuf.push_back(i);
      if ((1 << ((int)nbuf.size() >> 1)) > ++sz) {
        int t = H->buf.size();
        for (int x : nbuf) H->bel[x] = t;
        pq.push(qmin(nbuf), t);
        H->buf.push_back(nbuf), nbuf.clear();
      }
    }
    int top() const { return tp; }
    void modify(int x, int v) {
      int t = H->bel[x];
      H->vl[x] = v;
      if (t == -1) {
        if (v == LLONG_MAX) erase(nbuf, x);
      }
      else {
        pq.erase(t);
        if (v == LLONG_MAX) erase(H->buf[t], x);
        if (!H->buf[t].empty()) pq.push(qmin(H->buf[t]), t);
      }
      tp = min(pq.top(), qmin(nbuf)), sz -= v == LLONG_MAX;
    }
    int size() const { return sz; }
  };
  fastpush_heap pq[N];
  heap_binary_ext() {
    for (size_t i = 0; i < N; ++i) pq[i].H = this, pq[i].pq.H = this;
  }
  int newnode(int x, int i) { return pq[i].push(x, i), i; }
  int top(int x) { return pq[x].top(); }
  void join(int &x, int y) {
    if (rk[x] < rk[y]) swap(x, y);
    if (rk[x] == rk[y]) ++rk[x];
    pq[x].push(pq[y].top(), y + N), fa[y] = x;
  }
  void decrease_key(int h, int p, int v) {
    for (int x = p; int &f = fa[x]; x = exchange(f, h)) {
      if (f == h) {
        pq[f].modify(x + N, pq[x].top());
        break;
      }
      pq[f].modify(x + N, LLONG_MAX);
      if (pq[x].size()) pq[h].push(pq[x].top(), x + N);
    }
    if (pq[p].modify(p, v), p != h) pq[h].modify(p + N, pq[p].top());
  }
  void erase(int h, int x) { decrease_key(h, x, LLONG_MAX); }
};

