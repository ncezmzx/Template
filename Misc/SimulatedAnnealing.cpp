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
 * Name: simulated annealing (generic template)
 * Complexity: O(iterations * evaluation cost); iterations ~ ln(T0/T_end) /
 *             ln(1/cooling rate)
 * Usage: continuous or discrete optimization without an analytic solution
 *        (geometric extrema, shuffled-greedy tuning, ...); this template is a
 *        continuous 2D example — swap the target function and the perturbation
 *        to adapt.
 * Principle: perturb the current solution into a candidate; accept if better,
 *            otherwise accept with probability exp(-dE / T) (the Metropolis
 *            rule, which allows escaping local optima); the temperature T cools
 *            geometrically until convergence
 * Notes: tune the parameters (initial T0, cooling rate, perturbation scale = T)
 *        per problem; run anneal several times and keep the best, since it is
 *        randomized; the objective is minimization, so negate for maxima; for
 *        discrete problems use random swaps / flips as the perturbation
 * ============================================================
 */
