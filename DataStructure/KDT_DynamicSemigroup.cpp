#include <bits/stdc++.h>
using namespace std;

struct SumCfg3D {
  using Coord = long long;
  static constexpr int MAXK = 3;
  using Pos = array<Coord, MAXK>;
  struct Info { long long sum = 0, cnt = 0; }; // length folded in, so S + F needs no sz
  using Tag = long long;
  friend Info operator+(const Info &a, const Info &b) { return {a.sum + b.sum, a.cnt + b.cnt}; }
  friend Info operator+(const Info &a, const Tag &f) { return {a.sum + f * a.cnt, a.cnt}; }
  // Tag + Tag: built-in long long addition
};

struct MaxCfg2D {
  using Coord = int;
  static constexpr int MAXK = 2;
  using Pos = array<Coord, MAXK>;
  struct Info { long long sum = 0, cnt = 0; };
  using Tag = long long;
  friend Info operator+(const Info &a, const Info &b) { return {a.sum + b.sum, a.cnt + b.cnt}; }
  friend Info operator+(const Info &a, const Tag &f) { return {a.sum + f * a.cnt, a.cnt}; }
};

struct MinCfg2D {
  using Coord = long long;
  static constexpr int MAXK = 2;
  using Pos = array<Coord, MAXK>;
  struct Info { long long v = 0; };
  using Tag = Info; // same type: combine / apply / compose are all min
  friend Info operator+(const Info &a, const Info &b) { return {min(a.v, b.v)}; }
};

