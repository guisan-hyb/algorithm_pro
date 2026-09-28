## cpp iomanip 格式化

C++ 中的 `<iomanip>`（Input/Output Manipulators，输入/输出操纵符）头文件提供了大量用于格式化输入输出的工具。它主要配合 `std::cin` 和 `std::cout` 使用。

为了便于理解和查阅，下面将常用的格式化操纵符按**功能分类**进行详细整理，并附带代码示例。

---

### 💡 核心概念：粘性
在开始之前，需要明白一个重要概念：**大部分格式化操纵符是“粘性”的**，即一旦设置，其效果会一直持续，直到被改变。
**唯一的例外**：`std::setw()`（设置宽度），它只对紧接着的**下一个**输出项有效，输出后自动恢复默认。

---

### 一、 整数进制控制
用于控制整数的输入输出进制。

| 操纵符             | 作用           | 说明                                 |
| :----------------- | :------------- | :----------------------------------- |
| `std::dec`         | 十进制         | 默认状态                             |
| `std::hex`         | 十六进制       | 字母默认为小写                       |
| `std::oct`         | 八进制         |                                      |
| `std::showbase`    | 显示进制前缀   | 输出前缀（如 `0x` 或 `0`）           |
| `std::noshowbase`  | 不显示进制前缀 | 默认状态                             |
| `std::uppercase`   | 大写字母       | 应用于十六进制字母和科学计数法的 `E` |
| `std::nouppercase` | 小写字母       | 默认状态                             |

**示例代码：**
```cpp
#include <iostream>
#include <iomanip>

int main() {
    int n = 255;
    std::cout << "默认: " << n << std::endl;             // 255
    std::cout << "十六进制: " << std::hex << n << std::endl; // ff
    std::cout << "大写并带前缀: " << std::showbase << std::uppercase << n << std::endl; // 0XFF
    
    // 恢复默认
    std::cout << std::dec << std::noshowbase << std::nouppercase;
    
    // 输入也可以用进制控制
    int input;
    std::cin >> std::hex >> input; // 如果输入 "ff"，则 input 为 255
    std::cout << "输入的十进制是: " << std::dec << input << std::endl;

    return 0;
}
```

---

### 二、 对齐与填充控制
常用于制作表格或美观的输出排版。

| 操纵符                 | 作用         | 说明                                             |
| :--------------------- | :----------- | :----------------------------------------------- |
| `std::setw(int n)`     | 设置字段宽度 | **非粘性**！只对下一项有效。                     |
| `std::setfill(char c)` | 设置填充字符 | 默认是空格。粘性。                               |
| `std::left`            | 左对齐       | 右侧填充字符。粘性。                             |
| `std::right`           | 右对齐       | 默认状态。左侧填充字符。粘性。                   |
| `std::internal`        | 内部对齐     | 符号位或进制前缀在左，数字在右，中间填充。粘性。 |

**示例代码：**
```cpp
#include <iostream>
#include <iomanip>

int main() {
    double price = 12.5;
    // setw 需要放在每个输出项之前
    std::cout << std::left << std::setw(10) << "Apple" 
              << std::right << std::setw(10) << price << std::endl;
    // 输出: Apple            12.5 (左侧左对齐，右侧右对齐补空格)

    // 使用 setfill 填充
    std::cout << std::setfill('*') << std::setw(10) << 42 << std::endl;
    // 输出: ********42
    
    // 恢复填充字符
    std::cout << std::setfill(' '); 

    // internal 对齐（常用于金额）
    std::cout << std::internal << std::setfill('0') << std::setw(6) << -42 << std::endl;
    // 输出: -00042 (负号在最左，数字在右，中间填0)
    
    return 0;
}
```

---

### 三、 浮点数控制
控制浮点数（`float`, `double`）的显示方式。

| 操纵符                     | 作用             | 说明                             |
| :------------------------- | :--------------- | :------------------------------- |
| `std::fixed`               | 定点小数表示法   | 固定显示小数点后的数字。         |
| `std::scientific`          | 科学计数法表示   | 如 `1.23e+02`。                  |
| `std::defaultfloat`        | 默认浮点表示法   | C++11 引入，恢复默认。           |
| `std::setprecision(int n)` | 设置精度         | 控制有效数字位数或小数点后位数。 |
| `std::showpoint`           | 总是显示小数点   | 即使是整数也显示 `.0000`。       |
| `std::noshowpoint`         | 不显示多余小数点 | 默认状态。                       |

> **注意 `setprecision` 的行为：**
> - 在默认模式下：控制**总有效数字**位数。
> - 在 `std::fixed` 或 `std::scientific` 模式下：控制**小数点后**的位数。

