#include <bits/stdc++.h>
using namespace std;

mt19937 rng(random_device{}());

// splitmix64: high-quality fast random hash (with anti-hack time seed)
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

// uniform random integer in [l, r]
int rg(int l, int r) { return uniform_int_distribution<int>(l, r)(rng); }

/*
 * ============================================================
 * Name: random utilities (mt19937 + shuffle + random integer) and the
 *       splitmix64 fast random hash
 * Complexity: O(1) per call
 * Usage: randomized algorithms (simulated annealing, randomized incremental,
 *        shuffle, random weights) and anti-hack hashing; splitmix64 with the
 *        FIXED_RANDOM seed defeats targeted hacks. Kept as free functions /
 *        small structs: rg(l, r), shuffle(v.begin(), v.end(), rng),
 *        splitmix64_hash{}(x)
 * Source:
 *   - mt19937 rng(random_device{}()): all.cpp 2913 (79 occurrences file-wide,
 *     e.g. 533, 4242; the 64-bit mt19937_64 variant at all.cpp 37048)
 *   - shuffle(v.begin(), v.end(), rng): all.cpp 15391
 *   - uniform_int_distribution<int>(l, r)(rng): all.cpp 7320; wrapper
 *     int rg(int l,int r) from all.cpp 21664 (double version 21665)
 *   - splitmix64_hash: all.cpp 41576-41588 (extracted verbatim; same code at
 *     41931-41943)
 * ============================================================
 * Example (uncomment to compile; output is random per run):
 * int main() {
 *   // random integer
 *   cout << rg(1, 100) << '\n';
 *   // random shuffle
 *   vector<int> v = {1, 2, 3, 4, 5};
 *   shuffle(v.begin(), v.end(), rng);
 *   for (int x : v) cout << x << ' ';
 *   cout << '\n';
 *   // splitmix64 random hash
 *   splitmix64_hash h;
 *   cout << h(42) << '\n';
 * }
 * ============================================================
 */