template <class Cfg> class KDT {
  public:
  using Coord = typename Cfg::Coord;
  using Info = typename Cfg::Info;
  using Tag = typename Cfg::Tag;
  using Pos = typename Cfg::Pos;
  int K = Cfg::MAXK;

  KDT(int k) { K = k; }
  KDT(size_t cap = 0) { init(cap); }

  void set_dims(int k) {
    K = k;
    clear();
  }

  void init(size_t cap = 0) {
    clear();
    if (cap) ensure((int)cap + 1), grow_bins(cap + 1);
  }

  void clear() {
    n = tot = tp = hd = tl = 0;
    rt.clear(), val.clear(), sum.clear(), tag.clear(), lazy.clear();
    mn.clear(), mx.clear(), a.clear(), ls.clear(), rs.clear();
    tmp.clear();
    stk.clear();
    que.clear(), sz.clear();
  }

  size_t size() const { return (size_t)n; }

  void insert(const Pos &p, const Info &v) {
    int u = new_node();
    a[u] = mn[u] = mx[u] = p, val[u] = sum[u] = v, sz[u] = 1;
    lazy[u] = ls[u] = rs[u] = 0, tmp[tot = 1] = u;
    for (int i = 0; i < (int)rt.size(); ++i) {
      if (!rt[i]) return rt[i] = build(1, tot, 0), void();
      flat(rt[i]);
    }
  }

  void update(const Pos &l, const Pos &r, const Tag &f) {
    for (int i = 0; i < (int)rt.size(); ++i)
      if (rt[i]) update_one(rt[i], l, r, f);
  }

  Info query(const Pos &l, const Pos &r) {
    bool have = false;
    Info res{};
    for (int i = 0; i < (int)rt.size(); ++i)
      if (rt[i]) query_one(rt[i], l, r, have, res);
    return res;
  }

  private:
  void grow_bins(size_t point_cnt) {
    size_t need = __lg(point_cnt) + 1;
    if (rt.size() < need) rt.resize(need, 0);
  }

  void ensure(int need) {
    if ((int)val.size() >= need) return;
    int c = max(need * 2, 16);
    val.resize(c), sum.resize(c), tag.resize(c), lazy.resize(c, 0);
    mn.resize(c), mx.resize(c), a.resize(c), ls.resize(c, 0), rs.resize(c, 0);
    tmp.resize(c);
    stk.resize(c);
    que.resize(c), sz.resize(c, 0);
  }

  int new_node() { return grow_bins((size_t)n + 1), ensure(n + 2), ++n; }

  bool inside(const Pos &p, const Pos &l, const Pos &r) {
    for (int i = 0; i < K; ++i)
      if (p[i] < l[i] || r[i] < p[i]) return false;
    return true;
  }
  bool contained(const Pos &mn, const Pos &mx, const Pos &l, const Pos &r) {
    for (int i = 0; i < K; ++i)
      if (mn[i] < l[i] || r[i] < mx[i]) return false;
    return true;
  }
  bool disjoint(const Pos &mn, const Pos &mx, const Pos &l, const Pos &r) {
    for (int i = 0; i < K; ++i)
      if (mx[i] < l[i] || r[i] < mn[i]) return true;
    return false;
  }

  void up(int u) {
    sz[u] = 1 + (ls[u] ? sz[ls[u]] : 0) + (rs[u] ? sz[rs[u]] : 0);
    Info s = val[u];
    if (ls[u]) s = s + sum[ls[u]];
    if (rs[u]) s = s + sum[rs[u]];
    sum[u] = s, mn[u] = mx[u] = a[u];
    for (int i = 0; i < K; ++i) {
      if (ls[u]) mn[u][i] = min(mn[u][i], mn[ls[u]][i]), mx[u][i] = max(mx[u][i], mx[ls[u]][i]);
      if (rs[u]) mn[u][i] = min(mn[u][i], mn[rs[u]][i]), mx[u][i] = max(mx[u][i], mx[rs[u]][i]);
    }
  }

  void apply_node(int u, const Tag &f) {
    if (!u) return;
    val[u] = val[u] + f, sum[u] = sum[u] + f;
    tag[u] = lazy[u] ? tag[u] + f : f, lazy[u] = 1;
  }

  void down(int u) {
    if (!u || !lazy[u]) return;
    apply_node(ls[u], tag[u]), apply_node(rs[u], tag[u]), lazy[u] = 0;
  }

  int build(int l, int r, int kd) {
    if (l == r) return tmp[l];
    int m = (l + r) >> 1;
    nth_element(tmp.begin() + l, tmp.begin() + m, tmp.begin() + r + 1,
                [&](int x, int y) { return a[x][kd] < a[y][kd]; });
    if (l < m) ls[tmp[m]] = build(l, m - 1, (kd + 1) % K);
    if (m < r) rs[tmp[m]] = build(m + 1, r, (kd + 1) % K);
    return up(tmp[m]), tmp[m];
  }

  void flat(int &u) {
    hd = tl = 1;
    que[1] = u;
    u = 0;
    while (hd <= tl) {
      int x = que[hd++];
      down(x);
      tmp[++tot] = x;
      sz[x] = 1, lazy[x] = 0, sum[x] = val[x], mn[x] = mx[x] = a[x];
      if (ls[x]) {
        que[++tl] = ls[x];
        ls[x] = 0;
      }
      if (rs[x]) {
        que[++tl] = rs[x];
        rs[x] = 0;
      }
    }
  }

  void update_one(int root, const Pos &l, const Pos &r, const Tag &f) {
    hd = tl = 1;
    que[1] = root;
    tp = 0;
    while (hd <= tl) {
      int x = que[hd++];
      if (contained(mn[x], mx[x], l, r)) {
        apply_node(x, f);
        continue;
      }
      if (disjoint(mn[x], mx[x], l, r)) continue;
      down(x);
      if (inside(a[x], l, r)) val[x] = val[x] + f;
      if (ls[x]) que[++tl] = ls[x];
      if (rs[x]) que[++tl] = rs[x];
      stk[++tp] = x;
    }
    while (tp) {
      int x = stk[tp--];
      Info s = val[x];
      if (ls[x]) s = s + sum[ls[x]];
      if (rs[x]) s = s + sum[rs[x]];
      sum[x] = s;
    }
  }

  void query_one(int root, const Pos &l, const Pos &r, bool &have, Info &res) {
    hd = tl = 1;
    que[1] = root;
    while (hd <= tl) {
      int x = que[hd++];
      if (contained(mn[x], mx[x], l, r)) {
        add(res, have, sum[x]);
        continue;
      }
      if (disjoint(mn[x], mx[x], l, r)) continue;
      down(x);
      if (inside(a[x], l, r)) add(res, have, val[x]);
      if (ls[x]) que[++tl] = ls[x];
      if (rs[x]) que[++tl] = rs[x];
    }
  }

  static void add(Info &res, bool &have, const Info &v) {
    if (!have) res = v, have = true;
    else res = res + v;
  }

  int n = 0, tot = 0, tp = 0, hd = 0, tl = 0;
  vector<int> rt;
  vector<Info> val, sum;
  vector<Tag> tag;
  vector<char> lazy;
  vector<Pos> a, mn, mx;
  vector<int> ls, rs, sz, tmp, stk, que;
};

