#include <bits/stdc++.h>
using namespace std;

/*
 * ============================================================
 * 名称：K-D Tree（动态插入 + 懒标记，泛型双半群 + 任意维度）
 * 复杂度：insert 均摊 O(log n * n^(1-1/K))（二进制分组重建）；
 *         update / query 期望 O(sqrt n)（OI 常规数据，构造数据最坏 O(n)）
 * 用途：K 维带权点集，支持动态插入、矩形区域内整体加标记（tag）、
 *       矩形区域信息查询（info），节点/子树信息只需满足半群性质
 * 相比 KDT_BinaryGroup.cpp（二维静态、int 权、仅查询）：
 *       本版为泛型封装 —— ① info 与 tag 类型可任意自定义，仅需满足
 *       “双半群”结合律；② 维度数 K 可为任意编译期常量；③ 支持 clear()
 *       清空后重复使用。原 tg / s / w 分别对应本模板的 Tag / sum / val。
 * 来源：用户提供模板（二进制分组 + 懒标记 K-D Tree），已泛型化并修复：
 *       1) up() 原把不存在的子节点（编号 0）包围盒当作全 0 参与 min/max，
 *          坐标含负数时包围盒被污染 → 改为仅合并存在的子节点；
 *       2) sum 合并改为显式按存在性子节点合并，摆脱“空节点即幺元”假设，
 *          使 info 只需半群（不必含幺元）；懒标记用 lazy 标志位区分，
 *          使 tag 也只需半群（不必含幺元）；
 *       3) 改为 vector 节点池 + 按需扩容，去掉固定 N。
 * 接口：
 *   KDT<Cfg> tree;                       // Cfg 见下方“配置结构体”
 *   tree.init(cap);  tree.clear();       // 初始化 / 清空（释放内存、可复用）
 *   tree.insert(pos, info);              // 动态插入一个带权点
 *   tree.update(l, r, tag);              // 对矩形 [l, r] 内所有点应用标记 tag
 *   tree.query(l, r) -> Info;            // 矩形 [l, r] 内所有点的信息合并
 *   tree.size() -> size_t;               // 当前点数
 * 配置结构体 Cfg 需求（双半群）：
 *   using Coord      坐标类型（须支持 < 比较与 std::min/max）
 *   static constexpr int K = 维度数（编译期常量）
 *   using Info       节点信息类型（对应原 w / s）
 *   using Tag        懒标记类型（对应原 tg）
 *   static Info  combine(Info a, Info b)    // 信息结合（结合律）
 *   static Info  apply  (Info x, Tag f, int sz)  // 对“sz 个点聚合为 x”的信息整体施加标记 f
 *       —— 即 apply 需满足对 combine 的分配律：op 表示 combine 时
 *          apply(op(a,b), f, szA+szB) == op(apply(a,f,szA), apply(b,f,szB))
 *   static Tag   compose(Tag f, Tag g)     // 标记合成：先 g 后 f 等价于 f∘g
 *       —— 需满足 apply(x, compose(f,g), sz) == apply(apply(x,g,sz), f, sz)
 * 说明：query 在部分覆盖节点时会下传懒标记（down），因此会改变树内标记
 *       分布，但信息语义保持一致；空集查询返回 Info 的默认构造值。
 * 特化示例（等价于原代码 int 加和）见下方使用示例。
 * ============================================================
 */

// ---------------- 配置结构体（二选一，任选其一即可编译） ----------------
// ① 原模板等价配置：3 维、long long 坐标、加和 info、加法 tag
struct SumCfg3D {
  using Coord = long long;
  static constexpr int K = 3;
  using Pos = array<Coord, K>;
  using Info = long long;   // 对应原 w / s
  using Tag = long long;    // 对应原 tg
  static Info combine(Info a, Info b) { return a + b; }
  static Info apply(Info x, Tag f, int sz) { return x + f * (Info)sz; }
  static Tag compose(Tag f, Tag g) { return f + g; }   // 加法交换，顺序无关
};

// ② 二维、加法 tag 的另一种 info 示例（展示 info 与 tag 可不同）
struct MaxCfg2D {
  using Coord = int;
  static constexpr int K = 2;
  using Pos = array<Coord, K>;
  using Info = long long;  // 存每个点/子树的“和”
  using Tag = long long;   // 懒标记 = 全体加上一个增量
  static Info combine(Info a, Info b) { return a + b; }
  static Info apply(Info x, Tag f, int sz) { return x + f * (Info)sz; }
  static Tag compose(Tag f, Tag g) { return f + g; }
};

