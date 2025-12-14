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

int main() {
    Callable callable;
    function_wrapper wrap(callable);
    function_to_call(std::move(wrap));
    function_wrapper2 wrap3(callable);

}
