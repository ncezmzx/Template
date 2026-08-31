#include <bits/stdc++.h>
using namespace std;

constexpr int K = 11;
int G[1 << K][1 << K];

void init_gcd() {
  for (int i = 0; i < (1 << K); ++i)
    for (int j = 0; j < (1 << K); ++j) G[i][j] = __gcd(i, j);
}

int gcd_tab(int a, int b) {
  if (!a) return b;
  if (!b) return a;
  int s = __builtin_ctz(a | b);
  a >>= __builtin_ctz(a), b >>= __builtin_ctz(b);
  while (a != b) {
    if (a > b) {
      if (b < (1 << K)) return G[a % b][b] << s;
      a = (a - b) >> __builtin_ctz(a - b);
    }
    else {
      if (a < (1 << K)) return G[b % a][a] << s;
      b = (b - a) >> __builtin_ctz(b - a);
    }
  }
  return a << s;
}

/*
 * ============================================================
 * Name: O(1) GCD by lookup table (precomputed small table + early exit in binary GCD)
 * Complexity: preprocessing O(2^K * 2^K) (~4M entries at K = 11, ~16MB of int);
 *             queries amortized O(1)
 * Usage: scenarios with huge numbers of gcd calls (1e6+): when the smaller
 *        operand is < 2^K one lookup answers directly, otherwise it degrades to
 *        a few binary-GCD iterations.
 *        init_gcd(), then gcd_tab(a, b).
 * Principle: gcd(a, b) = gcd(a mod b, b); if b < 2^K then a mod b < 2^K as
 *            well, so both are inside the table and one lookup suffices; ctz
 *            strips factors of two so both sides are odd
 * Notes: this implementation targets 32-bit non-negative integers
 *        (__builtin_ctz); for 64-bit use Gcd_Binary.cpp, or raise K and switch
 *        to __builtin_ctzll (memory grows exponentially in K)
 * ============================================================
 */
