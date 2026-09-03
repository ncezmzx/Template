
template <class S, S (*e)(), class F, F (*id)()>
struct semigroup_segtree {
  struct node {
    S s;
    F tag;
    int lc, rc;
  };
  static constexpr int DEP = 64;
  int n, root;
  vector<node> pool;
  int stk[DEP * 3 + 4];
  int pth[DEP + 2], pl[DEP + 2], pr[DEP + 2];

  void clear() {
    root = 0;
    pool.clear();
    pool.push_back({e(), id(), 0, 0});
  }
  void init(int n_) {
    n = n_;
    clear();
  }

  void init(int n_, const vector<S> &a) {
    init(n_);
    if (n_ < 1) return;
    pool.reserve(2 * n_);
    root = new_node();
    vector<int> qu{root}, ql{1}, qr{n_};
    for (int i = 0; i < (int)qu.size(); i++) {
      int u = qu[i];
      if (ql[i] == qr[i]) {
        pool[u].s = a[ql[i]];
        continue;
      }
      int m = (ql[i] + qr[i]) >> 1, c1 = new_node(), c2 = new_node();
      pool[u].lc = c1, pool[u].rc = c2;
      qu.push_back(c1), ql.push_back(ql[i]), qr.push_back(m);
      qu.push_back(c2), ql.push_back(m + 1), qr.push_back(qr[i]);
    }
    for (int i = (int)qu.size() - 1; i >= 0; i--)
      if (ql[i] != qr[i]) pull(qu[i]);
  }
  void reserve(int k) { pool.reserve(k); }
  int new_node() {
    pool.push_back({e(), id(), 0, 0});
    return (int)pool.size() - 1;
  }

  void apply(int u, F f) {
    if (!u) return;
    pool[u].s = pool[u].s + f;
    pool[u].tag = pool[u].tag + f;
  }
  void push(int u) {

    if (!pool[u].lc) {
      int c = new_node();
      pool[u].lc = c;
    }
    if (!pool[u].rc) {
      int c = new_node();
      pool[u].rc = c;
    }
    if (pool[u].tag != id()) {
      apply(pool[u].lc, pool[u].tag);
      apply(pool[u].rc, pool[u].tag);
      pool[u].tag = id();
    }
  }
  void pull(int u) { pool[u].s = pool[pool[u].lc].s + pool[pool[u].rc].s; }

  void update(int x, int y, F f) {
    if (x > y) return;
    if (!root) root = new_node();
    int u = root, l = 1, r = n, tp = 0;
    while (l != r) {
      int m = (l + r) >> 1;
      if (y <= m) push(u), stk[tp++] = u, u = pool[u].lc, r = m;
      else if (x > m) push(u), stk[tp++] = u, u = pool[u].rc, l = m + 1;
      else break;
    }
    if (l == r) {
      apply(u, f);
    }
    else {
      push(u), stk[tp++] = u;
      int m = (l + r) >> 1;
      for (int v = pool[u].lc, ll = l, rr = m;;) {
        if (x <= ll) {
          apply(v, f);
          break;
        }
        push(v), stk[tp++] = v;
        int mm = (ll + rr) >> 1;
        if (x <= mm) apply(pool[v].rc, f), v = pool[v].lc, rr = mm;
        else v = pool[v].rc, ll = mm + 1;
      }
      for (int v = pool[u].rc, ll = m + 1, rr = r;;) {
        if (rr <= y) {
          apply(v, f);
          break;
        }
        push(v), stk[tp++] = v;
        int mm = (ll + rr) >> 1;
        if (y > mm) apply(pool[v].lc, f), v = pool[v].rc, ll = mm + 1;
        else v = pool[v].lc, rr = mm;
      }
    }
    while (tp) pull(stk[--tp]);
  }
  void update(int x, F f) { update(x, x, f); }

  void set(int x, S v) {
    if (!root) root = new_node();
    int u = root, l = 1, r = n, tp = 0;
    while (l != r) {
      push(u), stk[tp++] = u;
      int m = (l + r) >> 1;
      if (x <= m) u = pool[u].lc, r = m;
      else u = pool[u].rc, l = m + 1;
    }
    pool[u].s = v;
    while (tp) pull(stk[--tp]);
  }

