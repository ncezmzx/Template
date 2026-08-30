# OI Template Library (curated)

A curated collection of competitive-programming templates.

## Directory layout

```
Template/
├── DataStructure/   data structures (balanced trees / heaps / trees / segment-tree variants / bitset / tries)
│   └── Heaps/       16 mergeable-heap implementations
├── Graph/           graph algorithms (flows / Tarjan family / tree tricks / counting)
├── String/          strings (palindromes / suffix structures / hashing / Z function)
├── Math/            math (modular arithmetic / number theory / big ints / transforms / polynomials / floor sums / combinatorics / recurrences / integration)
├── Geometry/        computational geometry (2D basics / half-plane intersection / minimum enclosing circle)
├── Misc/            techniques & miscellaneous
└── Trash/           removed common/basic/duplicate templates (kept for reference)
```

119 templates (DataStructure 46 / Graph 22 / String 8 / Math 29 / Misc 11 /
Geometry 3) + Trash 23.

## Index

### DataStructure/ (46, incl. Heaps/ 16)

| File | Content |
|---|---|
| `BalancedTree_WBST_Seq.cpp` | weight-balanced BST, sequence form (deterministic, lazy reverse; implicit-treap replacement) — `wbst_seq<N>` |
| `BalancedTree_WBST_Set.cpp` | weight-balanced BST, set form (ordered multiset: ins/remove/rnk/kth) — `wbst_set<N>` |
| `BalancedTree_Treap.cpp` | fhq-Treap (split/merge by value or by size) — `treap<N>` |
| `BalancedTree_Splay.cpp` | splay tree (sequence operations: range reverse) — `splay<N>` |
| `LeftistHeap.cpp` | leftist heap (mergeable min-heap, O(log n) merge) — `leftist_heap<N>` |
| `CartesianTree.cpp` | Cartesian tree (monotonic-stack O(n) build; RMQ / largest rectangle) — `cartesian_tree<N>` |
| `CatTree.cpp` | cat tree (static associative range query O(1), via operator+) — `cattree<T, N>` |
| `ODT.cpp` | Chtholly tree / ODT (map-based interval assignment) — `odt_tree` |
| `SegmentTreeGCD.cpp` | gcd segment tree (point update + prefix gcd) — `segtree_gcd<N>` |
| `SegmentTreeBeats.cpp` | Segment Tree Beats (range add / chmin / chmax / sum & extrema) — `segbeats` |
| `SegmentTree_Semigroup.cpp` | semigroup lazy segment tree (dynamic nodes, fully iterative; ACL-aligned get/all_prod/max_right/min_left + O(n) vector build) — `semigroup_segtree<S, op, e, F, mapping, composition, id>` |
| `SegmentTree_IterativeLazy.cpp` | iterative (zkw-style) segment tree (range add + range sum, lazy tags) — `segtree_iterative_lazy<N>` |
| `SegmentTree_ACL.cpp` | lazy segment tree, ACL style (static full tree, generic op/mapping; supports segment-length-dependent actions such as range affine + range sum; max_right/min_left) — `lazy_segtree<...>` |
| `SegmentTree_LiChao.cpp` | Li Chao segment tree (line/segment insert, point minimum query) — `lichao_segtree<N, SP>` |
| `PersistentDSU.cpp` | persistent DSU (segment trees for fa/siz + union by size, historical versions) — `persistent_dsu<N, MV>` |
| `PersistentTreap.cpp` | persistent balanced tree (fully persistent fhq-Treap) — `persistent_treap<N>` |
| `SegmentTree_Merge.cpp` | segment tree merging (dynamic value-segment trees) — `segtree_merge<SP>` |
| `Trie_Binary.cpp` | persistent 01-Trie (xor extrema / rank over a version range, no compression needed) — `persistent_binary_trie<N, SP>` |
| `DSU_Weighted_Rollback.cpp` | weighted DSU (relative constraints) + rollback DSU (for divide-&-conquer offline) — `wdsu`, `rdsu` |
| `Bitset_Dynamic.cpp` | hand-written dynamic bitset (std::bitset operations + runtime resize) — `dynbitset` |
| `YFastTrie.cpp` | y-fast trie (x-fast trie + blocking; O(log 64) find/pred/succ/insert/erase) — `yfast` |
| `RangeSemigroup_ACK.cpp` | static range semigroup queries (Ackermann-function blocking, O(n) preprocessing, O(α(n)) query) — `uttree<...>` |
| `MonoidOfflineQuery.cpp` | offline static range monoid products (cat-tree divide, O((n+q) log n) total / O(1) per query) — `monoid_product<Mono, F>` |
| `RMQ_Linear.cpp` | static RMQ / FastST (in-block Cartesian-tree bitmask + sparse table over block extrema, O(1) query) — `FastST<U, Z, T>` |
| `SqrtTree.cpp` | sqrt tree (static associative range query, generic monoid, O(log log n)) — `SqrtTree<S, op, e, N>` |
| `LCT_Monoid.cpp` | generic-monoid LCT (any invertible monoid path product) — `LinkCutTree<Mono, N>` |
| `GlobalBST.cpp` | global balanced BST (static-tree path add / path max / point set, O(log n)) — `global_bst<N>` |
| `TopTree.cpp` | static top tree (cluster decomposition; path query + subtree cluster query) — `top_tree<N>` |
| `WaveletMatrix_Dynamic.cpp` | dynamic Wavelet Matrix via binary grouping (insert / global k-th / rank) — `dyn_wavelet` |
| `KDT_DynamicSemigroup.cpp` | generic K-D tree (binary-grouping dynamic insertion + lazy tags; custom dual-semigroup Info/Tag, arbitrary dimension K, clear/reuse; self-tests included; C++17) — `KDT<Cfg>` |
| `Heaps/` | 16 mergeable-heap implementations (see below) |