// ③ 非加法半群示例：Info = 子集内的 min，Tag = “取 min(f, x)”。
//    展示 info/tag 不必是加法/可交换，仅需半群性质：
//    combine = min，apply(x,f) = min(x,f)，compose(f,g) = min(f,g)，
//    对 min 分配律成立，故整体加标记语义自洽。
struct MinCfg2D {
  using Coord = long long;
  static constexpr int K = 2;
  using Pos = array<Coord, K>;
  using Info = long long;  // 每个点/子树的“最小值”
  using Tag = long long;   // 懒标记 = 全体与 f 取 min
  static Info combine(Info a, Info b) { return min(a, b); }
  static Info apply(Info x, Tag f, int) { return min(x, f); }
  static Tag compose(Tag f, Tag g) { return min(f, g); }
};

// ---------------- 泛型模板本体 ----------------
template <class Cfg>
class KDT {
 public:
  using Coord = typename Cfg::Coord;
  using Info = typename Cfg::Info;
  using Tag = typename Cfg::Tag;
  static constexpr int K = Cfg::K;
  using Pos = array<Coord, K>;

  KDT(size_t cap = 0) { init(cap); }

  // 初始化，预留 cap 个点的容量（可不调，按需扩容）
  void init(size_t cap = 0) {
    clear();
    if (cap) {
      ensure((int)cap + 1);
      grow_bins(cap + 1);
    }
  }
  // 清空：释放内存并复位所有状态，之后可继续 insert（节点从 1 重新分配）
  void clear() {
    n = tot = tp = hd = tl = 0;
    rt.clear();
    val.clear(); sum.clear(); tag.clear(); lazy.clear();
    mn.clear(); mx.clear(); a.clear();
    ls.clear(); rs.clear(); sz.clear();
    tmp.clear(); stk.clear(); que.clear();
  }
  size_t size() const { return (size_t)n; }

  void insert(const Pos& p, const Info& v) {
    int u = new_node();
    a[u] = p; mn[u] = mx[u] = p;
    val[u] = v; sum[u] = v; sz[u] = 1;
    lazy[u] = 0; ls[u] = rs[u] = 0;
    tmp[tot = 1] = u;
    for (int i = 0; i < (int)rt.size(); ++i) {
      if (!rt[i]) { rt[i] = build(1, tot, 0); return; }
      flat(rt[i]);   // 腾空第 i 组，节点并入 tmp
    }
  }

  // 对矩形 [l, r]（含端点）内所有点应用标记 f
  void update(const Pos& l, const Pos& r, const Tag& f) {
    for (int i = 0; i < (int)rt.size(); ++i)
      if (rt[i]) update_one(rt[i], l, r, f);
  }
  // 查询矩形 [l, r]（含端点）内所有点的信息合并；空集返回 Info 默认值
  Info query(const Pos& l, const Pos& r) {
    bool have = false;
    Info res{};
    for (int i = 0; i < (int)rt.size(); ++i)
      if (rt[i]) query_one(rt[i], l, r, have, res);
    return res;
  }

 private:
  static size_t bitlen(size_t x) { size_t b = 0; while (x) ++b, x >>= 1; return b; }

  void grow_bins(size_t point_cnt) {   // 二进制分组组数 = ceil(log2(cnt))+1
    size_t need = bitlen(point_cnt) + 1;
    if (rt.size() < need) rt.resize(need, 0);
  }
  void ensure(int need) {
    if ((int)val.size() >= need) return;
    int c = max(need * 2, 16);
    val.resize(c); sum.resize(c); tag.resize(c); lazy.resize(c, 0);
    mn.resize(c); mx.resize(c); a.resize(c);
    ls.resize(c, 0); rs.resize(c, 0); sz.resize(c, 0);
    tmp.resize(c); stk.resize(c); que.resize(c);
  }
  int new_node() {
    grow_bins((size_t)n + 1);          // 新点可能使需要的组数增加
    ensure(n + 2);                     // 节点 1-based：访问 a[n+1] 需 size>=n+2
    return ++n;
  }

