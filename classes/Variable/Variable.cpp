#include "Variable.h"

template<class T>
T & Variable<T>::getValue(){
    return value;
}

template<class T>
void Variable<T>::clear() {
    value.clrar();
}

template<class T>
T Variable<T>::copy() {
    return value.copy();
}

template<class T>
Variable<T>::Variable(T variable) {
    value = variable;
}

template<class T>
void variablesOnStack<T>::clear() {}

template<class T>
T variablesOnStack<T>::copy() {
    return value;
}

template<class T>
variablesOnStack<T>::variablesOnStack(T variable) {
    value = variable;
}

template<class T>
SetVariable<T>::SetVariable(std::map<std::string, Variable<T>> setVariable):
        setVariable(setVariable){
    for (auto variable: this->setVariable)
        nameVariables += variable->first;
}

template<class T>
bool SetVariable<T>::isEqualVariables(const std::string &functionVariables) {
    return functionVariables == this->nameVariables;
}

template<class T>
bool SetVariable<T>::isIn(const std::string symbol) {
    return setVariable.find(symbol) != setVariable.end();
}

template<class T>
Variable<T> SetVariable<T>::getVariable(std::string symbol) {
    if (!isIn(symbol))
        throw std::invalid_argument(symbol +  " not find");

    return setVariable[symbol].copy();
}
