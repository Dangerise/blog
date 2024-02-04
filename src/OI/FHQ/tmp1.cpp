#include <bits/stdc++.h>
using namespace std;

inline int qread() {
  int ans = 0;
  char c = getchar();
  bool f = 0;
  while (c < '0' || c > '9') {
    if (c == '-') {
      f = 1;
    }
    c = getchar();
  }
  while (c >= '0' && c <= '9') {
    ans = ans * 10 + c - '0';
    c = getchar();
  }
  if (f) {
    return -ans;
  } else {
    return ans;
  }
}

const int N = 200200;
struct T {
  int val, l, r, siz, p;
};

T t[N];

int tot, root;
inline int load(int val) {
  t[++tot] = {val, 0, 0, 1, rand() % 30};
  return tot;
}

inline void update(int o) { t[o].siz = t[t[o].l].siz + 1 + t[t[o].r].siz; }

void split(int o, int k, int &x, int &y) {
  // 如果此树为空
  if (!o) {
    x = y = 0;
    return;
  }

  int a, b;
  if (t[o].val <= k) {
    split(t[o].r, k, a, b);
    t[o].r = a;
    x = o;
    y = b;
  } else {
    split(t[o].l, k, a, b);
    t[o].l = b;
    x = a, y = o;
  }

  update(o);
}

int merge(int x, int y) {
  if (!x) {
    return y;
  }
  if (!y) {
    return x;
  }
  if (t[x].p < t[y].p) {
    t[x].r = merge(t[x].r, y);
    update(x);
    return x;
  } else {
    t[y].l = merge(x, t[y].l);
    update(y);
    return y;
  }
}

inline int kth(int x, int k) {
  while (1) {
    if (k <= t[t[x].l].siz) {
      x = t[x].l;
    } else if (t[t[x].l].siz + 1 == k) {
      return x;
    } else {
      k -= t[t[x].l].siz + 1;
      x = t[x].r;
    }
  }
}

// 删除一个值为x的数
inline void del(int x) {
  int a, b, c, d;
  split(root, x, a, b);
  split(a, x - 1, c, d);

  // 由于只需要删除一个数，我们舍弃根结点，直接把左右子树合在一起
  d = merge(t[d].l, t[d].r);

  root = merge(merge(c, d), b);
}

inline void insert(int x) {
  int idx = load(x);
  int a, b;
  split(root, x, a, b);
  root = merge(merge(a, idx), b);
}

inline string lit(int o) {
  if (1) {
    return to_string(t[o].val) + "-" + to_string(t[o].p);
  } else {
    return to_string(t[o].val);
  }
}

inline void dfs(int o) {
  if (t[o].l) {
    cout << lit(o) << ' ' << lit(t[o].l) << endl;
    dfs(t[o].l);
  }
  if (t[o].r) {
    cout << lit(o) << ' ' << lit(t[o].r) << endl;
    dfs(t[o].r);
  }
}

int n;
int main() {
  srand(114514);

  n = qread();
  for (int i = 1; i <= n; i++) {
    insert(i);
  }

  printf("%d as root\n\n", root);

  cout << lit(root) << endl;
  for (int i = 1; i <= n; i++) {
    if (root == i) {
      continue;
    }
    cout << lit(i) << endl;
  }

  cout << endl;
  dfs(root);
}
