#include "SetOperators.h"

SetOperator<bool> boolAlg() {
    OperatorFunction<bool> minus{
            1, 5, "!",
            [](std::vector<bool> operands){
                return !operands[0];
            }
    };
    OperatorFunction<bool> conjunction{
            2, 4, "&",
            [](std::vector<bool> operands) {
                return operands[0] && operands[1];
            }
    };
    OperatorFunction<bool> disjunction{
            2, 3, "|",
            [](std::vector<bool> operands) {
                return operands[0] || operands[1];
            }
    };
    OperatorFunction<bool> implication{
            2, 2, ">",
            [](std::vector<bool> operands) {
                return !operands[0] || operands[1];
            }
    };
    OperatorFunction<bool> equivalence{
            2, 1, "~",
            [](std::vector<bool> operands) {
                return operands[0] == operands[1];
            }
    };

    SetOperator result{std::vector{minus, conjunction, disjunction, implication, equivalence}};

    return result;
}