**示例代码：**
```cpp
#include <iostream>
#include <iomanip>

int main() {
    double pi = 3.1415926535;
    
    // 默认模式：总有效数字（默认为6位）
    std::cout << "默认: " << pi << std::endl; // 输出: 3.14159 (6位有效数字)
    
    // 设置总有效数字为 10
    std::cout << "精度10: " << std::setprecision(10) << pi << std::endl; // 3.141592654
    
    // Fixed 模式：控制小数点后位数
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Fixed(4): " << pi << std::endl; // 输出: 3.1416
    
    // 科学计数法
    std::cout << std::scientific << std::setprecision(2);
    std::cout << "Scientific(2): " << pi << std::endl; // 输出: 3.14e+00
    
    // 恢复默认
    std::cout << std::defaultfloat << std::setprecision(6);
    
    // showpoint 示例
    std::cout << std::showpoint << 100.0 << std::endl; // 输出: 100.0000
    
    return 0;
}
```

---

### 四、 布尔值控制
控制 `bool` 类型的输出格式。

| 操纵符             | 作用     | 说明                        |
| :----------------- | :------- | :-------------------------- |
| `std::boolalpha`   | 字母显示 | 输出 `true` 或 `false`。    |
| `std::noboolalpha` | 数字显示 | 默认状态，输出 `1` 或 `0`。 |

**示例代码：**
```cpp
#include <iostream>
#include <iomanip>

int main() {
    bool flag = true;
    std::cout << flag << std::endl;               // 1
    std::cout << std::boolalpha << flag << std::endl; // true
    std::cout << std::noboolalpha << flag << std::endl; // 1
    
    // 输入也支持 boolalpha
    bool input;
    std::cin >> std::boolalpha >> input; // 输入 "false"，则 input 变为 false
    return 0;
}
```

---

### 五、 空白符与正负号控制
| 操纵符           | 作用       | 说明                                 |
| :--------------- | :--------- | :----------------------------------- |
| `std::showpos`   | 显示正号   | 非负数前面显示 `+`。                 |
| `std::noshowpos` | 不显示正号 | 默认状态。                           |
| `std::ws`        | 跳过空白符 | 常用于输入流中提取前导空格、换行符。 |

**示例代码：**
```cpp
#include <iostream>
#include <iomanip>

int main() {
    int a = 10, b = -5;
    std::cout << std::showpos << a << " " << b << std::endl; // +10 -5
    
    // ws 用于输入
    char c1, c2;
    std::cin >> c1; // 如果输入 "  a"，c1 会读到空格
    // std::cin >> std::ws >> c1; // 如果用 ws，会跳过空格，c1 直接读到 'a'
    
    return 0;
}
```

---

### 六、 高级与辅助操纵符

| 操纵符                                        | 作用           | 说明                                                         |
| :-------------------------------------------- | :------------- | :----------------------------------------------------------- |
| `std::setbase(int n)`                         | 设置进制       | 参数只能是 8, 10, 16，其他值默认恢复为 10。                  |
| `std::quoted(const string&)`                  | 带引号输入输出 | C++14 引入，自动处理字符串两端的引号和转义字符，非常适合 CSV 文件读写。 |
| `std::setiosflags(std::ios_base::fmtflags)`   | 批量设置标志   | 相当于多个操纵符的或运算组合。                               |
| `std::resetiosflags(std::ios_base::fmtflags)` | 重置指定标志   | 取消之前的设置。                                             |

**示例代码 (`std::quoted` 极度实用)：**
```cpp
#include <iostream>
#include <iomanip>
#include <string>

int main() {
    // 输出时自动加双引号
    std::string str = "Hello, \"World\"!";
    std::cout << std::quoted(str) << std::endl; 
    // 输出: "Hello, \"World\"!" (自动处理内部转义)

    // 输入时自动解析双引号和转义
    std::string input;
    std::cout << "请输入带引号的字符串: ";
    // 输入: "Hello, C++!"
    std::cin >> std::quoted >> input;
    std::cout << "解析结果: " << input << std::endl; // 输出: Hello, C++! (不含外层引号)

    return 0;
}
```

---

### 总结与速查表

| 需求                            | 操纵符组合                                                   |
| :------------------------------ | :----------------------------------------------------------- |
| 输出 2 位小数                   | `std::fixed << std::setprecision(2)`                         |
| 十六进制大写带 `0X` 前缀        | `std::hex << std::showbase << std::uppercase`                |
| 居中对齐（无直接操纵符）        | `std::setw((总宽 + 字串长) / 2) << str` (需手动计算右侧空格) |
| 强制金额格式（正负号+两位小数） | `std::showpos << std::fixed << std::setprecision(2)`         |
| 读取带空格的整行字符串          | 不用 `<iomanip>`，直接用 `std::getline(std::cin, str)`       |
| 安全读写含特殊字符的字符串      | `std::quoted(str)`                                           |

掌握 `<iomanip>` 中的这些操纵符，可以完全抛弃 C 语言中的 `printf` 和 `scanf`，以更类型安全、更符合 C++ 风格的方式进行复杂的格式化操作。