  // ---------- 坐标/包围盒判断 ----------
  static bool inside(const Pos& p, const Pos& l, const Pos& r) {
    for (int i = 0; i < K; ++i) if (p[i] < l[i] || r[i] < p[i]) return false;
    return true;
  }
  static bool contained(const Pos& mn, const Pos& mx, const Pos& l, const Pos& r) {
    for (int i = 0; i < K; ++i) if (mn[i] < l[i] || r[i] < mx[i]) return false;
    return true;
  }
  static bool disjoint(const Pos& mn, const Pos& mx, const Pos& l, const Pos& r) {
    for (int i = 0; i < K; ++i) if (mx[i] < l[i] || r[i] < mn[i]) return true;
    return false;
  }

  void up(int u) {
    sz[u] = 1 + (ls[u] ? sz[ls[u]] : 0) + (rs[u] ? sz[rs[u]] : 0);
    Info s = val[u];
    if (ls[u]) s = Cfg::combine(s, sum[ls[u]]);
    if (rs[u]) s = Cfg::combine(s, sum[rs[u]]);
    sum[u] = s;
    mn[u] = mx[u] = a[u];
    for (int i = 0; i < K; ++i) {
      if (ls[u]) { mn[u][i] = min(mn[u][i], mn[ls[u]][i]); mx[u][i] = max(mx[u][i], mx[ls[u]][i]); }
      if (rs[u]) { mn[u][i] = min(mn[u][i], mn[rs[u]][i]); mx[u][i] = max(mx[u][i], mx[rs[u]][i]); }
    }
  }
  void apply_node(int u, const Tag& f) {
    if (!u) return;
    val[u] = Cfg::apply(val[u], f, 1);
    sum[u] = Cfg::apply(sum[u], f, sz[u]);
    tag[u] = lazy[u] ? Cfg::compose(f, tag[u]) : f;
    lazy[u] = 1;
  }
  void down(int u) {
    if (!u || !lazy[u]) return;
    apply_node(ls[u], tag[u]);
    apply_node(rs[u], tag[u]);
    lazy[u] = 0;
  }

  int build(int l, int r, int kd) {
    if (l == r) return tmp[l];
    int m = (l + r) >> 1;
    nth_element(tmp.begin() + l, tmp.begin() + m, tmp.begin() + r + 1,
                [&](int x, int y) { return a[x][kd] < a[y][kd]; });
    if (l < m) ls[tmp[m]] = build(l, m - 1, (kd + 1) % K);
    if (m < r) rs[tmp[m]] = build(m + 1, r, (kd + 1) % K);
    up(tmp[m]);
    return tmp[m];
  }
  void flat(int& u) {
    hd = tl = 1; que[1] = u; u = 0;
    while (hd <= tl) {
      int x = que[hd++];
      down(x);
      tmp[++tot] = x;
      sz[x] = 1; lazy[x] = 0; sum[x] = val[x]; mn[x] = mx[x] = a[x];
      if (ls[x]) { que[++tl] = ls[x]; ls[x] = 0; }
      if (rs[x]) { que[++tl] = rs[x]; rs[x] = 0; }
    }
  }

  void update_one(int root, const Pos& l, const Pos& r, const Tag& f) {
    hd = tl = 1; que[1] = root; tp = 0;
    while (hd <= tl) {
      int x = que[hd++];
      if (contained(mn[x], mx[x], l, r)) { apply_node(x, f); continue; }
      if (disjoint(mn[x], mx[x], l, r)) continue;
      down(x);
      if (inside(a[x], l, r)) val[x] = Cfg::apply(val[x], f, 1);
      if (ls[x]) que[++tl] = ls[x];
      if (rs[x]) que[++tl] = rs[x];
      stk[++tp] = x;
    }
    while (tp) {
      int x = stk[tp--];
      Info s = val[x];
      if (ls[x]) s = Cfg::combine(s, sum[ls[x]]);
      if (rs[x]) s = Cfg::combine(s, sum[rs[x]]);
      sum[x] = s;
    }
  }
  void query_one(int root, const Pos& l, const Pos& r, bool& have, Info& res) {
    hd = tl = 1; que[1] = root;
    while (hd <= tl) {
      int x = que[hd++];
      if (contained(mn[x], mx[x], l, r)) { add(res, have, sum[x]); continue; }
      if (disjoint(mn[x], mx[x], l, r)) continue;
      down(x);
      if (inside(a[x], l, r)) add(res, have, val[x]);
      if (ls[x]) que[++tl] = ls[x];
      if (rs[x]) que[++tl] = rs[x];
    }
  }
  static void add(Info& res, bool& have, const Info& v) {
    if (!have) res = v, have = true;
    else res = Cfg::combine(res, v);
  }

