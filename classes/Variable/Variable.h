#ifndef REVERSEPOLISHNOTATION_VARIABLE_H
#define REVERSEPOLISHNOTATION_VARIABLE_H

#include <map>
#include <string>
#include <stdexcept>

template<class T>
class Variable {
protected:
    T value;
public:
    explicit Variable(T variable);

    T &getValue();

    virtual void clear();

    virtual T copy();
};

template<class T>
class variablesOnStack : public Variable<T> {
protected:
    T value;
public:
    explicit variablesOnStack(T variable);

    void clear();

    T copy();
};

template<class T>
class SetVariable{
private:
    std::map<std::string, Variable<T>> setVariable;

    std::string nameVariables;

public:
    explicit SetVariable(std::map<std::string, Variable<T>> setVariable);

    bool isEqualVariables(const std::string &functionVariables);

    bool isIn(std::string symbol);

    Variable<T> getVariable(std::string symbol);
};

#endif //REVERSEPOLISHNOTATION_VARIABLE_H
