
#include <bits/stdc++.h>
using namespace std;

char buf[1 << 20], *p1, *p2;
char obuf[1 << 23], *O = obuf;
#define pc(x) (*O++ = (x))
#define flush() (fwrite(obuf, 1, O - obuf, stdout), O = obuf)
#define gc (p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, 1 << 20, stdin)) == p1 ? EOF : *p1++)
void read(int &x) {
  char c = gc;
  x = 0;
  for (; c < '0' || '9' < c; c = gc)
    ;
  for (; '0' <= c && c <= '9'; c = gc) x = (x << 1) + (x << 3) + (c & 15);
}
void print(int x) {
  if (x > 9) print(x / 10);
  pc(x % 10 + '0');
}

void print(long long x) {
  if (x > 9) print(x / 10);
  pc(x % 10 + '0');
}

