#include <iostream>

#include "classes/Function/Function.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    OperatorFunction<int> plus = {
            2, 1, "+",
            [](std::vector<Variable<int>> operands) {
                return Variable<int>{operands[0].getValue() + operands[1].getValue()};
            }
    };

    OperatorFunction<int> minus = {
            2, 1, "-",
            [](std::vector<Variable<int>> operands) {
                return Variable<int>{operands[0].getValue() - operands[1].getValue()};
            }
    };

    SetOperator<int> setOperator(std::vector<OperatorFunction<int>>{plus, minus});

    Function<int> function = {"A+B", setOperator};

    SetVariable<int> setVariable(std::map<std::string, Variable<int>>{
            {"A", variablesOnStack<int>(1)},
            {"B", variablesOnStack<int>(1)}
    });

    int result = function.call(setVariable).getValue();

    std::cout << result << std::endl;

    return 0;
}
