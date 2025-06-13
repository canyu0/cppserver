#include <iostream>

// 使用宏标记bool值
#define BOOL_VAL(x) static_cast<bool>(x)

void process_value(bool value) {
    std::cout << "处理bool值: " << std::boolalpha << value << std::endl;
}

void process_expression(bool value) {
    std::cout << "处理表达式结果: " << std::boolalpha << value << std::endl;
}

int main() {
    bool b = true;
    int a = 10;
    
    process_value(BOOL_VAL(b));           // 标记为bool值
    process_expression(a == 10);          // 表达式
    
    return 0;
}