#### Heaps/ (16, uniform interface: `newnode/top/join/decrease_key/erase`, min-heaps, heap = root index)

| File | Implementation |
|---|---|
| `Heap_PBDS.cpp` | pairing heap via `__gnu_pbds::priority_queue` — `pbds_heap<N>` |
| `Heap_Leftist.cpp` | leftist heap (O(log n) merge) — `heap_leftist<N>` |
| `Heap_Skew.cpp` | skew heap (amortized O(log n)) — `heap_skew<N>` |
| `Heap_Binomial.cpp` | binomial heap (no amortized-constant insert) — `heap_binomial<N>` |
| `Heap_Pairing.cpp` | pairing heap (amortized O(log n)) — `heap_pairing<N>` |
| `Heap_RankPairing_A.cpp` | rank-pairing heap, half-tree form (reference: known defects in decrease_key/erase) — `heap_rank_pairing_a<N>` |
| `Heap_RankPairing_B.cpp` | rank-pairing heap, multi-way form (reference: known defects) — `heap_rank_pairing_b<N>` |
| `Heap_Fibonacci.cpp` | Fibonacci heap (expected O(1) insert/decrease-key) — `heap_fibonacci<N>` |
| `Heap_Hollow.cpp` | hollow heap — `heap_hollow<N>` |
| `Heap_BinaryExt.cpp` | STL binary heap extension (lazy deletion + fast-push buffers) — `heap_binary_ext<N>` |
| `Heap_Thin.cpp` | thin heap (reference: known defects) — `heap_thin<N>` |
| `Heap_LazyBinomial.cpp` | lazy binomial heap (O(1) join) — `heap_lazy_binomial<N>` |
| `Heap_OneRootFibonacci.cpp` | one-root Fibonacci heap — `heap_one_root_fibonacci<N>` |
| `Heap_Quake.cpp` | quake heap — `heap_quake<N>` |
| `Heap_Slim.cpp` | slim heap (reference: known defects) — `heap_slim<N>` |
| `Heap_WeightLeftist.cpp` | weight-leftist heap (merges by subtree size) — `heap_weight_leftist<N>` |

> Verified by randomized stress tests: Leftist, WeightLeftist, Skew, Pairing,
> Binomial, Fibonacci, Hollow, LazyBinomial, OneRootFibonacci, Quake and PBDS
> work correctly; **RankPairing_A, RankPairing_B, Thin, Slim and BinaryExt
> carry the original article's known defects (decrease_key/erase paths) and
> are marked reference implementations**. Global array sizes (N, M) are
> template parameters — tune per problem.

### Graph/ (22)

