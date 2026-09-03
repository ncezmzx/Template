#include <bits/stdc++.h>
using namespace std;
#define int long long


template <size_t N> struct leftist_heap {
  int tot;
  int val[N], dis[N], ch[N][2], fa[N];
  int node(int v) {
    int x = ++tot;
    val[x] = v, dis[x] = 1, fa[x] = x;
    return x;
  }
  int merge(int a, int b) {
    if (!a) return b;
    if (!b) return a;
    if (val[a] > val[b]) swap(a, b);
    ch[a][1] = merge(ch[a][1], b);
    if (ch[a][1]) fa[ch[a][1]] = a;
    if (dis[ch[a][0]] < dis[ch[a][1]]) swap(ch[a][0], ch[a][1]);
    dis[a] = dis[ch[a][1]] + 1;
    return a;
  }
  int find(int x) {
    while (fa[x] != x) x = fa[x];
    return x;
  }
  int pop(int x) {
    int l = ch[x][0], r = ch[x][1];
    ch[x][0] = ch[x][1] = 0, dis[x] = 1;
    if (l) fa[l] = l;
    if (r) fa[r] = r;
    return merge(l, r);
  }
};