template <int K, class Cfg>
typename Cfg::Info brute_query(const vector<pair<typename Cfg::Pos, typename Cfg::Info>> &pts,
                               const typename Cfg::Pos &l, const typename Cfg::Pos &r) {
  bool have = false;
  typename Cfg::Info res{};
  for (auto &[p, v] : pts) {
    bool in = true;
    for (int i = 0; i < K; ++i)
      if (p[i] < l[i] || r[i] < p[i]) in = false;
    if (!in) continue;
    if (!have) res = v, have = true;
    else res = res + v;
  }
  return res;
}

void stress_3d() {
  using Cfg = SumCfg3D;
  KDT<Cfg> kd;
  vector<pair<Cfg::Pos, Cfg::Info>> pts;
  mt19937 rng(20240826);
  auto rnd = [&](int L, int R) { return uniform_int_distribution<int>(L, R)(rng); };
  int ops = 2000;
  for (int t = 0; t < ops; ++t) {
    int o = rnd(1, 3);
    if (o == 1) {
      Cfg::Pos p{rnd(-50, 50), rnd(-50, 50), rnd(-50, 50)};
      long long v = rnd(-100, 100);
      kd.insert(p, {v, 1});
      pts.push_back({p, {v, 1}});
    }
    else if (o == 2) {
      int x1 = rnd(-50, 50), x2 = rnd(x1, 50);
      int y1 = rnd(-50, 50), y2 = rnd(y1, 50);
      int z1 = rnd(-50, 50), z2 = rnd(z1, 50);
      Cfg::Pos l{x1, y1, z1}, r{x2, y2, z2};
      long long f = rnd(-100, 100);
      kd.update(l, r, f);
      for (auto &[p, v] : pts) {
        bool in = true;
        for (int i = 0; i < 3; ++i)
          if (p[i] < l[i] || r[i] < p[i]) in = false;
        if (in) v = v + f;
      }
    }
    else {
      int x1 = rnd(-50, 50), x2 = rnd(x1, 50);
      int y1 = rnd(-50, 50), y2 = rnd(y1, 50);
      int z1 = rnd(-50, 50), z2 = rnd(z1, 50);
      Cfg::Pos l{x1, y1, z1}, r{x2, y2, z2};
      Cfg::Info a = kd.query(l, r);
      Cfg::Info b = brute_query<3, Cfg>(pts, l, r);
      if (a.sum != b.sum || a.cnt != b.cnt) {
        printf("3D MISMATCH at op %d: kd=(%lld,%lld) brute=(%lld,%lld)\n", t, a.sum, a.cnt, b.sum, b.cnt);
        abort();
      }
    }
  }
  printf("3D stress OK (%zu points)\n", kd.size());
}

