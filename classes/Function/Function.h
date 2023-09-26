#ifndef REVERSEPOLISHNOTATION_FUNCTION_H
#define REVERSEPOLISHNOTATION_FUNCTION_H

#include <algorithm>
#include <iostream>

#include "../OperatorFunction/OperatorFunction.h"
#include "../Variable/Variable.h"

#define FREE_BUFFER_BEFORE_BRACKET                          \
while (!buffer.empty() && buffer.end()[-1] != '(') {        \
    result += buffer.end()[-1];                             \
    buffer.pop_back();                                      \
}                                                           \
if (buffer.empty())                                         \
    throw std::invalid_argument("missing opening bracket");

#define ALGORITHM_OF_CALL(pushVariable, pushResult, T)             \
for (const auto &symbol: expression) {                             \
    if (operators.isIn(symbol)){                                   \
        auto &currentOperator =                                    \
                operators.getOperator(symbol);                     \
        size_t countOperands = currentOperator.getCountOperands(); \
                                                                   \
        if (countOperands > buffer.size())                         \
            throw std::invalid_argument("invalid expression");     \
                                                                   \
        std::vector<T> currentOperands;                            \
        for (size_t i = countOperands; i >= 1; --i) {              \
            auto current = buffer.end()[-i];                       \
            currentOperands.push_back(current);                    \
        }                                                          \
                                                                   \
        for (int i = 0; i < countOperands; ++i)                    \
            buffer.pop_back();                                     \
                                                                   \
        pushResult;                                                \
    } else                                                         \
        pushVariable;                                              \
}

template<class T>
std::string getExpression(std::string &expression,
                          SetOperator<T> &operators) {
    std::string result, buffer;

    for (auto symbol: expression) {
        if (operators.isIn(symbol)) {
            auto &currentOperator =
                    operators.getOperator(symbol);

            //TODO postfix prefix Function

            while (!buffer.empty() && (buffer.end()[-1]) != '(' &&
                   operators.getOperator(buffer.end()[-1]).
                           getPriority() >=
                   currentOperator.getPriority()) {
                result += buffer.end()[-1];
                buffer.pop_back();
            }

            buffer += symbol;
        } else
            switch (symbol) {
                case ',':
                FREE_BUFFER_BEFORE_BRACKET
                    break;
                case '(':
                    buffer += symbol;
                    break;
                case ')':
                FREE_BUFFER_BEFORE_BRACKET

                    buffer.pop_back();
                    break;
                default:
                    result += symbol;
                    break;
            }
    }

    while (!buffer.empty()) {
        auto symbol = buffer.end()[-1];
        buffer.pop_back();

        if (symbol == '(')
            throw std::invalid_argument("missing closing bracket");

        result += symbol;
    }

    return result;
}

template<class T>
std::string getName(std::string &expression,
                    SetOperator<T> &operators) {
    std::string symbolsNotOperators;
    for (auto &symbol: expression)
        if (!operators.isIn(symbol))
            symbolsNotOperators += symbol;

    std::sort(symbolsNotOperators.begin(),
              symbolsNotOperators.end());
    size_t size = symbolsNotOperators.size();

    std::string result;
    for (int i = 0; i < size; ++i)
        if (symbolsNotOperators[i] !=
            symbolsNotOperators[i + 1])
            result += symbolsNotOperators[i];

    return result;
}

template<class T>
class Function {
private:
    std::string expression;

    SetOperator<T> &operators;

    std::string functionVariables;

public:
    Function(std::string expression, SetOperator<T> &operators) :
            operators(operators) {
        this->expression = getExpression(expression, operators);
        this->functionVariables = getName(this->expression,
                                          operators);

        int counter{};
        for (auto symbol: this->expression)
            if (operators.isIn(symbol))
                counter += operators.getOperator(symbol).getCountOperands() - 1;
            else
                counter -= 1;

        if (counter != -1)
            throw std::invalid_argument("invalid expression");
    }

    T call(SetVariable<T> variables) {
        if (!variables.isEqualVariables(functionVariables))
            throw std::invalid_argument(
                    "functionVariables != nameVariables");

        std::vector<T> buffer;

        ALGORITHM_OF_CALL(
                buffer.push_back(variables.getVariable(symbol)),
                buffer.push_back(currentOperator.
                callOperand(currentOperands)),
                T)

        if (buffer.size() != 1)
            throw std::invalid_argument("invalid expression");

        return buffer[0];
    }

    size_t getCountVariables() {
        return functionVariables.size();
    }

    std::string getFunctionExpression() {
        return expression;
    }

    std::string getFunctionVariables() {
        return functionVariables;
    }

    std::string getReversExpression() {
        std::vector<std::string> buffer;

        ALGORITHM_OF_CALL(
                buffer.push_back(std::string{symbol}),
                buffer.push_back(operators.getRevers(currentOperands,
                                                     symbol)),
                std::string)

        if (buffer.size() != 1)
            throw std::invalid_argument("invalid expression");

        return buffer[0];
    }
};


#endif //REVERSEPOLISHNOTATION_FUNCTION_H
