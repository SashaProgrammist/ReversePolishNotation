#include <iostream>

#include "SetOperators/SetOperators.h"
#include "GeneratorFormula/GeneratorFormula.h"

void example1() {
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

void example2() {
    auto ALG = boolAlg();

    auto formula = generatorFormula<bool>(ALG, "XYZ", 5);

    std::cout << formula << std::endl;

    auto function = Function<bool>(formula, ALG);

    std::cout << function.getReversExpression() << std::endl;

    bool X(true);
    bool Y(false);
    bool Z(true);
    SetVariable<bool> setVariable{std::map<std::string, bool>{
            {"X", X},
            {"Y", Y},
            {"Z", Z},
    }};

    std::cout << function.call(setVariable) << std::endl;
}

#define NUMBER(name, n)\
OperatorFunction<int> name{\
        0, 6, std::string{n + '0'},\
        [](std::vector<int> operands){\
            return n;\
        }\
};

void example3() {
    NUMBER(number0, 0)
    NUMBER(number1, 1)
    NUMBER(number2, 2)
    NUMBER(number3, 3)
    NUMBER(number4, 4)
    NUMBER(number5, 5)
    NUMBER(number6, 6)
    NUMBER(number7, 7)
    NUMBER(number8, 8)
    NUMBER(number9, 9)

    OperatorFunction<int> a{
            1, 5, "!",
            [](std::vector<int> operands) {
                return 5 - operands[0];
            }, prefix
    };
    OperatorFunction<int> b{
            1, 5, "^",
            [](std::vector<int> operands) {
                return operands[0] * operands[0];
            }, postfix
    };

    SetOperator setOperator{std::vector{
            number0, number1, number2, number3, number4, number5, number6, number7, number8, number9,
            a, b,
    }};


    std::string formula =
            generatorFormula(setOperator,
                             "0123456789",
                             5);

    Function<int> function(formula, setOperator);

    std::cout << formula
              << " = "
              << function.getReversExpression()
              << " = "
              << function.call(SetVariable<int>(
                      std::map<std::string, int>{}))
              << std::endl;
}

int main() {
    std::cout << "Hello, World!" << std::endl;

//    example1();

    for (int i = 0; i < 10; ++i)
        example3();

    return 0;
}
