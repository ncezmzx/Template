#include <bits/stdc++.h>
using namespace std;

constexpr int N = 5e4 + 9;
mt19937_64 rng(random_device{}());
using u64 = unsigned long long;
using i128 = __int128;
const u64 msk = rng();
struct hash_t {
  static u64 shift(u64 x) {
    x ^= msk;
    x ^= (x << 5ull);
    x ^= (x >> 11ull);
    x ^= (x << 54ull);
    return x ^ msk;
  }
  static constexpr u64 hmod = (1LL << 61) - 1;
  u64 h;
  constexpr hash_t() : h{0} {}
  constexpr hash_t(u64 H) : h{H % hmod} {}
  hash_t& operator+=(hash_t rhs) {
    h += rhs.h, h >= hmod && (h -= hmod);
    return *this;
  }
  hash_t& operator-=(hash_t rhs) {
    h += hmod - rhs.h, h >= hmod && (h -= hmod);
    return *this;
  }
  hash_t& operator*=(hash_t rhs) {
    i128 rt = (i128) h * rhs.h;
    h = u64(rt & hmod) + u64(rt >> 61);
    if (h >= hmod) h -= hmod;
    return *this;
  }
  friend hash_t operator+(hash_t lhs, const hash_t& rhs) {
    return lhs += rhs;
  }
  friend hash_t operator-(hash_t lhs, const hash_t& rhs) {
    return lhs -= rhs;
  }
  friend hash_t operator*(hash_t lhs, const hash_t& rhs) {
    return lhs *= rhs;
  }
  bool operator==(const hash_t& rhs) const {
    return h == rhs.h;
  }
  bool operator<(const hash_t& rhs) const {
    return h < rhs.h;
  }
};
hash_t pw[N];
const hash_t seed = uniform_int_distribution<u64>(hash_t::hmod >> 2, hash_t::hmod >> 1)(rng);
struct strhash {
  int n;
  hash_t h[N];
  void init(int m, string &a) {
    n = m, pw[0] = 1;
    for (int i = 1; i <= m; ++i) pw[i] = pw[i - 1] * seed;
    for (int i = 1; i <= m; ++i) h[i] = h[i - 1] * seed + hash_t::shift(a[i]);
  }
  hash_t query(int l, int r) const {
    return h[r] - h[l - 1] * pw[r - l + 1];
  }
} str;
/*
 * ============================================================
 * 名称：字符串哈希（mod 2^61-1，随机种子，shift 混淆）
 * 复杂度：O(n) 预处理，O(1) 子串查询
 * 用途：子串哈希快速比较（判等 / 排序），随机化防卡；字符串需 1-indexed（s = ' ' + s）。
 * 来源：all.cpp 行 53386-53446（原样保留；注释已统一移至文件尾部）
 * ============================================================
 * 使用示例（编译时取消注释）：
 * signed main() {
 *   cin.tie(nullptr)->sync_with_stdio(false);
 *   string s;
 *   cin >> s;
 *   s = ' ' + s;                          // 1-indexed
 *   str.init(s.size() - 1, s);            // 预处理
 *   int l1, r1, l2, r2;
 *   cin >> l1 >> r1 >> l2 >> r2;          // 两个子串区间 (1-indexed)
 *   cout << (str.query(l1, r1) == str.query(l2, r2)) << '\n';  // 两子串是否相等
 *   return 0;
 * }
 * ============================================================
 */