  int n = 0, tot = 0, tp = 0, hd = 0, tl = 0;
  vector<int> rt;                       // 二进制分组各组的根
  vector<Info> val, sum;                // 点自身信息 / 子树信息
  vector<Tag> tag;                      // 懒标记
  vector<char> lazy;                    // 该节点是否挂有懒标记
  vector<Pos> a, mn, mx;                // 点坐标 / 子树包围盒
  vector<int> ls, rs, sz, tmp, stk, que;
};

/* ============================================================
 * 使用示例（等价于原代码，3 维 long long 加和；编译时取消注释）：
 * using Cfg = SumCfg3D;
 * int main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   KDT<Cfg> kd;
 *   int m; cin >> m;
 *   long long lst = 0;
 *   while (m--) {
 *     int o; cin >> o;
 *     Cfg::Pos l, r;
 *     if (o == 1) {
 *       for (auto& x : l) cin >> x, x ^= lst;
 *       long long v; cin >> v, v ^= lst;
 *       kd.insert(l, v);
 *     } else if (o == 2) {
 *       for (auto& x : l) cin >> x, x ^= lst;
 *       for (auto& x : r) cin >> x, x ^= lst;
 *       long long v; cin >> v, v ^= lst;
 *       kd.update(l, r, v);
 *     } else {
 *       for (auto& x : l) cin >> x, x ^= lst;
 *       for (auto& x : r) cin >> x, x ^= lst;
 *       cout << (lst = kd.query(l, r)) << '\n';
 *     }
 *   }
 * }
 * ============================================================
 */

// ========================= 自测：随机数据对拍暴力 =========================
namespace {  // 仅作验证用，可删除
template <int K, class Cfg>
typename Cfg::Info brute_query(const vector<pair<typename Cfg::Pos, typename Cfg::Info>>& pts,
                 const typename Cfg::Pos& l, const typename Cfg::Pos& r) {
  bool have = false; typename Cfg::Info res{};
  for (auto& [p, v] : pts) {
    bool in = true;
    for (int i = 0; i < K; ++i) if (p[i] < l[i] || r[i] < p[i]) in = false;
    if (!in) continue;
    if (!have) res = v, have = true;
    else res = Cfg::combine(res, v);
  }
  return res;
}
}  // namespace

