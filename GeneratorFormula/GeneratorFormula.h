#ifndef REVERSEPOLISHNOTATION_GENERATORFORMULA_H
#define REVERSEPOLISHNOTATION_GENERATORFORMULA_H

#include <random>

#include "../classes/Function/Function.h"

size_t getRandom();

template<typename T>
std::string generatorFormula(SetOperator<T> setOperator,
                             std::string variables, size_t count) {
    if (count) {
        auto symbols = setOperator.getSymbolsOperators();

        auto symbol = std::string{symbols[getRandom() %
                                          symbols.size()]};
        size_t countOperands =
                setOperator.getOperator(symbol).getCountOperands();

        if (!countOperands) {
            symbol = std::string{symbols[getRandom() %
                                         symbols.size()]};
            countOperands =
                    setOperator.getOperator(symbol).getCountOperands();
        }

        if (!countOperands)
            return symbol;
        else if (countOperands == 1)
            return "(" + symbol + generatorFormula(
                    setOperator, variables, count - 1) + ")";
        else if (countOperands == 2)
            return "(" +
                   generatorFormula(setOperator, variables, count - 1) +
                   symbol +
                   generatorFormula(setOperator, variables, count - 1) +
                   ")";

        std::string result = "(" + symbol + "(";

        for (size_t i = 1; i < countOperands; ++i)
            result += generatorFormula(setOperator,
                                       variables,
                                       count - 1) +
                      ",";
        result += generatorFormula(setOperator,
                                   variables,
                                   count - 1) +
                  "))";

        return result;

    } else
        return std::string{variables[getRandom() % variables.size()]};
}

#endif //REVERSEPOLISHNOTATION_GENERATORFORMULA_H
