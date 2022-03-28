#ifndef REVERSEPOLISHNOTATION_OPERATORFUNCTION_H
#define REVERSEPOLISHNOTATION_OPERATORFUNCTION_H

#include <vector>
#include <stdexcept>

#include "../Variable/Variable.h"

template<class T>
class OperatorFunction {
private:
    size_t countOperands;
    size_t priority;
    std::string symbol;

    Variable<T> (*init)(std::vector<Variable<T>>);

public:
    OperatorFunction(size_t countOperands,
                     size_t priority,
                     std::string symbol,
                     Variable<T> (*init)(std::vector<Variable<T>>));

    Variable<T> callOperand(std::vector<Variable<T>> operands);

    size_t getCountOperands();

    size_t getPriority();

    std::string getSymbol();
};

template<class T>
class SetOperator {
private:
    std::map<std::string, OperatorFunction<T>> set;

public:
    explicit SetOperator(std::vector<OperatorFunction<T>> operators);

    bool isIn(std::string symbolOperator);

    OperatorFunction<T> & getOperator(std::string symbolOperator);
};


#endif //REVERSEPOLISHNOTATION_OPERATORFUNCTION_H
