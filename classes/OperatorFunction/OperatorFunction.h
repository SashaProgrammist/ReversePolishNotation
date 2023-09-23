#ifndef REVERSEPOLISHNOTATION_OPERATORFUNCTION_H
#define REVERSEPOLISHNOTATION_OPERATORFUNCTION_H

#include <vector>
#include <stdexcept>
#include <map>

template<class T>
class OperatorFunction {
private:
    size_t countOperands{};
    size_t priority{};
    std::string symbol;

    T (*init)(std::vector<T>);

public:
    OperatorFunction() = default;

    OperatorFunction(size_t countOperands, size_t priority, std::string symbol,
                     T (*init)(std::vector<T>)) :
            countOperands(countOperands),
            priority(priority),
            symbol(std::move(symbol)),
            init(init) {}

    T callOperand(std::vector<T> operands) {
        if (countOperands != operands.size())
            throw std::invalid_argument("number of operands does not match");

        T result = init(operands);

        return result;
    }


    size_t getCountOperands() { return countOperands; }

    size_t getPriority() { return priority; }

    std::string getSymbol() { return symbol; }
};

template<class T>
class SetOperator;

template<class T>
std::string getExpression(std::string &expression, SetOperator<T> &operators);

template<class T>
class SetOperator {
private:
    std::map<std::string, OperatorFunction<T>> set;

    std::string symbolsOperators{};

    size_t maxPriority{};

public:
    explicit SetOperator(std::vector<OperatorFunction<T>> operators) {
        for (auto _operator: operators) {
            set[_operator.getSymbol()] = _operator;
            symbolsOperators += _operator.getSymbol();

            if (maxPriority < _operator.getPriority())
                maxPriority = _operator.getPriority();
        }
    }

    [[nodiscard]] std::string getSymbolsOperators() const {
        return symbolsOperators;
    }

    bool isIn(std::string symbolOperator) {
        return set.find(symbolOperator) != set.end();
    }

    bool isIn(char symbolOperator) {
        return isIn(std::string{symbolOperator});
    }

    OperatorFunction<T> &getOperator(std::string symbolOperator) {
        if (!isIn(symbolOperator))
            throw std::invalid_argument(symbolOperator + " not find");

        return set[symbolOperator];
    }

    OperatorFunction<T> &getOperator(char symbolOperator) {
        return getOperator(std::string{symbolOperator});
    }

    size_t getPriorityLastOperator(std::string expression) {
        expression = getExpression(expression, *this);
        expression = expression.end()[-1];
        if (isIn(expression))
            return getOperator(expression).getPriority();
        else
            return maxPriority + 1;
    }

    std::string getRevers(std::vector<std::string> expressions, std::string symbolOperator) {
        OperatorFunction<T> _operator = getOperator(symbolOperator);

        if (expressions.size() != _operator.getCountOperands())
            throw std::invalid_argument("expressions.size() != _operator.getCountOperands()");


        switch (_operator.getCountOperands()) {
            case 1: {
                size_t priorityPastOperator = getPriorityLastOperator(expressions[0]);
                if (priorityPastOperator <= _operator.getPriority())
                    return symbolOperator + "(" + expressions[0] + ")";
                else
                    return symbolOperator + expressions[0];
            }
            case 2: {
                std::string result;

                size_t priorityLeftPastOperator = getPriorityLastOperator(expressions[0]);

                if (priorityLeftPastOperator < _operator.getPriority())
                    result += "(" + expressions[0] + ")" + symbolOperator;
                else
                    result += expressions[0] + symbolOperator;

                size_t PriorityRightPastOperator = getPriorityLastOperator(expressions[1]);

                if (PriorityRightPastOperator <= _operator.getPriority())
                    result += "(" + expressions[1] + ")";
                else
                    result += expressions[1];

                return result;
            }
            default: {
                std::string result = symbolOperator + "(";

                for (int i = 1; i < _operator.getCountOperands(); ++i)
                    result += expressions[i - 1] + ",";
                result += expressions.end()[-1] + ")";

                return result;
            }
        }
    }

    std::string getRevers(std::vector<std::string> expressions, char symbolOperator) {
        return getRevers(expressions, std::string{symbolOperator});
    }
};

#endif //REVERSEPOLISHNOTATION_OPERATORFUNCTION_H
