#include "SetOperators.h"

SetOperator<bool> boolAlg() {
    // Создание оператора "0"
    OperatorFunction<bool> zero{
            "0", false};

    // Создание оператора "1"
    OperatorFunction<bool> one{
            "1", true};

    // Создание оператора отрицания "\(\neg\)"
    OperatorFunction<bool> minus{
            1, 5, "!",
            [](std::vector<bool> operands) {
                return !operands[0];
            }
    };

    // Создание оператора конъюнкции "\&"
    OperatorFunction<bool> conjunction{
            2, 4, "&",
            [](std::vector<bool> operands) {
                return operands[0] && operands[1];
            }
    };

    // Создание оператора дизъюнкции "\(\vee\)"
    OperatorFunction<bool> disjunction{
            2, 3, "|",
            [](std::vector<bool> operands) {
                return operands[0] || operands[1];
            }
    };

    // Создание оператора импликации "\(\rightarrow\)"
    OperatorFunction<bool> implication{
            2, 2, ">",
            [](std::vector<bool> operands) {
                return !operands[0] || operands[1];
            }
    };

    // Создание оператора эквивалентности "\(\leftrightarrow\)"
    OperatorFunction<bool> equivalence{
            2, 1, "~",
            [](std::vector<bool> operands) {
                return operands[0] == operands[1];
            }
    };

    // Создание объекта SetOperator, содержащего все операторы
    SetOperator result{
            std::vector{zero, one, minus, conjunction,
                        disjunction, implication, equivalence}};

    return result;
}

#define octagonOfResonanceRing_digit(name, n) \
OperatorFunction<small> name{                 \
        std::string{n + '0'},                 \
        n                                     \
};

SetOperator<small> octagonOfResonanceRing() {
    octagonOfResonanceRing_digit(zero, 0)
    octagonOfResonanceRing_digit(one, 1)
    octagonOfResonanceRing_digit(two, 2)
    octagonOfResonanceRing_digit(three, 3)
    octagonOfResonanceRing_digit(four, 4)
    octagonOfResonanceRing_digit(five, 5)
    octagonOfResonanceRing_digit(six, 6)
    octagonOfResonanceRing_digit(seven, 7)

    OperatorFunction<small> i{"i", 2};
    OperatorFunction<small> j{"j", 4};

    OperatorFunction<small> plus{
            2, 0, "+",
            [](std::vector<small> operands) -> small {
                return (operands[0] ^ operands[1]) & 7;
            }
    };
    OperatorFunction<small> multiply{
            2, 1, "*",
            [](std::vector<small> operands) -> small {
                std::vector masks{
                        operands[1] & 1 ? 7 : 0,
                        operands[1] & 2 ? 7 : 0,
                        operands[1] & 4 ? 7 : 0,
                };

                return (operands[0] & masks[0]) ^
                       ((operands[0] << 1 |
                         (operands[0] >> 2) & 1) & masks[1]) ^
                       ((operands[0] << 2 |
                         (operands[0] >> 1) & 3) & masks[2]);
            }
    };

    return SetOperator<small>(
            std::vector({plus, multiply,
                         zero, one, two, three, four,
                         five, six, seven, i, j}));
}

#define haloOfVariableWorldsRing_concatenate(name, n) \
OperatorFunction<unsigned> name{                      \
        1, 0, std::string{n + '0'},                   \
        [](std::vector<unsigned> operands) {          \
            return operands[0] * 10 + n;              \
        }, postfix                                    \
};

SetOperator<unsigned> haloOfVariableWorldsRing() {
    haloOfVariableWorldsRing_concatenate(zero, 0)
    haloOfVariableWorldsRing_concatenate(one, 1)
    haloOfVariableWorldsRing_concatenate(two, 2)
    haloOfVariableWorldsRing_concatenate(three, 3)
    haloOfVariableWorldsRing_concatenate(four, 4)
    haloOfVariableWorldsRing_concatenate(five, 5)
    haloOfVariableWorldsRing_concatenate(six, 6)
    haloOfVariableWorldsRing_concatenate(seven, 7)
    haloOfVariableWorldsRing_concatenate(eight, 8)
    haloOfVariableWorldsRing_concatenate(nine, 9)

    OperatorFunction<unsigned> newNumber{"_", 0};

    OperatorFunction<unsigned> plus{
            2, 0, "+",
            [](std::vector<unsigned> operands) {
                return operands[0] ^ operands[1];
            }
    };
    OperatorFunction<unsigned> multiply{
            2, 1, "*",
            [](std::vector<unsigned> operands) -> unsigned {
                unsigned result = 0;

                while (operands[0]) {
                    result ^= operands[1] *
                              bool(operands[0] & 1);
                    operands[0] >>= 1;
                    operands[1] <<= 1;
                }

                return result;
            }
    };

    return SetOperator<unsigned>(
            std::vector({plus, multiply, newNumber,
                         zero, one, two, three, four, five,
                         six, seven, eight, nine,}));
}
