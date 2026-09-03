using u64 = unsigned long long;
using i64 = long long;

template <typename T> u64 custom_hash(const T &x);

constexpr u64 HM0 = 0x9e3779b97f4a3c15ull, HM1 = 0xc2b2ae3d27d4eb4full;
template <> u64 custom_hash<i64>(const i64 &x) { return static_cast<u64>(x + HM0) * HM1; }
template <> u64 custom_hash<u64>(const u64 &x) { return (x ^ (x >> 33)) * HM1; }
template <> u64 custom_hash<string>(const string &s) {
  u64 h = 0x9e3779b97f4a3c15ull;
  for (char c : s) h = (h * 1000003ull) ^ static_cast<u64>(c);
  return h;
}

template <typename K, typename V, int B = 20> class HashMap {
  static constexpr int N = 1 << B;
  K *key{new K[N]};
  V *val{new V[N]};
  bool *vist{new bool[N]};
  int sz = 0;

  int idx(const K &x) const {
    int i = custom_hash(x) >> (64 - B);
    while (vist[i] && key[i] != x) {
      i = (i + 1) & (N - 1);
    }
    return i;
  }

  public:
  HashMap() { memset(vist, 0, N * sizeof(bool)); }
  ~HashMap() {
    delete[] key;
    delete[] val;
    delete[] vist;
  }
  HashMap(const HashMap &) = delete;
  HashMap &operator=(const HashMap &) = delete;
  int size() const { return sz; }
  void clear() { memset(vist, 0, N * sizeof(bool)), sz = 0; }
  bool contains(const K &x) const { return vist[idx(x)]; }
  V &operator[](const K &x) {
    int i = idx(x);
    if (!vist[i]) {
      vist[i] = true;
      key[i] = x;
      val[i] = V{};
      ++sz;
    }
    return val[i];
  }
  void erase(const K &x) {
    int i = idx(x);
    if (!vist[i]) return;
    vist[i] = false, --sz;
    for (int j = i;;) {
      j = (j + 1) & (N - 1);
      if (!vist[j]) break;
      int h = int(custom_hash(key[j]) >> (64 - B));
      bool must_pass = (i < j) ? (h > i && h <= j) : (h > i || h <= j);
      if (!must_pass) {
        key[i] = key[j], val[i] = val[j], vist[i] = true, vist[j] = false;
        i = j;
      }
    }
  }
};

