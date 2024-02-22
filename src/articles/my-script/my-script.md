# 我所设计的脚本语言

## 目标 高性能 小巧 现代

## 阐述

### 静态强类型

此语言强类型，静态，对于需要动态类型的变量，采用 any 类型

与动态类型语言相比，可以有效在编译期拦截类型错误，利于开发大型项目，更加有利于 jit，可以编译到 wasm

### 无 GC

（其实就是不会写）

在运行时检查引用有效性，具体实现为对于会被引用的变量，将其地址与 version 存入 arena，在解引用的时候先在 arena 中查询是否存在这个变量

### 结构体

由于静态强类型的存在，使结构体成为可能，用来替代 table,object 之类的东西

### 多种编译方式

-   编译到 rust
-   编译到 wasm
-   编译到 js
-   编译到字节码，然后使用内置引擎解释执行

### 类型

#### 基本数据类型如下

| 关键字     | 类型     |
| ---------- | -------- |
| **int**    | 整数     |
| **bool**   | 布尔     |
| **float**  | 浮点数   |
| **string** | 字符串   |
| **nil**    | 空       |
| **any**    | 任意类型 |

#### 其他内置类型

| 语法                   | 类型 |
| ---------------------- | ---- |
| **[type]**             | 数组 |
| **table<type?,type?>** | 表   |
| **fn (arg...)->ret**   | 函数 |

table 的泛型参数如果被省略，为 any

#### 复合类型

| 语法   | 类型                                            |
| ------ | ----------------------------------------------- |
| A \| B | 可能为 A 也可能为 B 的类型                      |
| T?     | 可能为 T 也可能为 nil 的类型，T \| nil 的语法糖 |

#### 其他

此外，所有类型均可以被引用

## 语法

大致与 rust 相同，但也有所不同，比如没有分号

### 定义变量

```ts
let a = 1
let a = 1, b = 2
let a, b = 1, 2
//支持重影，类型推导
```

### 各种值

```rust
const a : string =  "Hello"

const t : table = {
    "a" : 1,
    2 : 3,
    [1,2] : "b"
}
// 理论上table的key能是任何能被hash的值

let arr : [int] = [1,2]

let tuple : (int, float) = (1, 2.)
// 有小数点就算浮点数

let big_int = 1000_1000
// 数字中间可以添加下划线
```

### 都是表达式

与 Rust 类似，任何语句和**block**都是表达式

```rust
let a = { 1 + 2 }
```

替代三元运算符

```rust
let res = if cond {
    1
} else {
    2
}
```

### if 语句

```rust
if cond {

} else if cond {

} else {

}
```

### for loop

```rust
for item in <iter> {}
```

```rust
// 0..100 可以创造一个迭代器 类似于python的 range(0,100)
for item in 0..100 {

}

// 表示可以取到100
for item in 0..=100{

}
```

### while loop

不必多说

```rust
while cond {}
```

### loop

与 rust 类似，采用 loop 代替 while true

以下两者等价

```rust
loop {}
```

```rust
while true {}
```

### 函数

```rust
fn sum(a : int , b : int) -> int {
    a + b
}

fn sum(a : int , b : int) -> int {
    return a + b
}
```

### 匿名函数

```rust
| x:int,y:int | -> int {
    x + y
}
```

### 结构体

```rust
struct Vector {
    x : float,
    y : float,
    sum : fn(&self)->float,
    // 方法也可以放在结构体内部，使得可以在运行时改变
}

// 定义方法，但是方法是不变的
impl Vector {
    fn new(x:float,y:float)->Vector{
        Vector {
            x, y,
            sum:fn (&self)->float{
                self.x+self.y
            }
        }
    }
}

// meta关键字也是用来定义方法的，但是类似于lua的元表，用来定义特殊的东西
meta Vector {
    fn add(self,other:Self)->Self{
        Vector {
            x:self.x+other.x,
            y:self.y+other.y
        }
    }
}

let v = Vector::new(1. , 2.)

let sum = v.sum()

let other = Vector::new(2. , 3.)

let e = v + other
```