| File | Content |
|---|---|
| `Dinic.cpp` | max flow (vector-based `dinic<T>`, current-arc optimization) |
| `MCMF.cpp` | min-cost max-flow (SPFA + push along shortest paths) — `mcmf` |
| `Flow_Bounds.cpp` | flows with lower bounds (feasible / max / min flow, Dinic-based) — `bounded_flow<N, M>` |
| `HopcroftKarp.cpp` | bipartite maximum matching, O(E√V) — `hopcroft_karp<N>` |
| `KM.cpp` | bipartite maximum-weight perfect matching, O(n³), slack optimization — `km_matching<N>` |
| `TarjanSCC.cpp` | strongly connected components + condensation — `scc<N>` |
| `TwoSat.cpp` | 2-SAT (implication graph + iterative Tarjan SCC; ACL twosat interface: add_clause/satisfiable/answer + if/not/xor/eq) — `two_sat` |
| `TarjanCutVertex.cpp` | articulation points (low[y] >= dfn[x], root special case) — `cut_vertex<N>` |
| `TarjanBridge.cpp` | bridges / edge-biconnected components (edge-id handling of multi-edges) — `edcc<N>` |
| `TarjanBCC.cpp` | vertex-biconnected components / block-cut tree — `block_cut_tree<N>` |
| `PointDivide.cpp` | centroid decomposition / dynamic centroid tree (centroid + double BIT inclusion-exclusion; lazy LCA table) — `centroid_tree<N>` |
| `KruskalWQS.cpp` | WQS binary search + Kruskal (MST with exactly k special edges) — `wqs_mst<N>` |
| `DominatorTree.cpp` | dominator tree (Lengauer-Tarjan) — `dominator_tree<N>` |
| `KruskalRebuildTree.cpp` | Kruskal rebuild tree (bottleneck paths / weight-limited connectivity) — `kruskal_rebuild_tree<N, M>` |
| `TriangleCount.cpp` | triangle counting (orientation trick, O(m√m)) — `triangle_count<N, M>` |
| `SPFA.cpp` | SPFA shortest paths (SLF; negative-cycle detection + difference constraints) — `spfa<N, M>` |
| `EulerPath.cpp` | Euler path / circuit (iterative Hierholzer, undirected + directed) — `euler_undirected`, `euler_directed` |
| `GeneralMatching.cpp` | general graph maximum matching (blossom, O(n³)) — `Blossom` |
| `DirectedMST.cpp` | minimum directed spanning tree (Chu-Liu/Edmonds, O(nm)) — `zhu_liu(...)` |
| `MatrixTree.cpp` | Matrix-Tree theorem (undirected spanning trees / directed arborescences, mod 998244353) — `count_spanning` / `count_arborescence` |
| `StoerWagner.cpp` | Stoer-Wagner global minimum cut (undirected positive weights) — `stoer_wagner<N>` |
| `SteinerTree.cpp` | minimum Steiner tree (subset DP + Dijkstra) — `steiner_tree(...)` |

### String/ (8)

| File | Content |
|---|---|
| `Manacher.cpp` | palindromes (longest palindromic substring; counting variants) — `manacher<N>` |
| `SuffixArray.cpp` | suffix array (doubling + radix sort) + height — `suffix_array<N>` |
| `SuffixAutomaton.cpp` | suffix automaton (endpos counts / distinct substrings) — `suffix_automaton<N>` |
| `StringHash.cpp` | string hashing (mod 2^61-1, random seed + xorshift scrambling) — `strhash` |
| `ZAlgorithm.cpp` | Z function / extended KMP (ACL-aligned, string + generic-container versions, O(n)) — `z_algorithm` |
| `PalindromicTree.cpp` | palindromic tree / eertree (distinct palindromes + occurrence counts, O(n)) — `palindromic_tree<N>` |
| `Lyndon.cpp` | Lyndon factorization (Duval) + minimal representation — `duval` / `min_representation` |
| `ACAutomaton.cpp` | Aho-Corasick automaton (multi-pattern matching; Trie + fail + topological counting) — `ac_automaton<N>` |

### Math/ (29)

