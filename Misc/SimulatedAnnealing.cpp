#include <bits/stdc++.h>
using namespace std;
#define int long long

mt19937 rng(random_device{}());
double rnd() { return (double)uniform_int_distribution<int>(0, 1e9)(rng) / 1e9; }

double fx, fy;
double target(double x, double y) {
  return (x - 3) * (x - 3) + (y + 2) * (y + 2) + 1;
}

void anneal(double& x, double& y) {
  double t = 1e4;
  double cur = target(x, y);
  while (t > 1e-12) {
    double nx = x + (rnd() * 2 - 1) * t;
    double ny = y + (rnd() * 2 - 1) * t;
    double nv = target(nx, ny);
    if (nv < cur || exp((cur - nv) / t) > rnd()) {
      x = nx, y = ny, cur = nv;
    }
    t *= 0.9995;
  }
}

/*
 * ============================================================
 * 名称：模拟退火（Simulated Annealing，通用模板）
 * 复杂度：O(迭代次数 * 评估代价)；迭代次数 ≈ ln(T0/T_end)/ln(1/降温率)
 * 用途：无解析解/难优化的连续或离散最优化问题（如几何最值、乱序贪心
 *       调参）；本模板为连续二维示例，改 target 与扰动方式即可套用
 * 原理：从当前解随机扰动得到候选；若更优则接受，否则以
 *       exp(-ΔE / T) 概率接受（Metropolis 准则，允许跳出局部最优）；
 *       温度 T 按几何级数降温，最终收敛
 * 注意：参数（初温 T0、降温率、扰动幅度 = T）需要按问题调；
 *       建议多次运行 anneal 取最优（随机性）；目标函数为 min 形式，
 *       求 max 取负；离散问题把扰动改为随机交换/翻转
 * ============================================================
 * 使用示例（编译时取消注释；求 (x-3)^2+(y+2)^2+1 的最小值点）：
 * signed main() {
 *   fx = fy = 0;
 *   double best = 1e18, bx = 0, by = 0;
 *   for (int i = 0; i < 20; ++i) {
 *     fx = rnd() * 20 - 10, fy = rnd() * 20 - 10;
 *     anneal(fx, fy);
 *     if (target(fx, fy) < best) best = target(fx, fy), bx = fx, by = fy;
 *   }
 *   cout << fixed << setprecision(6) << bx << ' ' << by << ' ' << best << '\n';
 * }
 * ============================================================
 */
