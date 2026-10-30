# C++ iomanip 格式化（输出格式速查）

> 来源笔记：`cpp iomanip 格式化.md`｜性质：工具速查表｜分类：10 专题与杂项

## 一句话

`<iomanip>` 提供"输入输出操纵符"，**大部分是粘性的**（设置后一直生效），**唯一例外是 `setw`**（只对紧随其后的一个输出项生效）。

## 一、整数进制控制

| 操纵符 | 作用 | 说明 |
| --- | --- | --- |
| `dec` | 十进制 | 默认 |
| `hex` | 十六进制 | 字母默认小写 |
| `oct` | 八进制 | |
| `showbase` / `noshowbase` | 显示/不显示进制前缀 | 前缀如 `0x`、`0` |
| `uppercase` / `nouppercase` | 大写/小写 | 影响十六进制字母与科学计数的 `E` |

```cpp
int n = 255;
cout << n << '\n';                                                  // 255
cout << hex << n << '\n';                                           // ff
cout << showbase << uppercase << n << '\n';                         // 0XFF
cout << dec << noshowbase << nouppercase;                           // 恢复默认
// 输入也能用进制控制
int x; cin >> hex >> x;                                             // 输入 "ff" → 255
```

## 二、对齐与填充

| 操纵符 | 作用 | 是否粘性 |
| --- | --- | --- |
| `setw(n)` | 设置字段宽度 | **否**（只对下一项有效） |
| `setfill(c)` | 填充字符（默认空格） | 是 |
| `left` / `right` | 左/右对齐（`right` 是默认） | 是 |
| `internal` | 符号/前缀在左、数字在右，中间填充 | 是 |

```cpp
cout << left << setw(10) << "Apple" << right << setw(10) << 12.5 << '\n';
cout << setfill('*') << setw(10) << 42 << '\n';                     // ********42
cout << setfill(' ');
cout << internal << setfill('0') << setw(6) << -42 << '\n';         // -00042
```

## 三、浮点数控制

| 操纵符 | 作用 |
| --- | --- |
| `fixed` | 定点小数 |
| `scientific` | 科学计数法 |
| `defaultfloat` | 恢复默认（C++11） |
| `setprecision(n)` | 精度：默认模式=**总有效数字**；fixed/scientific 模式=**小数点后位数** |
| `showpoint` / `noshowpoint` | 是否总显示小数点 |

```cpp
double pi = 3.1415926535;
cout << pi << '\n';                                   // 3.14159（默认 6 位有效数字）
cout << setprecision(10) << pi << '\n';               // 3.141592654
cout << fixed << setprecision(4) << pi << '\n';       // 3.1416
cout << scientific << setprecision(2) << pi << '\n';  // 3.14e+00
cout << defaultfloat << setprecision(6);
cout << showpoint << 100.0 << '\n';                   // 100.0000
```

## 四、布尔值、正负号与空白

| 操纵符 | 作用 |
| --- | --- |
| `boolalpha` / `noboolalpha` | 输出 `true/false` 或 `1/0`（输入同样支持） |
| `showpos` / `noshowpos` | 非负数是否显示 `+` |
| `ws` | 跳过输入流中的前导空白符 |

```cpp
cout << boolalpha << true << '\n';                    // true
cout << showpos << 10 << ' ' << -5 << '\n';           // +10 -5
cin >> ws >> c;                                       // 跳过空白再读字符
```

## 五、高级与辅助

| 操纵符 | 作用 |
| --- | --- |
| `setbase(n)` | 设置进制，参数只能是 8/10/16（其他值按 10 处理） |
| `quoted(str)` | C++14：带引号输入输出，自动处理转义（适合 CSV） |
| `setiosflags(...)` / `resetiosflags(...)` | 批量设置/重置格式标志 |

```cpp
string str = "Hello, \"World\"!";
cout << quoted(str) << '\n';                          // "Hello, \"World\"!"
string in; cin >> quoted >> in;                       // 输入 "Hello, C++!" → 解析为 Hello, C++!
```

## 速查表

| 需求 | 写法 |
| --- | --- |
| 输出 2 位小数 | `fixed << setprecision(2)` |
| 十六进制大写带 `0X` | `hex << showbase << uppercase` |
| 金额格式（带符号 + 两位小数） | `showpos << fixed << setprecision(2)` |
| 读含空格的整行 | 不用 iomanip，直接 `getline(cin, str)` |
| 安全读写含特殊字符的字符串 | `quoted(str)` |
| 居中对齐 | 没有直接操纵符，要手算 `setw` |

## 适用特征（看到这些信号就选它）

- 题目要求**固定小数位数**（如保留 2 位、保留 6 位）；
- 输出十六进制/八进制、带前缀；
- 输出对齐的表格；
- 读入带引号/空白的字符串。

## 易错点

1. `setw` **不粘性**：每个字段前都要写一次；
2. `setprecision` 的含义随 `fixed/scientific` 改变（总有效数字 vs 小数位数）；
3. 操纵符是**粘性**的，改完记得恢复（`dec/noshowbase/nouppercase/defaultfloat/setfill(' ')`）；
4. 竞赛里输出量大时要关闭 `iostream` 同步并避免频繁刷新；
5. `setbase` 只接受 8/10/16；
6. 读整行要用 `getline`，`cin >> ws` 只跳空白不读整行。

## 专题深化：为什么成立

`iomanip` 提供宽度、填充、精度和进制控制。宽度默认只作用于下一次输出，精度和小数格式会持续影响流状态，因此格式化后要恢复 `defaultfloat` 或保存 flags。

## 专题深化：手算与边界

`cout << fixed << setprecision(2) << 1.2` 输出 1.20；`setw(5)` 只给紧接的一个值补空格；`hex` 会持续到改为 `dec`。

### 对照与取舍

C 风格格式化直观但本项目统一使用 iostream；`format` 在 C++20 更方便，但需较新标准。

### 易错点与反例

1. `setw` 不会自动重复
2. 负数宽度/填充要测试
3. 浮点设置会污染后续输出
4. 混合整数和浮点输出要显式切换

## 专题深化：进阶拓展

自定义流操纵器可封装重复格式；大规模输出仍应关闭同步并减少 flush。

## 专题深化：面试追问

1. setw 的作用范围？
2. fixed 如何恢复？
3. 为什么 endl 会频繁 flush？

## 专题深化：自测（闭卷）

1. 格式化小数两位
2. 十六进制后恢复十进制
3. 用 setw 对齐表格