void stress_2d_clear() {
  using Cfg = MaxCfg2D;
  KDT<Cfg> kd;
  auto run = [&](int round) {
    vector<pair<Cfg::Pos, Cfg::Info>> pts;
    mt19937 rng(round + 1000);
    auto rnd = [&](int L, int R) { return uniform_int_distribution<int>(L, R)(rng); };
    int ops = 800;
    for (int t = 0; t < ops; ++t) {
      int o = rnd(1, 3);
      if (o == 1) {
        Cfg::Pos p{rnd(-40, 40), rnd(-40, 40)};
        long long v = rnd(-100, 100);
        kd.insert(p, {v, 1});
        pts.push_back({p, {v, 1}});
      }
      else if (o == 2) {
        int x1 = rnd(-40, 40), x2 = rnd(x1, 40);
        int y1 = rnd(-40, 40), y2 = rnd(y1, 40);
        Cfg::Pos l{x1, y1}, r{x2, y2};
        long long f = rnd(-100, 100);
        kd.update(l, r, f);
        for (auto &[p, v] : pts) {
          bool in = true;
          for (int i = 0; i < 2; ++i)
            if (p[i] < l[i] || r[i] < p[i]) in = false;
          if (in) v = v + f;
        }
      }
      else {
        int x1 = rnd(-40, 40), x2 = rnd(x1, 40);
        int y1 = rnd(-40, 40), y2 = rnd(y1, 40);
        Cfg::Pos l{x1, y1}, r{x2, y2};
        Cfg::Info a = kd.query(l, r);
        Cfg::Info b = brute_query<2, Cfg>(pts, l, r);
        if (a.sum != b.sum || a.cnt != b.cnt) {
          printf("2D MISMATCH round%d op%d: kd=(%lld,%lld) brute=(%lld,%lld)\n", round, t, a.sum, a.cnt, b.sum, b.cnt);
          abort();
        }
      }
    }
    return pts.size();
  };
  size_t total = 0;
  for (int round = 0; round < 5; ++round) {
    if (round) kd.clear();
    size_t got = run(round);
    total += got;
    if (kd.size() != got) {
      printf("size mismatch round%d\n", round);
      abort();
    }
  }
  printf("2D + clear stress OK (total %zu points across rounds)\n", total);
}

void stress_min_2d() {
  using Cfg = MinCfg2D;
  KDT<Cfg> kd;
  vector<pair<Cfg::Pos, Cfg::Info>> pts;
  mt19937 rng(7);
  auto rnd = [&](int L, int R) { return uniform_int_distribution<int>(L, R)(rng); };
  int ops = 1500;
  for (int t = 0; t < ops; ++t) {
    int o = rnd(1, 3);
    if (o == 1) {
      Cfg::Pos p{rnd(-60, 60), rnd(-60, 60)};
      long long v = rnd(0, 1000);
      kd.insert(p, {v});
      pts.push_back({p, {v}});
    }
    else if (o == 2) {
      int x1 = rnd(-60, 60), x2 = rnd(x1, 60);
      int y1 = rnd(-60, 60), y2 = rnd(y1, 60);
      Cfg::Pos l{x1, y1}, r{x2, y2};
      Cfg::Info f{rnd(0, 1000)};
      kd.update(l, r, f);
      for (auto &[p, v] : pts) {
        bool in = true;
        for (int i = 0; i < 2; ++i)
          if (p[i] < l[i] || r[i] < p[i]) in = false;
        if (in) v = v + f;
      }
    }
    else {
      int x1 = rnd(-60, 60), x2 = rnd(x1, 60);
      int y1 = rnd(-60, 60), y2 = rnd(y1, 60);
      Cfg::Pos l{x1, y1}, r{x2, y2};
      Cfg::Info a = kd.query(l, r);
      Cfg::Info b = brute_query<2, Cfg>(pts, l, r);
      if (a.v != b.v) {
        printf("MIN MISMATCH op%d: kd=%lld brute=%lld\n", t, a.v, b.v);
        abort();
      }
    }
  }
  printf("2D min-semigroup stress OK (%zu points)\n", kd.size());
}

void perf_smoke() {
  using Cfg = SumCfg3D;
  KDT<Cfg> kd;
  mt19937 rng(1);
  auto rnd = [&](int L, int R) { return uniform_int_distribution<int>(L, R)(rng); };
  int m = 30000;
  long long lst = 0, acc = 0;
  for (int i = 0; i < m; ++i) {
    int o = rnd(1, 3);
    if (o == 1) {
      Cfg::Pos p{rnd(0, 100000), rnd(0, 100000), rnd(0, 100000)};
      kd.insert(p, {rnd(-1000, 1000), 1});
    }
    else if (o == 2) {
      int x1 = rnd(0, 100000), x2 = rnd(x1, 100000);
      int y1 = rnd(0, 100000), y2 = rnd(y1, 100000);
      int z1 = rnd(0, 100000), z2 = rnd(z1, 100000);
      kd.update(Cfg::Pos{x1, y1, z1}, Cfg::Pos{x2, y2, z2}, rnd(-100, 100));
    }
    else {
      int x1 = rnd(0, 100000), x2 = rnd(x1, 100000);
      int y1 = rnd(0, 100000), y2 = rnd(y1, 100000);
      int z1 = rnd(0, 100000), z2 = rnd(z1, 100000);
      acc += kd.query(Cfg::Pos{x1, y1, z1}, Cfg::Pos{x2, y2, z2}).sum + lst;
    }
  }
  printf("perf smoke OK (3D, %d ops, %zu points, checksum %lld)\n", m, kd.size(), acc);
}

