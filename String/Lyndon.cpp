#include <bits/stdc++.h>
using namespace std;


vector<pair<int, int>> duval(const string &s) {
  int n = (int)s.size(), i = 0;
  vector<pair<int, int>> res;
  while (i < n) {
    int j = i + 1, k = i;
    while (j < n && s[k] <= s[j]) {
      if (s[k] < s[j]) k = i;
      else ++k;
      ++j;
    }
    while (i <= k) {
      res.push_back({i, i + j - k});
      i += j - k;
    }
  }
  return res;
}


int min_representation(const string &s) {
  int n = (int)s.size(), i = 0, j = 1, k = 0;
  while (i < n && j < n && k < n) {
    int a = s[(i + k) % n], b = s[(j + k) % n];
    if (a == b) ++k;
    else {
      if (a > b) i += k + 1;
      else j += k + 1;
      if (i == j) ++j;
      k = 0;
    }
  }
  return min(i, j) % n;
}

