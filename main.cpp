#include <iostream>

#include "SetOperators/SetOperators.h"
#include "GeneratorFormula/GeneratorFormula.h"

void example() {
    OperatorFunction<int> plus{
            2, 1, "+",
            [](std::vector<int> operands) {
                return operands[0] + operands[1];
            }
    };
    OperatorFunction<int> minus{
            1, 3, "-",
            [](std::vector<int> operands) {
                return -operands[0];
            }
    };
    OperatorFunction<int> multiply{
            2, 2, "*",
            [](std::vector<int> operands) {
                return operands[0] * operands[1];
            }
    };
    OperatorFunction<int> F{
            3, 4, "!",
            [](std::vector<int> operands) {
                return operands[0] * operands[1] - operands[2];
            }
    };

    SetOperator setOperator{std::vector{plus, minus, multiply, F}};

    Function<int> function{"-!(A+B,B+-C,C*-A)", setOperator};
    std::cout << function.getFunctionExpression() << std::endl;

    int A(2);
    int B(3);
    int C(4);
    SetVariable<int> setVariable{std::map<std::string, int>{
            {"A", A},
            {"B", B},
            {"C", C},
    }};

    int result = function.call(setVariable);

    std::cout << result << std::endl;
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    auto ALG = boolAlg();

    std::cout << generatorFormula<bool>(ALG, "ABS", 5);

    return 0;
}
