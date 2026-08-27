#include <bits/stdc++.h>
using namespace std;

struct trip { int a, b, c; };
constexpr struct ACK_PRECALCER {
  constexpr static int A = 3, H = 30;
  int Ack[A][H] = {};
  trip pos[H][A] = {};
  int cnt[H] = {};
  constexpr ACK_PRECALCER() {
    for (int t = 0; t < H; ++t) Ack[0][t] = t + 1;   // 手写 iota（C++14 无 constexpr iota）
    for (int i = 1; i < A; ++i)
      for (int j = 0; j < H; ++j)
        for (int T = j + 1, &x = Ack[i][j] = j; T && x < H; --T)
          x = Ack[i - 1][x];
    for (int j = 1; j < H; ++j) {
      int i = 1, x = A - 1, t = 0;
      while (i < j) {
        while (Ack[x][i] > j) --x;
        int k = i - 1, c = -1;
        while (Ack[x][i] <= j) i = Ack[x][i], ++c;
        pos[j][t++] = trip{k, x, c};
      }
      cnt[j] = t;   // 原 span 改为 (pos, cnt) 二元组，C++14 可用
    }
  }
} Ack;
template <class S, S (*op)(S, S), S (*e)()>
class uttree {
  constexpr static int A = ACK_PRECALCER::A, B = (A + 1) << 1;
  typedef pair<S, S> S_p;
  static S_p op_p(S_p x, S_p y) {
    return S_p(op(x.first, y.first), op(y.second, x.second));
  }
  vector<S> val, pre, suf;
  vector<array<vector<S_p>, A>> tog;

 public:
  void build(const vector<S>& v) {
    int n = v.size();
    val = pre = suf = v;
    for (int i = 1; i < n; ++i)
      if (i % B) pre[i] = op(pre[i - 1], pre[i]);
    for (int i = n - 1; i; --i)
      if (i % B) suf[i - 1] = op(suf[i - 1], suf[i]);
    if (n <= (B << 1)) return;
    int N = (n - 1) / B, H = __lg(N) + 1, M = 1 << H++;
    vector<S> zkw(M << 1, e());
    for (int i = 1, j = B; i < N; ++i, j += B)
      zkw[i | M] = accumulate(val.data() + j, val.data() + j + B, e(), op);
    for (int i = M - 1; i; --i) zkw[i] = op(zkw[i << 1], zkw[i << 1 | 1]);
    tog.resize(M << 1);
    for (int h = H - 2, s = 4; s <= M; s <<= 1, --h)
      for (int x = s; x < (s << 1); ++x)
        for (int i = 0; i < A; ++i)
          for (int y = h, c = 0; (y = Ack.Ack[i][y]) < H && c < h; ++c)
            tog[x][i].push_back(
                c ? op_p(tog[x][i][0], tog[x >> (Ack.Ack[i][h] - h)][i][c - 1])
                  : i ? op_p(tog[x][i - 1][0],
                             tog[x >> (Ack.Ack[i - 1][h] - h)][i - 1][h - 1])
                      : x & 1 ? S_p(e(), zkw[x ^ 1])
                              : S_p(zkw[x ^ 1], e()));
  }
  S query(int l, int r) const {
    if (l / B >= --r / B)
      return accumulate(val.data() + l, val.data() + r + 1, e(), op);
    S L = suf[l], R = pre[r];
    int M = (int)tog.size() >> 1;
    if ((l = l / B | M) + 1 < (r = r / B | M)) {
      int h = __lg(l ^ r) + 1;
      for (int t = 0; t < Ack.cnt[h]; ++t) {
        int j = Ack.pos[h][t].a, x = Ack.pos[h][t].b, y = Ack.pos[h][t].c;
        L = op(L, tog[l >> j][x][y].first), R = op(tog[r >> j][x][y].second, R);
      }
    }
    return op(L, R);
  }
  uttree() {}
  explicit uttree(const vector<S>& v) { build(v); }
};

/*
 * ============================================================
 * 名称：静态区间半群查询（阿克曼函数分块 + 跳跃表，uttree）
 * 复杂度：预处理 O(n)，单次查询 O(α(n))（α 为反阿克曼函数，实际 ≤ 4）
 * 用途：静态数组上的区间可结合查询（op 需满足结合律，如和/最值/gcd/xor），
 *       在线回答；预处理 O(n) 比 ST 表 O(n log n)、猫树 O(n log n) 更省，
 *       查询 O(α(n)) 近似 O(1)
 * 原理：底层按 B = 2(A+1) = 8 分块做前缀/后缀和；块间建 zkw 线段树；
 *       对"高度"用阿克曼函数递推跳表（ACK_PRECALCER 编译期预处理，
 *       只算 c = 1 的阿克曼值并截断上界），查询时按 pth 贪心拆分前缀，
 *       拼出区间答案（本质是带反阿克曼加速的倍增）
 * 来源：洛谷文章《线段树，阿克曼，分块》(luogu_blog_2_线段树阿克曼分块.md)；
 *       已改写为 C++14 兼容（span/requires/auto 模板参 → 数组+函数指针）
 * 注意：C++14 及以上可编译（__lg/__int128 为 GNU 扩展，g++ 下可用）；
 *       op/e 以普通函数指针传入模板；下标 0-indexed，
 *       query(l, r) 为**半开区间 [l, r)**（右端点不包含，与文章代码一致）
 *       —— 查询 [l, r] 闭区间请调用 query(l, r + 1)；
 *       只支持静态查询（build 后不可修改）
 * ============================================================
 * 使用示例（编译时取消注释；区间和）：
 * long long op(long long a, long long b) { return a + b; }
 * long long e() { return 0; }
 * signed main() {
 *   uttree<long long, op, e> st;
 *   st.build({1, 3, 5, 2, 4, 6});
 *   cout << st.query(1, 3) << '\n';  // [1,3) = 3+5 = 8
 *   cout << st.query(0, 6) << '\n';  // [0,6) = 21
 *   cout << st.query(2, 3) << '\n';  // 5
 * }
 * ============================================================
 */
