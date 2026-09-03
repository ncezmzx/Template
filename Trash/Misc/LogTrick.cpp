
struct log_trick_odt {
  map<int, int> mp;
  void split(int x) {
    auto it = prev(mp.upper_bound(x));
    mp[x] = it->second;
  }
  void assign(int l, int r, int x) {
    split(l), split(r + 1);
    auto it = mp.find(l);
    while (it->first != r + 1) it = mp.erase(it);
    mp[l] = x;
  }
  void perform(int l, int r, int x) {
    split(l), split(r + 1);
    auto it = mp.find(l);
    while (it->first != r + 1) {
      if (it->second > x) it->second = __gcd(it->second, x);
      it = next(it);
    }
  }
};

