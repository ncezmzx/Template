#include <bits/stdc++.h>
using namespace std;
// ============================================================
// 名称：ST 表（Sparse Table，静态区间最值 RMQ，此处为区间最小值）
// 复杂度：预处理 O(n log n)，查询 O(1)
// 用途：静态数组的区间最值查询（不支持修改）
// 使用示例：
//   N = 数组长度; 填好 ht[1..N]; st.init(); st.query(l, r);
// 来源：all.cpp 第 27805-27821 行（struct ST_Table，原为 SA 的 height 数组建表）
// 调整说明：原块使用 #define maxn 200005、全局 N 与 ht[]（SA height），模板中保留同名全局，
//           把 ht 当作待查询的源数组即可；要查最大值把 min 换成 max。
// ============================================================
#define maxn 200005  // 数组上限，按需调整
int N;               // 数组长度（1-based）
int ht[maxn];        // 源数组（原题为 SA 的 height 数组）
struct ST_Table
{
	int d[maxn][30],lg[maxn];
	inline void init()
	{
		lg[0]=-1;
		for(int i=1;i<=N;++i) d[i][0]=ht[i],lg[i]=lg[i>>1]+1;
		for(int j=1;j<=25;++j)
			for(int i=1;i+(1<<(j-1))<=N;++i)
				d[i][j]=min(d[i][j-1],d[i+(1<<(j-1))][j-1]);
	}
	inline int query(int l,int r)
	{
		int k=lg[r-l+1];
		return min(d[l][k],d[r-(1<<k)+1][k]);
	}
}st;

#ifdef DEMO
signed main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  N = 5;
  ht[1] = 3, ht[2] = 1, ht[3] = 4, ht[4] = 1, ht[5] = 5;
  st.init();
  cout << st.query(1, 5) << '\n';  // 1
  cout << st.query(2, 3) << '\n';  // 1
  cout << st.query(3, 5) << '\n';  // 1
  return 0;
}
#endif
