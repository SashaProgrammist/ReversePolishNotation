#ifndef REVERSEPOLISHNOTATION_VARIABLE_H
#define REVERSEPOLISHNOTATION_VARIABLE_H

#include <map>
#include <string>
#include <stdexcept>


template<class T>
class SetVariable {
private:
    std::map<std::string, T> setVariable;

    std::string nameVariables;

public:
    explicit SetVariable(std::map<std::string, T> setVariable) :
            setVariable(setVariable) {
        for (auto variable: this->setVariable)
            nameVariables += variable.first;
    }

    [[nodiscard]] std::string getNameVariables() const {
        return nameVariables;
    }

    bool isEqualVariables(const std::string &functionVariables) {
        return any_of(
                functionVariables.begin(),
                functionVariables.end(),
                [this](auto name) {
                    return nameVariables.find(name) != std::string::npos;
                });
    }

    bool isIn(const std::string symbol) {
        return setVariable.find(symbol) != setVariable.end();
    }

    bool isIn(const char symbol) {
        return isIn(std::string{symbol});
    }

    T &getVariable(std::string const symbol) {
        if (!isIn(symbol))
            throw std::invalid_argument(symbol + " not find");

        return setVariable[symbol];
    }

    T &getVariable(const char symbol) {
        return getVariable(std::string{symbol});
    }
};

#endif //REVERSEPOLISHNOTATION_VARIABLE_H