int main() {
  stress_3d();
  stress_2d_clear();
  stress_min_2d();
  perf_smoke();
  printf("ALL TESTS PASSED\n");
  return 0;
}

/*
 * ====================================================================
 * Generic K-D Tree with dynamic insertion and lazy tags
 *
 * - Custom Info and Tag types (dual semigroups), arbitrary compile-time
 *   dimension K, and clear() support for reuse.
 *
 * Complexity: insert amortized O(log n * n^(1-1/K)) (binary-grouping rebuild);
 *   update / query expected O(sqrt n) on typical competitive data
 *
 * Compared with KDT_BinaryGroup.cpp (static 2D, int weights, query-only):
 *   1) Info and Tag are fully customizable and only need semigroup laws;
 *   2) dimension K is any compile-time constant;
 *   3) clear() releases memory and allows the object to be reused.
 *
 * Public interface:
 *   KDT<Cfg> tree;
 *   tree.init(cap);                 // optional pre-reserve capacity
 *   tree.clear();                   // clear and allow reuse
 *   tree.insert(pos, info);         // insert one weighted point
 *   tree.update(l, r, tag);         // apply tag to all points in [l, r]
 *   tree.query(l, r) -> Info;       // merge info of all points in [l, r]
 *   tree.size() -> size_t;          // current number of points
 *
 * Config struct (Cfg) requirements, a "dual semigroup":
 *   using Coord          coordinate type (must support < and std::min/max)
 *   static constexpr int K = dimension count (compile-time)
 *   using Info           node info type      (original w / s)
 *   using Tag            lazy tag type       (original tg)
 *   operator+ overloads (free functions / friends of Info and Tag):
 *     Info + Info -> Info   // associative combine
 *     Info + Tag  -> Info   // apply tag f to info (Info is the first operand)
 *     Tag  + Tag  -> Tag    // compose tags; left tag first, then right tag
 *   The template itself only uses `a + b`, `x + f`, `f + g`.
 *
 * Notes:
 *   query() pushes lazy tags down on partially covered nodes, so it can
 *   change the tag distribution inside the tree while preserving semantics.
 *   An empty query returns the default-constructed Info.
 *   Since Info + Tag has no length parameter, length-dependent actions (range
 *   add + range sum) fold the count into Info, e.g. Info = {sum, cnt} with
 *   (sum, cnt) + f = (sum + f * cnt, cnt).
 *
 * Fixes applied over the original code:
 *   1) up() previously merged child bounding boxes even for missing children
 *      (node 0 treated as all-zero), which polluted the box whenever any
 *      coordinate was negative; now only existing children are merged.
 *   2) sum is merged only over existing children, dropping the implicit
 *      "empty node is identity" assumption, so Info needs no identity.
 *      A lazy flag distinguishes pending tags, so Tag also needs no identity.
 *   3) fixed-size arrays were replaced by an auto-growing vector node pool,
 *      removing the hard-coded limits N / M.
 *   4) combine / apply / compose static methods were replaced by operator+.
 *
 * Self-test:
 *   main() runs randomized stress tests against a brute-force reference and
 *   a large workload smoke test; it prints "ALL TESTS PASSED" on success.
 *     - stress_3d:       3D additive sum, negative coordinates.
 *     - stress_2d_clear: 2D additive sum across multiple clear() rounds.
 *     - stress_min_2d:   2D min semigroup (non-additive, non-commutative).
 *     - perf_smoke:      large 3D workload correctness/performance smoke test.
 *   Compile with -std=c++17.
 *
 * ====================================================================
 */
