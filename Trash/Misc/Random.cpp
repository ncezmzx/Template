// ============================================================
// 名称：随机工具（mt19937 + shuffle + 随机整数）与 splitmix64 快速随机哈希
// 复杂度：O(1) / 次
// 用途：随机化算法（模拟退火、随机增量、随机打乱、随机权值哈希）与抗卡哈希；
//       splitmix64 是著名的高质量快速随机哈希，配合 FIXED_RANDOM 种子可防针对性 hack
// 使用示例：见 #ifdef DEMO
// 来源：
//   - mt19937 rng(random_device{}())：all.cpp 2913（全文件 79 处同款，如 533、4242；
//     64 位版 mt19937_64 rng(random_device{}()) 见 all.cpp 37048）
//   - shuffle(v.begin(), v.end(), rng)：all.cpp 15391
//   - uniform_int_distribution<int>(l, r)(rng)：all.cpp 7320；
//     封装写法 int rg(int l,int r) 见 all.cpp 21664（double 版 21665）
//   - splitmix64_hash：all.cpp 41576-41588（原样提取；同款还出现在 41931-41943）
// ============================================================
#include <bits/stdc++.h>
using namespace std;

mt19937 rng(random_device{}());

// 来源：all.cpp 41576-41588（原样提取）
struct splitmix64_hash {
  static uint64_t splitmix64(uint64_t x) {
    x += 0x9e3779b97f4a7c15;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
  }

  size_t operator()(uint64_t x) const {
    static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
    return splitmix64(x + FIXED_RANDOM);
  }
};

// 来源：all.cpp 21664（原样提取）—— [l, r] 内均匀随机整数
int rg(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }

#ifdef DEMO
int main() {
  // 随机整数
  cout << rg(1, 100) << '\n';
  // 随机打乱
  vector<int> v = {1, 2, 3, 4, 5};
  shuffle(v.begin(), v.end(), rng);
  for (int x : v) cout << x << ' ';
  cout << '\n';
  // splitmix64 随机哈希
  splitmix64_hash h;
  cout << h(42) << '\n';
}
#endif
