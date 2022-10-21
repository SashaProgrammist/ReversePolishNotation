#ifndef REVERSEPOLISHNOTATION_FUNCTION_H
#define REVERSEPOLISHNOTATION_FUNCTION_H

#include <algorithm>
#include <iostream>

#include "../OperatorFunction/OperatorFunction.h"
#include "../Variable/Variable.h"


template<class T>
std::string getExpression(std::string &expression, SetOperator<T> &operators) {
    std::string result, buffer;

    for (auto symbol: expression) {
        if (operators.isIn(symbol)) {
            auto &currentOperator = operators.getOperator(symbol);

            //TODO postfix prefix Function

            while (!buffer.empty() && (buffer.end()[-1]) != '(' &&
                   operators.getOperator(buffer.end()[-1]).getPriority() >=
                   currentOperator.getPriority()) {
                result += buffer.end()[-1];
                buffer.pop_back();
            }

            buffer += symbol;
        } else
            switch (symbol) {
                case ',': //TODO
                    break;
                case '(':
                    buffer += symbol;
                    break;
                case ')':
                    while (!buffer.empty() && buffer.end()[-1] != '(') {
                        result += buffer.end()[-1];
                        buffer.pop_back();
                    }

                    if (buffer.empty())
                        throw std::invalid_argument("missing opening bracket");

                    buffer.pop_back();
                    break;
                default:
                    result += symbol;
                    break;
            }
    }

    while (!buffer.empty()) {
        result += buffer.end()[-1];
        buffer.pop_back();
    }

    return result;
}

template<class T>
std::string getName(std::string &expression, SetOperator<T> &operators) {
    std::string symbolsNotOperators;
    for (auto &symbol: expression)
        if (!operators.isIn(symbol))
            symbolsNotOperators += symbol;

    std::sort(symbolsNotOperators.begin(), symbolsNotOperators.end());
    size_t size = symbolsNotOperators.size();

    std::string result;
    for (int i = 0; i <= size; ++i)
        if (i == size ||
            symbolsNotOperators[i] != symbolsNotOperators[i + 1])
            result += symbolsNotOperators[i];

    return result;
}

template<class T>
class Function {
private:
    std::string expression;

    SetOperator<T> &operators;

public:
    std::string functionVariables;

    Function(std::string expression, SetOperator<T> &operators) :
            operators(operators) {
        this->expression = getExpression(expression, operators);
        this->functionVariables = getName(this->expression, operators);
    }

    T call(SetVariable<T> variables) {
        if (!variables.isEqualVariables(functionVariables))
            throw std::invalid_argument("functionVariables != nameVariables");

        std::vector<T> buffer;

        for (const auto &symbol: expression) {
            if (variables.isIn(symbol))
                buffer.push_back(variables.getVariable(symbol));
            else {
                auto &currentOperator =
                        operators.getOperator(symbol);
                size_t countOperands = currentOperator.getCountOperands();

                std::vector<T> currentOperands;
                for (size_t i = countOperands; i >= 1; --i) {
                    auto current = buffer.end()[-i];
                    currentOperands.push_back(current);
                }

                for (int i = 0; i < countOperands; ++i)
                    buffer.pop_back();

                buffer.push_back(currentOperator.callOperand(currentOperands));
            }
        }

        //TODO throw buffer.size != 1

        return buffer[0];
    }

    size_t getCountVariables() {
        return functionVariables.size();
    }
};


#endif //REVERSEPOLISHNOTATION_FUNCTION_H
