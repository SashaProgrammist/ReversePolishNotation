#include "OperatorFunction.h"

template<class T>
OperatorFunction<T>::OperatorFunction(size_t countOperands, size_t priority, std::string symbol,
                                      Variable<T> (*init)(std::vector<Variable<T>>)) :
        countOperands(countOperands),
        priority(priority),
        symbol(std::move(symbol)),
        init(init) {}

template<class T>
Variable<T> OperatorFunction<T>::callOperand(std::vector<Variable<T>> operands) {
    if (countOperands != operands.size())
        throw std::invalid_argument("number of operands does not match");

    Variable result = init(operands);

    for (auto operand: operands)
        operand.clear();

    return result;
}

template<class T>
size_t OperatorFunction<T>::getCountOperands() { return countOperands; }

template<class T>
size_t OperatorFunction<T>::getPriority() { return priority; }

template<class T>
std::string OperatorFunction<T>::getSymbol() { return symbol; }

template<class T>
SetOperator<T>::SetOperator(std::vector<OperatorFunction<T>> operators) {
    for (auto _operator: operators)
        set[_operator.getSymbol()] = _operator;
}

template<class T>
bool SetOperator<T>::isIn(std::string symbolOperator) {
    return set.find(symbolOperator) != set.end();
}

template<class T>
OperatorFunction<T> & SetOperator<T>::getOperator(std::string symbolOperator) {
    if (!this->isIn(symbolOperator))
        throw std::invalid_argument(symbolOperator + " not find");

    return set[symbolOperator];
}
