
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

