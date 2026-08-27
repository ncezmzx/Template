#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>
using namespace std;
// 需要事先 #include <ext/pb_ds/priority_queue.hpp>
typedef __gnu_pbds::priority_queue<int, greater<int>> heap;
constexpr int N = 1e6 + 9;
heap pq[N];
heap::point_iterator its[N];
int newnode(int x, int i) { return its[i] = pq[i].push(x), i; }
int top(int x) { return pq[x].top(); }
void join(int x, int y) { pq[x].join(pq[y]); }
void decrease_key(int h, int p, int v) { pq[h].modify(its[p], v); }
void erase(int h, int x) { pq[h].erase(its[x]); }
/*
 * ============================================================
 * 名称：pbds 配对堆（Pairing Heap，__gnu_pbds 实现，可并堆）
 * 复杂度：newnode/top/join O(1) 均摊；decrease_key/erase O(log n) 均摊
 * 用途：newnode/top/join/decrease_key/erase；小根堆（greater<int>）
 * 来源：洛谷文章《对优先队列的爱》(luogu_blog_1_对优先队列的爱.md) 第 1 节，代码原样保留
 * 注意：需要 pb_ds（已按要求添加 #include <ext/pb_ds/priority_queue.hpp>）；its[] 存迭代器，erase/modify 后失效；join 后 y 变空堆；元素类型固定为 int
 * ============================================================
 */
