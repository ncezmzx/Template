
vector<int> window_max(const vector<int> &a, int y) {
  int m = (int)a.size();
  vector<int> res(max(0, m - y + 1));
  deque<int> q;
  for (int j = 0; j < m; ++j) {
    while (!q.empty() && q.front() < j - y + 1) q.pop_front();
    while (!q.empty() && a[q.back()] < a[j]) q.pop_back();
    q.push_back(j);
    if (j >= y - 1) res[j - y + 1] = a[q.front()];
  }
  return res;
}

