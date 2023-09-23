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
class SetOperator {
private:
    std::map<std::string, OperatorFunction<T>> set;

    std::string symbolsOperators{};

public:
    explicit SetOperator(std::vector<OperatorFunction<T>> operators) {
        for (auto _operator: operators) {
            set[_operator.getSymbol()] = _operator;
            symbolsOperators += _operator.getSymbol();
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
};

#endif //REVERSEPOLISHNOTATION_OPERATORFUNCTION_H
