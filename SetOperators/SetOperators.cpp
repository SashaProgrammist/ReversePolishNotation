#include "SetOperators.h"

SetOperator<bool> boolAlg() {
    // Создание оператора "0"
    OperatorFunction<bool> zero{
            0, 6, "0",
            [](std::vector<bool> operands){
                return false;
            }
    };

    // Создание оператора "1"
    OperatorFunction<bool> one{
            0, 6, "1",
            [](std::vector<bool> operands){
                return true;
            }
    };

    // Создание оператора отрицания "\(\neg\)"
    OperatorFunction<bool> minus{
            1, 5, "!",
            [](std::vector<bool> operands){
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
    SetOperator result{std::vector{zero, one, minus, conjunction,
                                   disjunction, implication, equivalence}};

    return result;
}
