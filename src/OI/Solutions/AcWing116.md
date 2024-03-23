# [AcWing116飞行员兄弟 题解](https://www.acwing.com/problem/content/description/118/)

## 题目大意

给定一个4*4的矩阵，可以操作一个元素，使此元素所在行与列的所有元素**异或 1**，求最少操作次数与对应的字典序最小的操作序列

## 解析

注意到，这个题目中的操作就是异或，通过异或的性质，我们可以得出这个操作具有如下性质

**性质1：由于异或同一个数会相互抵消，所以每个点应该只被操作1次** 

**性质2：由于异或满足交换律，操作之间的顺序对于操作效果没有影响**

并且，由于这道题数据范围很小，只有$4*4$

由于这两个性质，我们只需要枚举每个元素会不会被操作即可,时间复杂度为$O(2^n)$，在本题中，$n=16$

## 代码

### 方法1 DFS

通过朴素的dfs,对每个元素进行搜索，枚举是否会被操作

```cpp
#include <bits/stdc++.h>
using namespace std;

bool a[6][6];

// 对这个点进行操作
inline void turn(int x, int y) {
    for (int i = 1; i <= 4; i++) {
        a[x][i] ^= 1;
        a[i][y] ^= 1;
    }
    // 以上对这个元素所在行，所在列进行异或
    a[x][y] ^= 1;
    // 由于这个元素被多异或了1此，所以通过又一次异或操作进行抵消
}

// 检查目前的状态是否是成功的，即所以把手都打开，所以元素都为1
inline bool check() {
    bool suc = 1;
    for (int i = 1; i <= 4; i++) {
        for (int j = 1; j <= 4; j++) {
            suc &= a[i][j];
        }
    }
    return suc;
}

// 这个类型表示操作序列
typedef vector<pair<int, int>> O;

O ans, opt;
int ans_size = 0x3f3f3f3f;
// 我们从（1,1）开始，按照字典序进行搜索，所以得到的操作序列也是
void dfs(int x, int y, int cnt) {
    if (cnt >= ans_size) {
        return;
    }
    if (check()) {
        ans = opt;
        ans_size = cnt;
    }
    if (y > 4) {
        x++, y = 1;
    }
    if (x > 4) {
        return;
    }

    dfs(x, y + 1, cnt);

    opt.push_back({x, y});
    turn(x, y);
    dfs(x, y + 1, cnt + 1);
    turn(x, y);
    // 因为异或操作，所以我们再进行一次这种操作就能抵消
    opt.pop_back();
}

int main() {
    for (int i = 1; i <= 4; i++) {
        for (int j = 1; j <= 4; j++) {
            char c;
            cin >> c;
            if (c == '-') {
                a[i][j] = 1;
            }
        }
    }

    dfs(1, 1, 0);

    printf("%d\n", ans_size);
    for (auto p : ans) {
        int x = p.first, y = p.second;
        printf("%d %d\n", x, y);
    }
    return 0;
}

```

### 枚举

除了直接dfs外，我们还有另外一种方式，我们可以把操作序列看成一个**16位的二进制数**，这个数的每一个**二进制位**都表示一个元素有没有被进行过操作

具体地说，我们将这个数的倒数第一位视作矩阵的 **(1,1)** 位置，倒数第二位对应 **(1,2)**，倒数第五位则对应 **(2,1)** ...


```cpp
#include <bits/stdc++.h>
using namespace std;

int power[20];

bool a[6][6];
bool backup[6][6];

inline void turn(int x, int y) {
    for (int i = 1; i <= 4; i++) {
        a[x][i] ^= 1;
        a[i][y] ^= 1;
    }
    a[x][y] ^= 1;
}

inline bool check() {
    bool suc = 1;
    for (int i = 1; i <= 4; i++) {
        for (int j = 1; j <= 4; j++) {
            suc &= a[i][j];
        }
    }
    return suc;
}

// 找出这个数二进制下的最后一个为1的位
inline int lowbit(int x) { return x & (-x); }

typedef vector<pair<int, int>> O;

// 把这个二进制数转换成对应的操作序列
inline void gen_opt(int opt, O &output) {
    output.clear();

    int power_index = 0;
    while (opt) {
        int lb = lowbit(opt);
        opt -= lb;
        while (lb != power[power_index]) {
            power_index++;
        }

        int x = 1, y = power_index + 1;
        while (y > 4) {
            y -= 4;
            x++;
        }
        output.push_back({x, y});
    }
}

O ans;
int ans_size = 0x3f3f3f3f;
inline void solve() {
    memcpy(backup, a, sizeof(a));
    int all = (1 << 16);
    O opt;
    for (int i = 0; i <= all; i++) {
        gen_opt(i, opt);
        for (auto p : opt) {
            turn(p.first, p.second);
        }
        if (check()) {
            if (opt.size() < ans_size) {
                ans_size = opt.size();
                ans = opt;
            }
        }
        memcpy(a, backup, sizeof(a));
    }
}

int main() {
    for (int i = 1; i <= 4; i++) {
        for (int j = 1; j <= 4; j++) {
            char c;
            cin >> c;
            if (c == '-') {
                a[i][j] = 1;
            }
        }
    }

    power[0] = 1;
    for (int i = 1; i <= 16; i++) {
        power[i] = power[i - 1] * 2;
    }

    solve();

    printf("%d\n", ans_size);
    for (auto p : ans) {
        int x = p.first, y = p.second;
        printf("%d %d\n", x, y);
    }
    return 0;
}

```