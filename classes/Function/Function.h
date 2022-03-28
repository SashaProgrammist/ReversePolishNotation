#ifndef REVERSEPOLISHNOTATION_FUNCTION_H
#define REVERSEPOLISHNOTATION_FUNCTION_H

#include <algorithm>

#include "../OperatorFunction/OperatorFunction.h"
#include "../Variable/Variable.h"

template<class T>
class Function {
private:
    std::string expression;

    SetOperator<T> operators;

public:
    std::string functionVariables;

    Function(std::string expression, SetOperator<T> &operators);

    Variable<T> call(SetVariable<T> variables);

    size_t getCountVariables();
};


#endif //REVERSEPOLISHNOTATION_FUNCTION_H
