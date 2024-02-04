#include <bits/stdc++.h>
#include <cstdio>
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
// 添加结点
inline int load(int val) {
  t[++tot] = {val, 0, 0, 1, rand()};
  return tot;
}

// 维护子树大小
inline void update(int o) { t[o].siz = t[t[o].l].siz + 1 + t[t[o].r].siz; }

// x表示分裂之后的较小的子树，y表示分裂之后较大的子树
void split(int o, int k, int &x, int &y) {
  // 如果此树为空
  if (!o) {
    x = y = 0;
    return;
  }

  int a, b;
  if (t[o].val <= k) {
    // 如果这个结点的值<=k，那么就去分裂右子树，那么断口就在右子树
    // 从右子树分裂出来的子树作为答案
    split(t[o].r, k, a, b);
    t[o].r = a;
    x = o;
    y = b;
  } else {
    // 同上
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
  // 对于随机权值维护堆的性质
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

// 查询某个子树中排名为x的数
// 这个很好实现，由于我们维护了子树大小
// 可以直接通过子树大小查询

inline int kth(int x, int k) {
  while (1) {
    if (k <= t[t[x].l].siz) {
      x = t[x].l;
    } else if (t[t[x].l].siz + 1 == k) {
      return x;
    } else {
      // 注意！由于我们进入了右子树，所以排名要减去左子树和根节点
      // 原因是我们查询在某个子树的排名时，排名总是相对于这个子树而言的
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

  // 先分裂得到 a<=x, b>x
  // 再由a分裂得到c<x,d=x

  // 由于只需要删除一个数，我们舍弃根结点，直接把左右子树合在一起
  d = merge(t[d].l, t[d].r);

  // 按顺序得到从小到大合一起
  root = merge(merge(c, d), b);
}

inline void insert(int x) {
  // 先添加一个新的结点
  int t = load(x);
  int a, b;
  // 由于合并操作所操作的A,B两树要求A的结点总要小于等于B
  // 所以我们将原树分裂，然后就可以进行合并了
  split(root, x, a, b);
  // a<=x,t=x,b>x
  root = merge(merge(a, t), b);
}

int n;
int main() {
  n = qread();
  for (int i = 1; i <= n; i++) {
    int k = qread(), x = qread();
    if (k == 1) {
      insert(x);
    } else if (k == 2) {
      del(x);
    } else if (k == 3) {
      int a, b;
      split(root, x - 1, a, b);
      // 分裂使得a<x,那么输出a的大小就行
      printf("%d\n", t[a].siz + 1);
      root = merge(a, b);
    } else if (k == 4) {
      printf("%d\n", t[kth(root, x)].val);
    } else if (k == 5) {
      int a, b;
      split(root, x - 1, a, b);
      // 由于我们要寻找小于x的最大的数
      // 我们对x-1进行分裂，使得a<x，那么a中的最大数就是答案
      printf("%d\n", t[kth(a, t[a].siz)].val);
      root = merge(a, b);
    } else if (k == 6) {
      int a, b;
      split(root, x, a, b);
      // 由于我们要寻找大于x的最小数
      // 我们对x进行分裂，使得b>x，那么b中的最小数就是答案
      printf("%d\n", t[kth(b, 1)].val);
      root = merge(a, b);
    }
  }
  return 0;
}
