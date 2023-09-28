#ifndef REVERSEPOLISHNOTATION_GENERATORFORMULA_H
#define REVERSEPOLISHNOTATION_GENERATORFORMULA_H

#include <random>

#include "../classes/Function/Function.h"

size_t getRandom();

template<typename T>
std::string generatorFormula(SetOperator<T> &setOperator,
                             std::string variables, size_t count) {
    if (count) {
        auto symbols = setOperator.getSymbolsOperators();

        auto symbol = std::string{symbols[getRandom() %
                                          symbols.size()]};
        size_t countOperands =
                setOperator.getOperator(symbol).getCountOperands();

        if (!countOperands) {
            size_t countNolares = setOperator.getCountNolares();
            size_t countNonNolares = setOperator.getCountNonNolares();
            size_t sum = 0;

            while (!countOperands && sum < countNolares) {
                symbol = std::string{symbols[getRandom() %
                                             symbols.size()]};
                countOperands =
                        setOperator.getOperator(symbol).getCountOperands();
                sum += countNonNolares;
            }
        }

        if (!countOperands)
            return symbol;
        else if (countOperands == 1) {
            auto _operator = setOperator.getOperator(symbol);

            if (_operator.isPostfix())
                return "(" + generatorFormula(
                        setOperator, variables, count - 1) + symbol + ")";

            return "(" + symbol + generatorFormula(
                    setOperator, variables, count - 1) + ")";

        } else if (countOperands == 2) {
            auto _operator = setOperator.getOperator(symbol);
            switch (_operator.getNationType()) {
                case prefix:
                    return "(" +
                           symbol + +"(" +
                           generatorFormula(setOperator, variables, count - 1) + "," +
                           generatorFormula(setOperator, variables, count - 1) +
                           "))";
                default: // infix
                    return "(" +
                           generatorFormula(setOperator, variables, count - 1) +
                           symbol +
                           generatorFormula(setOperator, variables, count - 1) +
                           ")";
                case postfix: {
                    return "(" +
                           generatorFormula(setOperator, variables, count - 1) + "," +
                           generatorFormula(setOperator, variables, count - 1) +
                           ")" +
                           symbol;
                }
            }
        }


        std::string result = "(" + symbol + "(";

        for (size_t i = 0; i < countOperands; ++i)
            result += generatorFormula(setOperator,
                                       variables,
                                       count - 1) +
                      ",";
        result += "))";

        return result;

    } else
        return std::string{variables[getRandom() % variables.size()]};
}

#endif //REVERSEPOLISHNOTATION_GENERATORFORMULA_H