void stress_3d() {
  using Cfg = SumCfg3D;
  KDT<Cfg> kd;
  vector<pair<Cfg::Pos, long long>> pts;
  mt19937 rng(20240826);
  auto rnd = [&](int L, int R) { return uniform_int_distribution<int>(L, R)(rng); };
  int ops = 2000;
  for (int t = 0; t < ops; ++t) {
    int o = rnd(1, 3);
    if (o == 1) {
      Cfg::Pos p{rnd(-50, 50), rnd(-50, 50), rnd(-50, 50)};
      long long v = rnd(-100, 100);
      kd.insert(p, v);
      pts.push_back({p, v});
    } else if (o == 2) {
      int x1 = rnd(-50, 50), x2 = rnd(x1, 50);
      int y1 = rnd(-50, 50), y2 = rnd(y1, 50);
      int z1 = rnd(-50, 50), z2 = rnd(z1, 50);
      Cfg::Pos l{x1, y1, z1}, r{x2, y2, z2};
      long long f = rnd(-100, 100);
      kd.update(l, r, f);
      for (auto& [p, v] : pts) {
        bool in = true;
        for (int i = 0; i < 3; ++i) if (p[i] < l[i] || r[i] < p[i]) in = false;
        if (in) v += f;
      }
    } else {
      int x1 = rnd(-50, 50), x2 = rnd(x1, 50);
      int y1 = rnd(-50, 50), y2 = rnd(y1, 50);
      int z1 = rnd(-50, 50), z2 = rnd(z1, 50);
      Cfg::Pos l{x1, y1, z1}, r{x2, y2, z2};
      long long a = kd.query(l, r);
      long long b = brute_query<3, Cfg>(pts, l, r);
      if (a != b) {
        printf("3D MISMATCH at op %d: kd=%lld brute=%lld\n", t, a, b);
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
    vector<pair<Cfg::Pos, long long>> pts;
    mt19937 rng(round + 1000);
    auto rnd = [&](int L, int R) { return uniform_int_distribution<int>(L, R)(rng); };
    int ops = 800;
    for (int t = 0; t < ops; ++t) {
      int o = rnd(1, 3);
      if (o == 1) {
        Cfg::Pos p{rnd(-40, 40), rnd(-40, 40)};
        long long v = rnd(-100, 100);
        kd.insert(p, v);
        pts.push_back({p, v});
      } else if (o == 2) {
        int x1 = rnd(-40, 40), x2 = rnd(x1, 40);
        int y1 = rnd(-40, 40), y2 = rnd(y1, 40);
        Cfg::Pos l{x1, y1}, r{x2, y2};
        long long f = rnd(-100, 100);
        kd.update(l, r, f);
        for (auto& [p, v] : pts) {
          bool in = true;
          for (int i = 0; i < 2; ++i) if (p[i] < l[i] || r[i] < p[i]) in = false;
          if (in) v += f;
        }
      } else {
        int x1 = rnd(-40, 40), x2 = rnd(x1, 40);
        int y1 = rnd(-40, 40), y2 = rnd(y1, 40);
        Cfg::Pos l{x1, y1}, r{x2, y2};
        long long a = kd.query(l, r);
        long long b = brute_query<2, Cfg>(pts, l, r);
        if (a != b) { printf("2D MISMATCH round%d op%d: kd=%lld brute=%lld\n", round, t, a, b); abort(); }
      }
    }
    return pts.size();
  };
  // 连续多轮：验证 clear() 后可复用、旧数据不残留
  size_t total = 0;
  for (int round = 0; round < 5; ++round) {
    if (round) kd.clear();
    size_t got = run(round);
    total += got;
    if (kd.size() != got) { printf("size mismatch round%d\n", round); abort(); }
  }
  printf("2D + clear stress OK (total %zu points across rounds)\n", total);
}

void stress_min_2d() {
  using Cfg = MinCfg2D;
  KDT<Cfg> kd;
  vector<pair<Cfg::Pos, long long>> pts;
  mt19937 rng(7);
  auto rnd = [&](int L, int R) { return uniform_int_distribution<int>(L, R)(rng); };
  int ops = 1500;
  for (int t = 0; t < ops; ++t) {
    int o = rnd(1, 3);
    if (o == 1) {
      Cfg::Pos p{rnd(-60, 60), rnd(-60, 60)};
      long long v = rnd(0, 1000);
      kd.insert(p, v);
      pts.push_back({p, v});
    } else if (o == 2) {
      int x1 = rnd(-60, 60), x2 = rnd(x1, 60);
      int y1 = rnd(-60, 60), y2 = rnd(y1, 60);
      Cfg::Pos l{x1, y1}, r{x2, y2};
      long long f = rnd(0, 1000);
      kd.update(l, r, f);
      for (auto& [p, v] : pts) {
        bool in = true;
        for (int i = 0; i < 2; ++i) if (p[i] < l[i] || r[i] < p[i]) in = false;
        if (in) v = min(v, f);
      }
    } else {
      int x1 = rnd(-60, 60), x2 = rnd(x1, 60);
      int y1 = rnd(-60, 60), y2 = rnd(y1, 60);
      Cfg::Pos l{x1, y1}, r{x2, y2};
      long long a = kd.query(l, r);
      long long b = brute_query<2, Cfg>(pts, l, r);
      if (a != b) { printf("MIN MISMATCH op%d: kd=%lld brute=%lld\n", t, a, b); abort(); }
    }
  }
  printf("2D min-semigroup stress OK (%zu points)\n", kd.size());
}

// ========================= 性能 / 大样例冒烟 =========================
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
      kd.insert(p, rnd(-1000, 1000));
    } else if (o == 2) {
      int x1 = rnd(0, 100000), x2 = rnd(x1, 100000);
      int y1 = rnd(0, 100000), y2 = rnd(y1, 100000);
      int z1 = rnd(0, 100000), z2 = rnd(z1, 100000);
      kd.update(Cfg::Pos{x1, y1, z1}, Cfg::Pos{x2, y2, z2}, rnd(-100, 100));
    } else {
      int x1 = rnd(0, 100000), x2 = rnd(x1, 100000);
      int y1 = rnd(0, 100000), y2 = rnd(y1, 100000);
      int z1 = rnd(0, 100000), z2 = rnd(z1, 100000);
      acc += kd.query(Cfg::Pos{x1, y1, z1}, Cfg::Pos{x2, y2, z2}) + lst;
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
