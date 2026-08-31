// fast I/O (fread/fwrite buffered) — note: read/print handle non-negative ints only
#include <bits/stdc++.h>
using namespace std;

char buf[1 << 20], *p1, *p2;
char obuf[1 << 23], *O = obuf;
#define pc(x) (*O++ = (x))
#define flush() (fwrite(obuf, 1, O - obuf, stdout), O = obuf)
#define gc (p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, 1 << 20, stdin)) == p1 ? EOF : *p1++)
void read(int &x) { // skips non-digits, then accumulates digits
  char c = gc;
  x = 0;
  for (; c < '0' || '9' < c; c = gc)
    ;
  for (; '0' <= c && c <= '9'; c = gc) x = (x << 1) + (x << 3) + (c & 15);
}
void print(int x) { // recursive digit output
  if (x > 9) print(x / 10);
  pc(x % 10 + '0');
}
// extension: the original print handled int only; long long overload added
void print(long long x) {
  if (x > 9) print(x / 10);
  pc(x % 10 + '0');
}

/*
 * ============================================================
 * Name: fast I/O (fread/fwrite buffered)
 * Complexity: O(input size); reads/writes are memory copies plus one
 *             fread/fwrite, much faster than cin/cout
 * Usage: huge I/O volumes (1e6+ integers); read(n) reads a NON-NEGATIVE int,
 *        print(x) + pc(' ') buffers output, flush() once at the end (writes
 *        the buffer to stdout). Original read/print do not handle negatives
 *        (read skips non-digits; print emits non-negative integers)
 * Source: all.cpp 14767-14781 (extracted verbatim); the long long print
 *         overload is an extension. Also: a simple getchar version at
 *         all.cpp 5267-5276 (read/write, no negatives), and a getchar version
 *         with '-' support at all.cpp 15167-15172 (inline int read)
 * ============================================================
 * Example (uncomment to compile):
 * int main() {
 *   int n;
 *   read(n);  // non-negative integers only
 *   for (int i = 1; i <= n; ++i) {
 *     int x;
 *     read(x);
 *     print(x);
 *     pc(i == n ? '\n' : ' ');
 *   }
 *   print(123456789012345LL);  // long long overload
 *   pc('\n');
 *   flush();
 * }
 * ============================================================
 */