  S query(int x, int y) {
    if (x > y || !root) return e();
    int u = root, l = 1, r = n;
    while (l != r) {
      int m = (l + r) >> 1;
      if (y <= m) {
        if (!u) return e();
        if (pool[u].tag != id()) push(u);
        u = pool[u].lc, r = m;
      }
      else if (x > m) {
        if (!u) return e();
        if (pool[u].tag != id()) push(u);
        u = pool[u].rc, l = m + 1;
      }
      else break;
    }
    if (!u || l == r) return u ? pool[u].s : e();
    if (pool[u].tag != id()) push(u);
    int m = (l + r) >> 1;
    S res = e();
    for (int v = pool[u].lc, ll = l, rr = m;;) {
      if (x <= ll) {
        if (v) res = pool[v].s + res;
        break;
      }
      if (!v) break;
      if (pool[v].tag != id()) push(v);
      int mm = (ll + rr) >> 1;
      if (x <= mm) {
        int c = pool[v].rc;
        if (c) res = pool[c].s + res;
        v = pool[v].lc, rr = mm;
      }
      else v = pool[v].rc, ll = mm + 1;
    }
    for (int v = pool[u].rc, ll = m + 1, rr = r;;) {
      if (rr <= y) {
        if (v) res = res + pool[v].s;
        break;
      }
      if (!v) break;
      if (pool[v].tag != id()) push(v);
      int mm = (ll + rr) >> 1;
      if (y > mm) {
        int c = pool[v].lc;
        if (c) res = res + pool[c].s;
        v = pool[v].rc, ll = mm + 1;
      }
      else v = pool[v].lc, rr = mm;
    }
    return res;
  }
  S query(int x) { return query(x, x); }
  S get(int x) { return query(x, x); }
  S all_prod() { return root ? pool[root].s : e(); }

  template <class G> int max_right(int x, G g) {
    if (x > n || !root) return n;
    int u = root, l = 1, r = n, tp = 0;
    while (l != r) {
      if (pool[u].tag != id()) push(u);
      pth[tp] = u, pl[tp] = l, pr[tp] = r, tp++;
      int m = (l + r) >> 1;
      if (x <= m) u = pool[u].lc, r = m;
      else u = pool[u].rc, l = m + 1;
    }
    pth[tp] = u;
    S sm = e();
    if (!g(sm + pool[u].s)) return x - 1;
    sm = sm + pool[u].s;
    for (int i = tp - 1; i >= 0; i--) {
      int p = pth[i];
      if (pth[i + 1] != pool[p].lc) continue;
      int b = pool[p].rc, m = (pl[i] + pr[i]) >> 1;
      if (!b) continue;
      if (g(sm + pool[b].s)) {
        sm = sm + pool[b].s;
        continue;
      }
      int w = b, lw = m + 1, rw = pr[i];
      while (w && lw < rw) {
        if (pool[w].tag != id()) push(w);
        int mm = (lw + rw) >> 1;
        if (g(sm + pool[pool[w].lc].s)) sm = sm + pool[pool[w].lc].s, w = pool[w].rc, lw = mm + 1;
        else w = pool[w].lc, rw = mm;
      }
      if (!w) continue;
      return lw - 1;
    }
    return n;
  }

  template <class G> int min_left(int y, G g) {
    if (y < 1 || !root) return 1;
    int u = root, l = 1, r = n, tp = 0;
    while (l != r) {
      if (pool[u].tag != id()) push(u);
      pth[tp] = u, pl[tp] = l, pr[tp] = r, tp++;
      int m = (l + r) >> 1;
      if (y <= m) u = pool[u].lc, r = m;
      else u = pool[u].rc, l = m + 1;
    }
    pth[tp] = u;
    S sm = e();
    if (!g(pool[u].s + sm)) return y + 1;
    sm = pool[u].s + sm;
    for (int i = tp - 1; i >= 0; i--) {
      int p = pth[i];
      if (pth[i + 1] != pool[p].rc) continue;
      int b = pool[p].lc, m = (pl[i] + pr[i]) >> 1;
      if (!b) continue;
      if (g(pool[b].s + sm)) {
        sm = pool[b].s + sm;
        continue;
      }
      int w = b, lw = pl[i], rw = m;
      while (w && lw < rw) {
        if (pool[w].tag != id()) push(w);
        int mm = (lw + rw) >> 1;
        if (g(pool[pool[w].rc].s + sm)) sm = pool[pool[w].rc].s + sm, w = pool[w].lc, rw = mm;
        else w = pool[w].rc, lw = mm + 1;
      }
      if (!w) continue;
      return lw + 1;
    }
    return 1;
  }
};

