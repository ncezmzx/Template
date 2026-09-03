
using ull = unsigned long long;

ull nim_prod(ull a, ull b) {
  if (a < b) swap(a, b);
  if (b == 0) return 0;
  if (b == 1) return a;
  int k = 0;
  while (k < 5 && (1ULL << (1 << (k + 1))) <= a) ++k;
  ull m = 1ULL << (1 << k);
  ull a1 = a >> (1 << k), a0 = a & (m - 1);
  ull b1 = b >> (1 << k), b0 = b & (m - 1);
  ull a1b1 = nim_prod(a1, b1);
  ull a0b0 = nim_prod(a0, b0);
  ull t = nim_prod(a1 ^ a0, b1 ^ b0) ^ a1b1 ^ a0b0;

  return a0b0 ^ (t << (1 << k)) ^ (a1b1 << (1 << k)) ^ nim_prod(a1b1, m >> 1);
}

