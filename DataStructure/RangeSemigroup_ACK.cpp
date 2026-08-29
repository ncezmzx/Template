#include <bits/stdc++.h>
using namespace std;

struct trip { int a, b, c; };
constexpr struct ACK_PRECALCER {
  constexpr static int A = 3, H = 30;
  int Ack[A][H] = {};
  trip pos[H][A] = {};
  int cnt[H] = {};
  constexpr ACK_PRECALCER() {
    for (int t = 0; t < H; ++t) Ack[0][t] = t + 1;   // hand-written iota (no constexpr iota in C++14)
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
      cnt[j] = t;   // the original span becomes a (pos, cnt) pair, usable in C++14
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
 * Name: static range semigroup queries (Ackermann-function blocking + jump table, uttree)
 * Complexity: preprocessing O(n); query O(alpha(n)) (inverse Ackermann, <= 4 in
 *             practice)
 * Usage: associative range queries on a static array (sum / max / gcd / xor,
 *        ...), answered online;
 *        O(n) preprocessing beats sparse table / cat tree O(n log n), queries
 *        are essentially O(1).
 * Principle: bottom-layer blocking at B = 2(A+1) = 8 with prefix / suffix sums,
 *            a zkw segment tree over the blocks, and a jump table over
 *            Ackermann heights (ACK_PRECALCER precomputes at compile time)
 * Source: Luogu article "Segment trees, Ackermann, blocking"
 *         (Luogu blog 2: segment trees, Ackermann, blocking); rewritten for C++14
 *         (span/requires/auto template params -> arrays + function pointers)
 * Notes: 0-indexed HALF-OPEN ranges: query(l, r) covers [l, r), so for the
 *        closed range [l, r] call query(l, r + 1);
 *        static only (no updates after build); op / e are plain function
 *        pointer template parameters; C++14+ (__lg / __int128 are GNU
 *        extensions)
 * ============================================================
 */
