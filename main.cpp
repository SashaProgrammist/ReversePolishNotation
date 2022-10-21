#include <iostream>

#include "classes/Function/Function.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    OperatorFunction<int> plus{
            2, 1, "+",
            [](std::vector<int> operands) {
                return operands[0] + operands[1];
            }
    };
    OperatorFunction<int> minus{
            2, 1, "-",
            [](std::vector<int> operands) {
                return operands[0] - operands[1];
            }
    };
    OperatorFunction<int> multiply{
            2, 2, "*",
            [](std::vector<int> operands) {
                return operands[0] * operands[1];
            }
    };

    std::vector _setOperator{plus, minus, multiply};
    SetOperator setOperator{_setOperator};

    Function<int> function{"A*((A+A)*B)", setOperator};

    int A(2);
    int B(3);
    SetVariable<int> setVariable{std::map<std::string, int>{
            {"A", A},
            {"B", B}
    }};

    int result = function.call(setVariable);

    std::cout << result << std::endl;

    return 0;
}
