#define int long long

template <size_t N> struct manacher {
  int p[2 * N];
  string t;
  int m;

  int build(const string &s) {
    t = "&|";
    for (char c : s) t += c, t += '|';
    t += '?';
    m = (int)t.size() - 1;
    int ans = 0;
    for (int i = 1, mid = 0, r = 0; i <= m; ++i) {
      p[i] = (i > r ? 1 : min(r - i + 1, p[2 * mid - i]));
      while (t[i + p[i]] == t[i - p[i]]) ++p[i];
      if (p[i] + i - 1 > r) mid = i, r = p[i] + i - 1;
      ans = max(ans, p[i]);
    }
    return ans - 1;
  }
};

