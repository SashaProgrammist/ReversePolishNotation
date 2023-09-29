#ifndef REVERSEPOLISHNOTATION_OPERATORFUNCTION_H
#define REVERSEPOLISHNOTATION_OPERATORFUNCTION_H

#include <vector>
#include <stdexcept>
#include <map>

#define GET_REVERS_GET_PRIORITY(name, n)\
if (isIn(expressions[n].second.end()[-1]))                               \
size_t name = getOperator(expressions[n].second.end()[-1]).getPriority(); \
size_t name = maxPriority + 1;

typedef enum {
    prefix,
    infix,
    postfix,
    uncertain
} NationType;

template<class T>
class OperatorFunction {
private:
    size_t countOperands{};
    size_t priority{};
    std::string symbol;

    T (*init)(std::vector<T>);

    NationType nationType = uncertain;

public:
    OperatorFunction() = default;

    OperatorFunction(size_t countOperands,
                     size_t priority,
                     std::string symbol,
                     T (*init)(std::vector<T>), NationType type = uncertain) :
            countOperands(countOperands),
            priority(priority),
            symbol(std::move(symbol)),
            init(init), nationType(type) {
        if (type == uncertain)
            switch (countOperands) {
                case 0:
                    nationType = postfix;
                    break;
                case 1:
                    nationType = prefix;
                    break;
                case 2:
                    nationType = infix;
                    break;
                default:
                    nationType = prefix;
                    break;
            }
    }

    [[nodiscard]] T callOperand(std::vector<T> operands) const {
        if (countOperands != operands.size())
            throw std::invalid_argument(
                    "number of operands does not match");

        T result = init(operands);

        return result;
    }


    [[nodiscard]] size_t getCountOperands() const { return countOperands; }

    [[nodiscard]] size_t getPriority() const { return priority; }

    [[nodiscard]] std::string getSymbol() const { return symbol; }

    [[nodiscard]] bool isPostfix() const { return nationType == postfix; }

    [[nodiscard]] bool isInfix() const { return nationType == infix; }

    [[nodiscard]] bool isPrefix() const { return nationType == prefix; }

    [[nodiscard]] bool isNolar() const { return countOperands == 0; }

    [[nodiscard]] NationType getNationType() const { return nationType; }
};

template<class T>
class SetOperator;

template<class T>
std::string getExpression(std::string &expression,
                          SetOperator<T> &operators);

template<class T>
class SetOperator {
private:
    std::map<std::string, OperatorFunction<T>> set;

    std::string symbolsOperators{};

    size_t maxPriority{};

    size_t countNolars{};

