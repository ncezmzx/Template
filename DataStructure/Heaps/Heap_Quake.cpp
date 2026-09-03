#define int long long

template <size_t N> struct heap_quake {
  static constexpr size_t M = N << 1;
  static constexpr double alpha = .75;
  int a[N], vl[N], tp[N], pos[N], frm[M], d[M], fa[M];
  int ls[M], rs[M], stk[N], stkt, tot;
  int pre[M], nxt[M], bg[N], ed[N], bid[M], eid[M];
  vector<int> bel[N];
  void cmin(int &x, int y) { x > y && (x = y); }
  int newnode(int x, int i) { return tp[i] = vl[i] = x, bg[i] = ed[i] = eid[i] = bid[i] = pos[i] = frm[i] = i; }
  int top(int x) { return tp[x]; }
  int merge(int l, int r) {
    int x = stkt ? stk[stkt--] : (int)(N << 1) - (++tot), tl = frm[l], tr = frm[r];
    ls[x] = l, rs[x] = r, d[x] = d[l] + 1, fa[l] = fa[r] = x;
    if (vl[tl] < vl[tr]) frm[pos[tl] = x] = tl;
    else frm[pos[tr] = x] = tr;
    return bel[d[x]].push_back(x), x;
  }
  void add(int x, int l, int r) { nxt[ed[x]] = l, pre[l] = ed[x], eid[ed[x] = r] = x; }
  void join(int x, int y) { add(x, bg[y], ed[y]), cmin(tp[x], tp[y]); }
  void destroy(int x) {
    int u = pre[x], v = nxt[x], l = ls[x], r = rs[x];
    fa[l] = fa[r] = 0;
    pos[frm[x]] = frm[x] == frm[l] ? l : r;
    if (l && r) nxt[l] = r, pre[r] = l;
    else if (!l && !r) l = v, r = u;
    else if (!l) pre[r] = 0, l = r;
    else nxt[l] = 0, r = l;
    if (!u) (bg[bid[x]] = l) && (bid[l] = bid[x]);
    else (nxt[u] = l) && (pre[l] = u);
    if (!v) (ed[eid[x]] = r) && (eid[r] = eid[x]);
    else (pre[v] = r) && (nxt[r] = v);
    fa[stk[++stkt] = x] = pre[x] = nxt[x] = 0;
  }
  void decrease_key(int h, int p, int v) {
    cmin(tp[h], vl[p] = v);
    int f = fa[p = pos[p]];
    if (!f || vl[frm[f]] <= v) return;
    fa[p] = (p == ls[f] ? ls[f] : rs[f]) = 0;
    add(h, p, p);
  }
  void erase(int h, int x) {
    decrease_key(h, x, LLONG_MIN);
    while (int p = pos[x]) destroy(p);
    int mx = -1;
    for (int x = bg[h], p; x; x = p) {
      p = nxt[x], pre[x] = nxt[x] = fa[x] = 0;
      while (int &y = a[d[x]]) x = merge(x, exchange(y, 0));
      mx = max(mx, d[x]), a[d[x]] = x;
    }
    bg[h] = ed[h] = 0, tp[h] = LLONG_MAX;
    for (int i = 0; i <= mx; ++i)
      if (int x = exchange(a[i], 0)) {
        cmin(tp[h], vl[frm[x]]);
        if (!ed[h]) bg[h] = ed[h] = x, bid[x] = eid[x] = h;
        else add(h, x, x);
      }
    mx = 0;
    while (!bel[mx].empty()) ++mx;
    for (int i = 0; i < mx; ++i) {
      if (bel[i + 1].size() <= bel[i].size() * alpha) continue;
      while (mx-- > i) {
        for (int j : bel[mx]) destroy(j);
        bel[mx].clear();
      }
    }
  }
};

