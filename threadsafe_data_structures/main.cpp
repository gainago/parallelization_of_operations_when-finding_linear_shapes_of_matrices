#include "function_wrapper.h"
#include <iostream>

class Callable {
public:
    void operator()() {
        std::cout << "Work Done!\n";
    }
    Callable() = default;
    Callable(const Callable& other) = delete;
    Callable(Callable&&) = default;
};

void function_to_call(function_wrapper&& fun) {
    fun();
}

void fun_to_change(std::unique_ptr<function_wrapper>& ptr_to_fun) {
    Callable cal;
    if(!ptr_to_fun) {
        throw "pointer is not to empty function";
    }
    ptr_to_fun = std::;
}

int main() {
    std::unique_ptr<function_wrapper> ptr(Callable);

}
