#ifndef REVERSEPOLISHNOTATION_VARIABLE_H
#define REVERSEPOLISHNOTATION_VARIABLE_H

#include <map>
#include <string>
#include <stdexcept>


template<class T>
class SetVariable {
private:
    // Ассоциативный контейнер для
    // хранения переменных и их значений.
    std::map<std::string, T> setVariable;

    // Строка для хранения имен всех переменных.
    std::string nameVariables;

public:
    /// Инициализация объекта SetVariable
    /// с заданным набором переменных и их значений.
    explicit SetVariable(std::map<std::string, T> setVariable) :
            setVariable(setVariable) {
        for (auto variable: this->setVariable)
            nameVariables += variable.first;
    }

    /// Возвращает строку,
    /// содержащую имена всех переменных.
    [[nodiscard]] std::string getNameVariables() const {
        return nameVariables;
    }

    /// Проверяет, равны ли переданные
    /// имена переменных и имена переменных в объекте.
    bool isEqualVariables(const std::string &functionVariables) {
        if (functionVariables == nameVariables)
            return true;

        // Проверяет, есть ли хотя бы одно общее
        // имя переменной между переданными именами
        // и именами переменных в объекте.
        return any_of(
                functionVariables.begin(),
                functionVariables.end(),
                [this](auto name) {
                    return nameVariables.find(name) !=
                    std::string::npos;
                });
    }

    // Проверяет, существует ли переменная с заданным именем.
    bool isIn(const std::string symbol) {
        return setVariable.find(symbol) != setVariable.end();
    }

    // Проверяет существование переменной
    // с заданным символьным именем.
    bool isIn(const char symbol) {
        return isIn(std::string{symbol});
    }

    // Возвращает ссылку на переменную с заданным именем,
    // если она существует. В противном случае генерирует исключение.
    T &getVariable(std::string const symbol) {
        if (!isIn(symbol))
            throw std::invalid_argument(symbol + " not find");

        return setVariable[symbol];
    }

    // Возвращает ссылку на переменную с заданным символьным именем,
    // если она существует. В противном случае генерирует исключение.
    T &getVariable(const char symbol) {
        return getVariable(std::string{symbol});
    }
};

#endif //REVERSEPOLISHNOTATION_VARIABLE_H
