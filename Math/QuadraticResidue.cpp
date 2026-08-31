#include <bits/stdc++.h>
using namespace std;
#define int long long

int pw(int x, int n, int p) {
  int r = 1 % p;
  x %= p;
  while (n) {
    if (n & 1) r = r * x % p;
    x = x * x % p, n >>= 1;
  }
  return r;
}

// Cipolla: solves x^2 = a (mod p), p an odd prime; returns the smaller root, -1 if none
struct cpx { // "complex" number x + y*sqrt(w) over F_p
  int x, y;
};
int W; // square of sqrt(w) (w = b^2 - a, a non-residue)
cpx cmul(const cpx &a, const cpx &b, int p) {
  return {(a.x * b.x + a.y * b.y % p * W) % p, (a.x * b.y + a.y * b.x) % p};
}
int sqrt_mod(int a, int p) {
  a %= p;
  if (a == 0) return 0;
  if (pw(a, (p - 1) / 2, p) != 1) return -1; // Euler criterion
  int b;
  for (b = 1;; ++b) { // find b with w = b^2 - a a non-residue (expected 2 tries)
    W = ((b * b - a) % p + p) % p;
    if (W && pw(W, (p - 1) / 2, p) == p - 1) break;
  }
  cpx r{1, 0}, c{b, 1};
  for (int e = (p + 1) / 2; e; e >>= 1) { // (b + sqrt(w))^{(p+1)/2}; imaginary part is 0
    if (e & 1) r = cmul(r, c, p);
    c = cmul(c, c, p);
  }
  return min(r.x, p - r.x);
}

/*
 * ============================================================
 * Name: quadratic residues (Cipolla square root mod p)
 * Complexity: expected O(log^2 p) (finding b takes ~2 tries)
 * Usage: sqrt_mod(a, p) finds x with x^2 = a (mod p) for an odd prime p,
 *        returning the smaller root, or -1 when a is a non-residue; a = 0 gives
 *        0.
 * Principle: Euler's criterion a^{(p-1)/2} = 1 decides solvability; pick b with
 *            w = b^2 - a a non-residue, then in F_p(sqrt w) Frobenius gives (b
 *            + sqrt w)^p = b - sqrt w, so (b + sqrt w)^{p+1} = b^2 - w = a and
 *            its (p+1)/2-th power is the root
 * Notes: odd primes only (for p = 2 the root is a mod 2); the two roots are x
 *        and p - x; 0 has only the root 0
 * ============================================================
 */
