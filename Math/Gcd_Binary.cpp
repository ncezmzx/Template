#define int long long

int gcd_bin(int a, int b) {
  if (!a) return b;
  if (!b) return a;
  int s = __builtin_ctzll(a | b);
  a >>= __builtin_ctzll(a), b >>= __builtin_ctzll(b);
  while (a != b) {
    if (a > b) a = (a - b) >> __builtin_ctzll(a - b);
    else b = (b - a) >> __builtin_ctzll(b - a);
  }
  return a << s;
}