    size_t countOperators{};

public:
    explicit SetOperator(
            std::vector<OperatorFunction<T>> operators) :
            countOperators(operators.size()) {
        for (auto _operator: operators) {
            set[_operator.getSymbol()] = _operator;
            symbolsOperators += _operator.getSymbol();

            if (maxPriority < _operator.getPriority())
                maxPriority = _operator.getPriority();

            if (_operator.isNolar())
                countNolars++;
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

    OperatorFunction<T> &getOperator(
            std::string symbolOperator) {
        if (!isIn(symbolOperator))
            throw std::invalid_argument(symbolOperator +
                                        " not find");

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

    std::pair<std::string, std::string>
    getRevers(std::vector<std::pair<std::string, std::string>> expressions,
              std::string symbolOperator) {
        OperatorFunction<T> _operator = getOperator(symbolOperator);

        if (expressions.size() != _operator.getCountOperands())
            throw std::invalid_argument(
                    "expressions.size() != "
                    "_operator.getCountOperands()");


        switch (_operator.getCountOperands()) {
            case 0: {
                return {symbolOperator, symbolOperator};
            }
            case 1: {
                std::string expression = expressions[0].second;
                if (isIn(expression.end()[-1])) {
                    OperatorFunction<T> pastOperator = getOperator(expression.end()[-1]);
                    if (_operator.isPrefix()) {
                        if (pastOperator.isPrefix() ||
                            (pastOperator.isPostfix() && pastOperator.isNolar()))
                            return {symbolOperator + expressions[0].first,
                                    expressions[0].second + symbolOperator};
                        else
                            return {symbolOperator + "(" + expressions[0].first + ")",
                                    expressions[0].second + symbolOperator};
                    } else if (_operator.isPostfix())
                        switch (pastOperator.getNationType()) {
                            case postfix:
                            case prefix:
                                return {expressions[0].first + symbolOperator,
                                        expressions[0].second + symbolOperator};
                            case infix:
                                return {"(" + expressions[0].first + ")" + symbolOperator,
                                        expressions[0].second + symbolOperator};
                        }

                    size_t priorityPastOperator = pastOperator.getPriority();
                    if (priorityPastOperator <= _operator.getPriority())
                        return {symbolOperator + "(" + expressions[0].first + ")",
                                expressions[0].second + symbolOperator};
                    else
                        return {symbolOperator + expressions[0].first,
                                expressions[0].second + symbolOperator};
                } else if (_operator.isPrefix() || _operator.isInfix())
                    return {symbolOperator + expressions[0].first,
                            expressions[0].second + symbolOperator};
                else
                    return {expressions[0].first + symbolOperator,
                            expressions[0].second + symbolOperator};
            }
            case 2: {
                if (_operator.isPrefix())
                    return {symbolOperator + "(" + expressions[0].first + "," + expressions[1].first + ")",
                            expressions[0].second + expressions[1].second + symbolOperator};
                else if (_operator.isPostfix())
                    return {"(" + expressions[0].first + "," + expressions[1].first + ")" + symbolOperator,
                            expressions[0].second + expressions[1].second + symbolOperator};

//                std::string result = "(";
                std::string result;

                if (isIn(expressions[0].second.end()[-1])) {
                    OperatorFunction<T> leftPastOperator = getOperator(expressions[0].second.end()[-1]);
                    if (!leftPastOperator.isInfix()) {
                        switch (leftPastOperator.getNationType()) {
                            case prefix:
                            case postfix:
                                result += expressions[0].first + symbolOperator;
                                break;
                            default:
                                result += "(" + expressions[0].first + ")" + symbolOperator;
                        }
                    } else {
                        size_t priorityLeftPastOperator = leftPastOperator.getPriority();
                        if (priorityLeftPastOperator < _operator.getPriority())
                            result += "(" + expressions[0].first + ")" +
                                      symbolOperator;
                        else
                            result += expressions[0].first + symbolOperator;
                    }
                } else
                    result += expressions[0].first + symbolOperator;


                if (isIn(expressions[1].second.end()[-1])) {
                    auto leftPastOperator = getOperator(expressions[1].second.end()[-1]);
                    if (!leftPastOperator.isInfix()) {
                        switch (leftPastOperator.getNationType()) {
                            case prefix:
                            case postfix:
                                result += expressions[1].first;
                                break;
                            default:
                                result += "(" + expressions[1].first + ")";
                        }
                    } else {
                        size_t PriorityRightPastOperator = leftPastOperator.getPriority();
                        if (PriorityRightPastOperator <=
                            _operator.getPriority())
                            result += "(" + expressions[1].first + ")";
                        else
                            result += expressions[1].first;
                    }
                } else
                    result += expressions[1].first;

//                result += ")";


                return {result,
                        expressions[0].second + expressions[1].second + symbolOperator};
            }
            default: {
                std::string resultFirst = symbolOperator + "(";
                std::string resultSecond;

                for (int i = 1; i < _operator.getCountOperands(); ++i)
                    resultFirst += expressions[i - 1].first + ",";
                resultFirst += expressions.end()[-1].first + ")";

                for (int i = 0; i < _operator.getCountOperands(); ++i) {
                    resultSecond += expressions[i].second;
                }
                resultSecond += symbolOperator;

                return {resultFirst, resultSecond};
            }
        }
    }

    std::pair<std::string, std::string>
    getRevers(std::vector<std::pair<std::string, std::string>> expressions,
              char symbolOperator) {
        return getRevers(expressions, std::string{symbolOperator});
    }

    size_t getCountNolares() {
        return countNolars;
    }

    size_t getCountNonNolares() {
        return countOperators - countNolars;
    }
};

#endif //REVERSEPOLISHNOTATION_OPERATORFUNCTION_H
