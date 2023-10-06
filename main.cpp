#include <iostream>
#include <cassert>

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

    auto formula = generatorFormula<bool>(ALG, "01", 5);

    std::cout << formula << std::endl;

    auto function = Function<bool>(formula, ALG);

    std::cout << function.getReversExpression() << std::endl;

    SetVariable<bool> setVariable{std::map<std::string, bool>{}};

    std::cout << function.call(setVariable) << std::endl;
}

// Макрос для создания оператора, представляющего константное число.
#define NUMBER(name, n)\
OperatorFunction<int> name{\
        0, 6, std::string{n + '0'},\
        []([[maybe_unused]] std::vector<int> operands){\
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
    OperatorFunction<int> c{
            2, 4, "+",
            [](std::vector<int> operands) {
                return operands[0] + operands[1];
            }, prefix
    };
    OperatorFunction<int> d{
            2, 4, "/",
            [](std::vector<int> operands) {
                if (operands[1])
                    return operands[0] / operands[1];
                return 0;
            }, postfix
    };
    OperatorFunction<int> e{
            2, 3, "-",
            [](std::vector<int> operands) {
                return operands[0] - operands[1];
            }, infix
    };
    OperatorFunction<int> f{
            2, 2, "$",
            [](std::vector<int> operands) {
                return operands[0] - operands[1];
            }, infix
    };
    OperatorFunction<int> g{
            2, 1, "&",
            [](std::vector<int> operands) {
                return operands[0] - operands[1];
            }, infix
    };
    OperatorFunction<int> k{
            3, 0, "?",
            [](std::vector<int> operands) {
                return operands[0] ? operands[1] : operands[2];
            }, infix
    };

    SetOperator setOperator{std::vector{
            number0, number1, number2, number3, number4,
            number5, number6, number7, number8, number9,
            a, b, c, d, e, f, g, k
    }};

    for (int i = 0; i < 1000; ++i) {
        std::string formula1 =
                generatorFormula(setOperator,
                                 "0123456789ABCD",
                                 6);
        try {
            Function<int> function1(formula1, setOperator);
            std::string formula2 = function1.getReversExpression();
            Function<int> function2(formula2, setOperator);

            if (function1.getFunctionExpression() !=
                function2.getFunctionExpression())
                std::cout << formula1 << std::endl
                          << formula2 << std::endl
                          << function1.getFunctionExpression() << std::endl
                          << function2.getFunctionExpression() << std::endl;


            assert(function1.getFunctionExpression() ==
                   function2.getFunctionExpression());
        } catch (std::invalid_argument &e) {
            std::cout << formula1 << std::endl;

            Function<int> function1(formula1, setOperator);
            std::string formula2 = function1.getReversExpression();
            std::cout << formula2 << std::endl;

            Function<int> function2(formula2, setOperator);
            std::cout << function1.getFunctionExpression() << std::endl
                      << function2.getFunctionExpression() << std::endl;

            throw e;
        }
    }

    for (int i = 0; i < 10; ++i) {
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

    while (true) {
        std::string formula;
        std::cin >> formula;

        try {
            Function<int> function(formula, setOperator);

            std::cout << formula
                      << " = "
                      << function.getReversExpression()
                      << " = "
                      << function.call(SetVariable<int>(
                              std::map<std::string, int>{}))
                      << std::endl;
        } catch (std::invalid_argument &e) {
            std::cout << e.what() << std::endl;
        }
    }
}

void example4() {
    // Создание операторов для чисел от 0 до 3.
    NUMBER(number0, 0)
    NUMBER(number1, 1)
    NUMBER(number2, 2)
    NUMBER(number3, 3)

    // Оператор унарного минуса.
    OperatorFunction<int> unaryMinus{
            1, 4, "-", // Один операнд, приоритет 4, символ "-".
            [](std::vector<int> operands) {
                return -operands[0]; // Префиксный унарный минус.
            }, prefix // Указываем, что это префиксный оператор.
    };

    // Оператор сложения.
    OperatorFunction<int> plus{
            2, 3, "+", // два операнда, приоритет 3, символ "+".
            [](std::vector<int> operands) {
                return operands[0] + operands[1];
            }, infix // Указываем, что это инфиксный оператор.
    };

    // Оператор сравнения
    OperatorFunction<int> lessThan{
            2, 2, "<", // два операнда, приоритет 2, символ "<".
            [](std::vector<int> operands) {
                return int(operands[0] < operands[1]);
            }, infix // Указываем, что это инфиксный оператор.
    };

    // Оператор тернарного условия (инфиксный).
    OperatorFunction<int> ternary{
            3, 1, "?", // три операнда, приоритет 1, символ "?".
            [](std::vector<int> operands) {
                return operands[0] ? operands[1] : operands[2];
            }, infix // Указываем, что это инфиксный оператор.
    };

    // Оператор с четырьмя операндами (инфиксный).
    OperatorFunction<int> quadronary{
            4, 0, "#", // четыре операнда, приоритет 0, символ "#".
            [](std::vector<int> operands) {
                return operands[0] * operands[1] -
                       operands[2] * operands[3];
            }, infix // Указываем, что это инфиксный оператор.
    };

    // Создание множества операторов.
    SetOperator<int> setOperator{std::vector{
            number0, number1, number2, number3,
            ternary, plus, quadronary, lessThan,
            unaryMinus
    }};

    // Примеры выражений.
    auto examples = std::vector{
            "a<b?a:b",
            "0<x?x:(x<0?-x:0)",
            "0+1?(3#2+2:2+1:0):3+2",
            "1#1?2:1:0?2:1:1",
    };

    // Проход по примерам и вывод результатов.
    for (auto expression: examples) {
        std::string current = expression;
        std::cout << expression << " ~ "
                  << getExpression(current,
                                   setOperator)
                  << std::endl;
    }

//    for (auto expression: examples) {
//        Function<int> function(expression, setOperator);
//
//        std::cout << expression << std::endl
//                  << function.getFunctionExpression()
//                  << std::endl
//                  << function.call(SetVariable<int>(
//                          std::map<std::string, int>{}))
//                  << std::endl << std::endl;
//    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    example3();

    return 0;
}
