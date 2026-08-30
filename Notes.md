### Coding conventions (library-wide)
- Each file holds **only the code body**: `#include` + `using namespace std;`
  (+ `#define int long long` where the algorithm needs it) + the
  encapsulated implementation;
- **all documentation lives in one trailing comment block** (Name /
  Complexity / Usage / Interface / Principle / Notes);
- the block documents **only the essentials**: Name, Complexity, a compact
  Usage (API + the conventions you must obey), Notes (hard constraints and
  pitfalls) and, where useful, Principle;
- comments are written in **English**.


## Notes

- Every template compiles under `g++ -std=c++14 -O2` (Trash included). The
  few C++17 constructs used (structured bindings) only warn under -std=c++14
  (-Wc++17-extensions) on clang; `__lg` / `__int128` are GNU extensions.
  Exception: `KDT_DynamicSemigroup.cpp` self-tests need `-std=c++17`.
  Compile check: `g++ -std=c++14 -O2 file.cpp -c`.
- The library passed an ASan + UBSan randomized stress-test matrix; legal use
  shows no UB / out-of-bounds / use-after-free. Every file is a code body only
  and compiles standalone: `g++ -std=c++14 -O2 file.cpp -c`.
- Array sizes (N, M) are template parameters in the wrapped structures — tune
  them per problem.
- Files using `#define int long long`: mind memset / 0x3f semantics on 64-bit ints.
- Multiple implementations of the same algorithm are distinguished by file
  suffix: balanced trees (Treap / Splay / WBST_Seq / WBST_Set), GCD (Binary /
  Table), hash tables (PBDS / Chain / OpenAddress) — pick per problem.
