#include <bits/stdc++.h>
using namespace std;

// Nimber multiplication (characteristic-2 field, pairs with XOR Nim-sum)
// 64-bit nimbers; divide-and-conquer by largest Fermat 2-power F(k)=2^{2^k}, depth <= 5
using ull = unsigned long long;

ull nim_prod(ull a, ull b) {
  if (a < b) swap(a, b);
  if (b == 0) return 0;
  if (b == 1) return a;
  int k = 0;
  while (k < 5 && (1ULL << (1 << (k + 1))) <= a) ++k; // largest k with F(k) <= a < F(k+1)
  ull m = 1ULL << (1 << k);                           // m = F(k)
  ull a1 = a >> (1 << k), a0 = a & (m - 1);           // a = a1*m xor a0
  ull b1 = b >> (1 << k), b0 = b & (m - 1);
  ull a1b1 = nim_prod(a1, b1);
  ull a0b0 = nim_prod(a0, b0);
  ull t = nim_prod(a1 ^ a0, b1 ^ b0) ^ a1b1 ^ a0b0; // t = a1*b0 xor a0*b1
  // a*b = a0b0 xor (t*m) xor (a1b1*(m*m)), with m*m = m xor (m>>1) (= 3m/2)
  return a0b0 ^ (t << (1 << k)) ^ (a1b1 << (1 << k)) ^ nim_prod(a1b1, m >> 1);
}

/*
 * ============================================================
 * Name: nim product (nimber multiplication)
 * Complexity: ~O(log^2 64) recursion (depth <= 5, constant splits per level)
 * Usage: multiply SG values of independent coin-turning subgames (2D Nim,
 *        products of coin-turning games); nimbers form a characteristic-2
 *        field, so with xor as the Nim sum
 *        arbitrary field operations work, including inverses (x (x) x^{-1} =
 *        1). Method list: see Interface below.
 * Interface: nim_prod(a, b), a and b 64-bit unsigned nimbers
 * Principle: with m = F(k) = 2^{2^k} the largest Fermat 2-power <= max(a, b),
 *            split a = a1*m (+) a0 and b = b1*m (+) b0; distributivity plus m
 *            (x) z = m*z for z < m and m (x) m = m (+) (m>>1)
 *            give a (x) b = a0b0 (+) (a1b0 (+) a0b1)*m (+) a1b1*(m (+) m>>1),
 *            each term recursing with a smaller k
 * Notes: inputs must be < 2^64; the Nim sum is bitwise xor; inverses come from
 *        fast exponentiation in the nimber field (a^{-1} = a^{2^{64}-2})
 * Source: OI-Wiki "Impartial games / Nim numbers"
 *         (https://oi-wiki.org/math/game-theory/impartial-game/)
 * ============================================================
 */
