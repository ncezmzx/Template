#include <bits/stdc++.h>
using namespace std;

// CDQ divide & conquer (3D partial order counting): pairs i < j with a_i <= a_j, b_i <= b_j, c_i <= c_j
// swap the contribution step for "how many points dominate each point", etc.
constexpr int N = 2e5 + 9;
struct Node {
  int a, b, c;
};
Node v_[N], tmp_[N];
int bit_[N], C_;
void bit_add(int x, int w) {
  for (; x <= C_; x += x & -x) bit_[x] += w;
}
long long bit_sum(int x) {
  long long r = 0;
  for (; x; x -= x & -x) r += bit_[x];
  return r;
}
long long ans_;

void cdq(int l, int r) {  // [l, r): v_ sorted by (a, original order)
  if (r - l <= 1) return;
  int m = (l + r) / 2;
  cdq(l, m), cdq(m, r);
  int p = l;
  for (int t = m; t < r; ++t) {  // left half's contribution to the right: 2D order on b, c
    while (p < m && v_[p].b <= v_[t].b) bit_add(v_[p].c, 1), ++p;
    ans_ += bit_sum(v_[t].c);
  }
  for (int t = l; t < p; ++t) bit_add(v_[t].c, -1);  // restore
  int i = l, j = m, k = l;  // merge by b (stable)
  while (i < m && j < r) tmp_[k++] = v_[i].b <= v_[j].b ? v_[i++] : v_[j++];
  while (i < m) tmp_[k++] = v_[i++];
  while (j < r) tmp_[k++] = v_[j++];
  copy(tmp_ + l, tmp_ + r, v_ + l);
}

// each pts entry {a, b, c} (c within [1, 1e9]; compressed internally)
long long count_3d(vector<array<int, 3>> pts) {
  int n = pts.size();
  vector<int> cs;
  for (auto& p : pts) cs.push_back(p[2]);
  sort(cs.begin(), cs.end()), cs.erase(unique(cs.begin(), cs.end()), cs.end());
  C_ = cs.size();
  for (int i = 0; i < n; ++i) v_[i] = {pts[i][0], pts[i][1], (int)(lower_bound(cs.begin(), cs.end(), pts[i][2]) - cs.begin()) + 1};
  // sort by full (a, b, c) order: for a comparable pair {u, v} (u <= v dimensionwise),
  // the dominator's lex order is always <= the dominated one's (equal triples comparable in any order); across levels only b, c matter
  stable_sort(v_, v_ + n, [](const Node& x, const Node& y) {
    return x.a != y.a ? x.a < y.a : (x.b != y.b ? x.b < y.b : x.c < y.c);
  });
  ans_ = 0;
  cdq(0, n);
  return ans_;
}

/*
 * ============================================================
 * Name: CDQ divide & conquer (3D partial order)
 * Complexity: O(n log^2 n) (divide levels x BIT)
 * Usage: count comparable pairs in a 3D partial order (classic: Luogu P3810
 *        asks per point how many points dominate it; this file is the total-
 *        count form — an unordered pair {u,v} counts once whenever u <= v
 *        dimensionwise); the CDQ idea generalizes to "left-half updates
 *        contribute to right-half queries" style 2D problems
 * Interface: count_3d({{a, b, c}, ...}) counts unordered pairs {u, v} whose
 *        min dominates the max dimensionwise (equal triples counted once in
 *        input order)
 * Principle: first dimension sorted by full (a, b, c) order (dominators
 *        always precede); second dimension handled by divide-and-conquer
 *        merging — during the merge the left half enters the BIT keyed by c
 *        (a already guaranteed by sorting), the right half queries prefix
 *        sums; O(n log n) per level, log n levels
 * Notes: to count "strictly distinct in all 3 dimensions", deduplicate equal
 *        triples first (combining counts inside a group separately); for
 *        mixed update/query CDQ, put the queries into v_ as elements too
 * ============================================================
 * Example (uncomment to compile):
 * signed main() {
 *   cout << count_3d({{1, 1, 1}, {2, 2, 2}, {3, 3, 3}}) << '\n';   // 3 (all comparable)
 *   cout << count_3d({{2, 1, 1}, {1, 2, 2}, {3, 3, 3}}) << '\n';   // 2 ((2,1,1)<(3,3,3) and (1,2,2)<(3,3,3); the first two are a-descending, not comparable)
 *   cout << count_3d({{1, 1, 1}, {1, 1, 1}}) << '\n';              // 1 (equal triples count once in input order)
 * }
 */
