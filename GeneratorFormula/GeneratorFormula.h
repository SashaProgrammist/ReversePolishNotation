#ifndef REVERSEPOLISHNOTATION_GENERATORFORMULA_H
#define REVERSEPOLISHNOTATION_GENERATORFORMULA_H

#include <random>

#include "../classes/Function/Function.h"

size_t getRandom();

template<typename T>
std::string generatorFormula(SetOperator<T> &setOperator,
                             std::string variables, size_t count) {
    if (count) {
        // Получаем список символов операторов из объекта setOperator
        auto symbols = setOperator.getSymbolsOperators();

        // Выбираем случайный символ оператора
        auto symbol = std::string{symbols[getRandom() %
                                          symbols.size()]};
        size_t countOperands =
                setOperator.getOperator(symbol).getCountOperands();

        // Если оператор не имеет операндов,
        // выбираем другой символ оператора
        // с некоторым шансом
        if (!countOperands) {
            size_t countNolares = setOperator.getCountNolares();
            size_t countNonNolares = setOperator.getCountNonNolares();
            size_t sum = 0;

            while (!countOperands && sum < countNolares) {
                symbol = std::string{symbols[getRandom() %
                                             symbols.size()]};
                countOperands =
                        setOperator.getOperator(symbol).
                                getCountOperands();
                sum += countNonNolares;
            }
        }

        // Генерируем формулу на основе выбранного оператора
        if (!countOperands)
            return symbol;
        else if (countOperands == 1) {
            auto _operator = setOperator.getOperator(symbol);

            // Если оператор является постфиксным,
            // генерируем формулу в постфиксной нотации
            if (_operator.isPostfix())
                return "(" + generatorFormula(
                        setOperator, variables, count - 1) +
                       symbol + ")";

            // Иначе генерируем формулу в инфиксной нотации
            return "(" + symbol + generatorFormula(
                    setOperator, variables, count - 1) + ")";

        } else if (countOperands == 2) {
            auto _operator = setOperator.getOperator(symbol);
            switch (_operator.getNotationType()) {
                case prefix:
                    // Генерируем формулу в префиксной нотации
                    return "(" +
                           symbol + +"(" +
                           generatorFormula(setOperator,
                                            variables,
                                            count - 1) + "," +
                           generatorFormula(setOperator,
                                            variables,
                                            count - 1) +
                           "))";
                default: // infix
                    // Генерируем формулу в инфиксной нотации
                    return "(" +
                           generatorFormula(setOperator,
                                            variables,
                                            count - 1) +
                           symbol +
                           generatorFormula(setOperator,
                                            variables,
                                            count - 1) +
                           ")";
                case postfix: {
                    // Генерируем формулу в постфиксной нотации
                    return "((" +
                           generatorFormula(setOperator,
                                            variables,
                                            count - 1) + "," +
                           generatorFormula(setOperator,
                                            variables,
                                            count - 1) +
                           ")" +
                           symbol + ")";
                }
            }
        }

        auto _operator = setOperator.getOperator(symbol);
        switch (_operator.getNotationType()) {
            case prefix: {
                // Генерируем формулу в префиксной
                // нотации с несколькими операндами
                std::string result = "(" + symbol + "(";

                for (size_t i = 0; i < countOperands; ++i)
                    result += generatorFormula(setOperator,
                                               variables,
                                               count - 1) +
                              ",";
                result.pop_back();
                result += "))";

                return result;
            }
            case postfix: {
                // Генерируем формулу в постфиксной
                // нотации с несколькими операндами
                std::string result = "((";

                for (size_t i = 0; i < countOperands; ++i)
                    result += generatorFormula(setOperator,
                                               variables,
                                               count - 1) +
                              ",";
                result.pop_back();
                result += ")" + symbol + ")";

                return result;
            }
            default: { // infix
                // Генерируем формулу в инфиксной нотации с несколькими операндами
                std::string result = "(" +
                                     generatorFormula(setOperator,
                                                      variables,
                                                      count - 1) +
                                     symbol;
                for (size_t i = 1; i < countOperands; ++i)
                    result += generatorFormula(setOperator,
                                               variables,
                                               count - 1) +
                              ":";
                result.pop_back();
                result += ")";

                return result;
            }
        }


    } else
        // Возвращаем случайную переменную, если count равен 0
        return std::string{variables[getRandom() % variables.size()]};
}

#endif //REVERSEPOLISHNOTATION_GENERATORFORMULA_H
