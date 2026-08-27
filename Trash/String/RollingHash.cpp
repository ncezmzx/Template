// ============================================================
// 名称: 随机种子滚动哈希 / 可拼接哈希 (mod 2^61-1)
// 复杂度: O(n) 预处理, O(1) 子串查询 / 拼接
// 用途: 子串哈希比较、树/图上拼接两个哈希 (Hash(x·y) = Hash(x)*pw[|y|] + Hash(y))
// 使用示例: 见下方 #ifdef DEMO 的 main (字符串 1-indexed)
// 来源: all.cpp 行 51593-51638 (hash_t: 模板构造函数, 可直接用 char 构造)
//       all.cpp 行 51640-51649 (pw / hsh / query 前缀哈希)
// 拼接技巧 (来源 all.cpp 51674/51683 的滚动方式): 哈希是线性函数,
// 已知 Hash(a) 与 Hash(b) (b 长 len), 拼接串 a·b 的哈希为 Hash(a)*pw[len] + Hash(b);
// 若子串来自同一前缀哈希数组, 也可直接 query 拼出的区间。
// ============================================================
#include <bits/stdc++.h>
using namespace std;

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
  template <class T> constexpr hash_t(T H) : h{H % hmod} {}
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
const hash_t seed = uniform_int_distribution<u64>(hash_t::hmod >> 2, hash_t::hmod >> 1)(rng);
constexpr int N = 1e5 + 9;
hash_t hsh[N], pw[N];   // hsh 前缀哈希, pw 幂
hash_t query(int l, int r) {
  return hsh[r] - hsh[l - 1] * pw[r - l + 1];
}

#ifdef DEMO
signed main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  string s;
  cin >> s;
  s = ' ' + s;                       // 1-indexed
  pw[0] = 1;
  for (int i = 1; i < N; ++i) pw[i] = pw[i - 1] * seed;
  int m = s.size() - 1;
  for (int j = 1; j <= m; ++j) hsh[j] = hsh[j - 1] * seed + s[j];  // 滚动前缀哈希
  // 子串哈希 (1-indexed 闭区间)
  auto h1 = query(1, 3), h2 = query(4, m);
  cout << (h1 == h2) << '\n';        // 两个子串是否相等
  // 拼接: Hash(s[1..3] · s[4..m]) = h1 * pw[m - 3] + h2, 应等于 query(1, m)
  hash_t cat = h1 * pw[m - 3] + h2;
  cout << (cat == query(1, m)) << '\n';
  return 0;
}
#endif
