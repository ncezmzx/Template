#include <bits/stdc++.h>
using namespace std;
#define int long long

mt19937 rng(random_device{}());
double rnd() { return (double)uniform_int_distribution<int>(0, 1e9)(rng) / 1e9; }

double fx, fy;
double target(double x, double y) { return (x - 3) * (x - 3) + (y + 2) * (y + 2) + 1; }

void anneal(double &x, double &y) {
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