| File | Content |
|---|---|
| `ModInt.cpp` | Montgomery modint wrapper (fixed modulus) — `mint = modint<md>` |
| `ModInt_Full.cpp` | full modint (Montgomery reduction: + - * / % pow inv sqrt comparisons IO) — `modint<m>` |
| `DynamicModInt.cpp` | Barrett dynamic-modulus modint (runtime set_mod) — `dynamic_modint` |
| `BigInt.cpp` | big integers (base 2^32, + - * / % pow gcd, Knuth D division, truncation toward zero) — `bigint` |
| `MatrixMaxPlus.cpp` | (max,+) matrix fast exponentiation — `matrix` |
| `MillerRabin.cpp` | Miller-Rabin primality (7 deterministic bases, 64-bit) — `MillerRabin` |
| `PollardRho.cpp` | Pollard-Rho factorization (pairs with Miller-Rabin) — `factorize` |
| `Gcd_Binary.cpp` | binary GCD (__builtin_ctz accelerated) — `gcd_bin` |
| `Gcd_Table.cpp` | O(1) GCD by lookup table (2^11 precomputed table + early exit) — `gcd_tab` |
| `FWT.cpp` | fast Walsh-Hadamard transform (or / and / xor convolutions) — `fwt_or/and/xor` |
| `GaussJordan.cpp` | Gauss-Jordan elimination over a prime field (solve / rank; double version commented) — `gauss_jordan<N>` |
| `LagrangeInterpolation.cpp` | Lagrange interpolation (general O(n²) + consecutive-nodes O(n)) — `lagrange` / `lagrange_1n` |
| `NTT.cpp` | NTT + polynomial toolkit (convolution / inv / ln / exp / deri / inte; fastest for the fixed modulus) — `namespace Poly` |
| `Poly_fstdlib.cpp` | fstdlib polynomial library (fixed-modulus NTT + arbitrary-modulus MTT `m_poly` / inv / log / exp; more features) — `namespace fstdlib` |
| `Min25.cpp` | Min25 sieve (multiplicative prefix sums, polynomial f(p)) — `Min25<T, M, F>` |
| `CRT.cpp` | CRT / exCRT (merges non-coprime moduli, ACL-aligned; {0,0} when unsolvable; exgcd/inv_gcd included) — `crt` |
| `FloorSum.cpp` | Euclidean floor_sum (Σ⌊(ai+b)/m⌋, ACL-aligned, negative a/b, O(log)) — `floor_sum` |
| `XORBasis.cpp` | xor linear basis (insert / representability / extrema / k-th smallest / ≤x count / merge) — `xor_basis` |
| `DiscreteLog.cpp` | BSGS / exBSGS discrete logarithm (minimal solution, a^{-k} giant steps) — `bsgs` / `exbsgs` |
| `QuadraticResidue.cpp` | Cipolla (modular square root for odd primes) — `sqrt_mod` |
| `PrimitiveRoot.cpp` | primitive roots (smallest / all) — `min_primitive_root` / `primitive_roots` |
| `Lucas.cpp` | Lucas / exLucas (large binomials mod prime / arbitrary modulus) — `lucas`, `exlucas` |
| `Cantor.cpp` | Cantor expansion / inverse (BIT, O(n log n)) — `cantor<N>` |
| `DuSieve.cpp` | Du's sieve (Σφ, Σμ prefix sums, O(n^{2/3})) — `du_sieve<SN>` |
| `LinearRecurrence.cpp` | Berlekamp-Massey + Kitamasa (k-th term of a linear recurrence, O(d² log k)) — `kth_term` |
| `Simpson.cpp` | adaptive Simpson integration — `integrate` |
| `SubsetConvolution.cpp` | subset convolution (popcount layering + OR zeta/Möbius, O(n² 2^n)) — `subset_convolution` |
| `Pell.cpp` | Pell equation x²-Dy²=1 fundamental solution (continued fractions) — `pell` |
| `NimProduct.cpp` | nim product (nimber multiplication, largest Fermat 2-power divide & conquer, 64-bit) — `nim_prod` |

### Misc/ (11)

| File | Content |
|---|---|
| `Bitset.cpp` | bitset trick (wildcard string matching with 26 bitsets) |
| `HashTable_PBDS.cpp` | hash table I: gp_hash_table + splitmix64 (anti-hack) — `splitmix64_hash` |
| `HashTable_Chain.cpp` | hash table II: hand-written chained reference-counting table (slot-pool recycling) |
| `HashMap_OpenAddress.cpp` | hash table III: open addressing, linear probing + backward-shift deletion — `HashMap<K, V, B>` |
| `DSUonTree.cpp` | DSU on tree (small-to-large subtree statistics) |
| `SimulatedAnnealing.cpp` | simulated annealing (generic template, continuous/discrete optimization) |
| `ExpressionEval.cpp` | expression evaluation (recursive descent: + - * / % ^ parentheses, unary minus, variables) — `expr_eval` |
| `MoAlgorithm.cpp` | Mo's algorithm (plain + with updates; distinct-values example; odd/even blocking) — `mo_distinct` / `mo_distinct_modify` |
| `CDQDivide.cpp` | CDQ divide & conquer (3D partial order counting, O(n log² n)) — `count_3d` |
| `ParallelBinarySearch.cpp` | parallel binary search (offline static range k-th smallest) — `parallel_kth` |
| `RollbackMo.cpp` | rollback Mo (deletion-free; farthest equal-value distance example) — `rollback_mo` |

### Geometry/ (3)

| File | Content |
|---|---|
| `Geometry_2D.cpp` | 2D basics (exact integer arithmetic: convex hull / rotating-calipers diameter / closest pair / point in polygon / segment intersection) |
| `Geometry_HalfPlane.cpp` | half-plane intersection (sort & incremental deque, O(n log n), area included) |
| `Geometry_MinCircle.cpp` | minimum enclosing circle (randomized incremental, expected O(n); circle2/circle3 included) |