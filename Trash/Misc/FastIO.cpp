// ============================================================
// 名称：快速读入输出（fread/fwrite 缓冲版）
// 复杂度：O(输入规模)；读入/输出均为内存级拷贝 + 一次 fread/fwrite，远快于 cin/cout
// 用途：输入输出量极大（如 1e6 以上整数）时的读入输出优化；
//       注意原版 read/print 均不支持负数（read 跳过非数字，print 递归输出非负整数）
// 使用示例（#ifdef DEMO）：
//   int n; read(n);           // 读入（仅非负整数）
//   print(x); pc(' ');        // 输出到缓冲
//   flush();                  // 结束时必须 flush 一次（写回 stdout）
// 来源：all.cpp 14767-14781（原样提取）；long long 版 print 为自加扩展
// 另：简单 getchar 版见 all.cpp 5267-5276（read/write，无负数处理）；
//     带负数处理的 getchar 版见 all.cpp 15167-15172（inline int read，支持 '-'）
// ============================================================
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
  for (; c < '0' || '9' < c; c = gc);
  for (; '0' <= c && c <= '9'; c = gc) x = (x << 1) + (x << 3) + (c & 15);
}
void print(int x) {
  if (x > 9) print(x / 10);
  pc(x % 10 + '0');
}
// —— 扩展：原代码 print 仅支持 int（且不含负数），此处重载一个 long long 版 ——
void print(long long x) {
  if (x > 9) print(x / 10);
  pc(x % 10 + '0');
}

#ifdef DEMO
int main() {
  int n;
  read(n);  // 只读非负整数
  for (int i = 1; i <= n; ++i) {
    int x;
    read(x);
    print(x);
    pc(i == n ? '\n' : ' ');
  }
  print(123456789012345LL);  // long long 扩展版
  pc('\n');
  flush();
}
#endif
