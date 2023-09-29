#ifndef REVERSEPOLISHNOTATION_FUNCTION_H
#define REVERSEPOLISHNOTATION_FUNCTION_H

#include <algorithm>
#include <iostream>

#include "../OperatorFunction/OperatorFunction.h"
#include "../Variable/Variable.h"

#define comma ,

#define FREE_PREFIX_BUFFER                            \
while (!buffer.empty() && buffer.end()[-1] != '(' &&  \
operators.getOperator(buffer.end()[-1]).isPrefix()) { \
result += buffer.end()[-1];                           \
buffer.pop_back();                                    \
}

#define FREE_BUFFER_BEFORE_BRACKET                          \
while (!buffer.empty() && buffer.end()[-1] != '(') {        \
    result += buffer.end()[-1];                             \
    buffer.pop_back();                                      \
}                                                           \
if (buffer.empty())                                         \
    throw std::invalid_argument("missing opening bracket");

#define ALGORITHM_OF_CALL(pushVariable, pushResult, T)             \
for (const auto &symbol: expression) {                             \
    if (operators.isIn(symbol)){                                   \
        auto &currentOperator =                                    \
                operators.getOperator(symbol);                     \
        size_t countOperands = currentOperator.getCountOperands(); \
                                                                   \
        if (countOperands > buffer.size())                         \
            throw std::invalid_argument("invalid expression");     \
                                                                   \
        std::vector<T> currentOperands;                            \
        for (size_t i = countOperands; i >= 1; --i) {              \
            auto current = buffer.end()[-i];                       \
            currentOperands.push_back(current);                    \
        }                                                          \
                                                                   \
        for (int i = 0; i < countOperands; ++i)                    \
            buffer.pop_back();                                     \
                                                                   \
        pushResult;                                                \
    } else                                                         \
        pushVariable;                                              \
}

/// функция которая переводит выражение
/// из инфиксной формы в обратную польскую нотацию
/// \tparam T класс  которым оперирует множество операторов
/// \param expression строка выражения в инфиксной форме
/// \param operators множество операторов
/// \return строка выражения в обратной Польской нотации
template<class T>
std::string getExpression(std::string &expression,
                          SetOperator<T> &operators) {
    std::string result, buffer;

    // итерируемся посимвольно
    for (auto symbol: expression) {
        // если это символ оператора
        if (operators.isIn(symbol)) {
            // получаем оператор
            auto &currentOperator =
                    operators.getOperator(symbol);

            // пока буфер не пуст и на вершине буфера не скобка и
            // приоритет оператора на вершине буфера больше или равен
            // приоритету текущего оператора
            if (!(currentOperator.isPostfix() || currentOperator.isPrefix()))
                while (!buffer.empty() && (buffer.end()[-1]) != '(' &&
                       operators.getOperator(buffer.end()[-1]).
                               getPriority() >=
                       currentOperator.getPriority()) {
                    // добавляем символ с вершины буфера к результату
                    // и удаляем его из буфера
                    result += buffer.end()[-1];
                    buffer.pop_back();
                }


            // добавляем текущий символ
            if (!currentOperator.isPostfix())
                buffer += symbol;
            else {
                result += symbol;
                if (currentOperator.isNolar())
                    FREE_PREFIX_BUFFER
            }
        } else
            switch (symbol) {
                case ',': // этот случай нужен для тернарных и более операторов
                    FREE_BUFFER_BEFORE_BRACKET
                    break;
                case '(': // добавляем открывающеюся скобку к буферу
                    buffer += symbol;
                    break;
                case ')': { // освобождаем буфер в результат до встречи
                          // открывающеюся скобки и удаляем её
                    FREE_BUFFER_BEFORE_BRACKET

                    buffer.pop_back();

                    FREE_PREFIX_BUFFER
                }
                    break;
                default: // это случай когда символ является символом переменной
                    result += symbol;
                    FREE_PREFIX_BUFFER
                    break;
            }
    }

    // пока буфер не пустой
    while (!buffer.empty()) {
        // получаем символ на вершине буфера и удаляем его из буфера
        auto symbol = buffer.end()[-1];
        buffer.pop_back();

        // это случится лиш в том случае когда
        // в ворожении пропущена закрывающая скобка
        if (symbol == '(')
            throw std::invalid_argument("missing closing bracket");

        // добавляем символ к результату
        result += symbol;
    }

    // возвращаем результат
    return result;
}

/// Функция возвращает отформатированную строку,
/// содержащую только уникальные символы из строки expression,
/// не являющиеся операторами.
/// \tparam T класс  которым оперирует множество операторов
/// \param expression строка выражения
/// \param operators множество операторов
/// \return строка с переменными выражения
template<class T>
std::string getName(std::string &expression,
                    SetOperator<T> &operators) {
    // получение символов не являющиеся символами операторов
    std::string symbolsNotOperators;
    for (auto &symbol: expression)
        if (!operators.isIn(symbol))
            symbolsNotOperators += symbol;

    // сортировка символов переменных
    std::sort(symbolsNotOperators.begin(),
              symbolsNotOperators.end());

    // Уникальные символы переменных добавляются в результирующую строку.
    std::string result;
    for (int i = 0; i < symbolsNotOperators.size(); ++i)
        if (symbolsNotOperators[i] !=
            symbolsNotOperators[i + 1])
            result += symbolsNotOperators[i];

    return result;
}

/// класс который служит для хранения выражения
/// и ссылку на множество операторов
/// и позволяет вычислить выражение с подстановкой переменных
/// \tparam T класс переменных в выражении
template<class T>
class Function {
private:
    std::string expression;

    SetOperator<T> &operators;

    std::string functionVariables;

public:
    /// инициализатор класса Function
    /// \param expression  строка выражения в обычной нотации
    /// \param operators множество операторов
    Function(std::string expression, SetOperator<T> &operators) :
            operators(operators) {
        // перевод выражение из инфиксной формы в обратную польскую нотацию
        this->expression = getExpression(expression, operators);
        // получение символов переменных
        this->functionVariables = getName(this->expression,
                                          operators);

        // проверка на корректность выражения
        int counter = 0;
        for (auto symbol: this->expression)
            if (operators.isIn(symbol))
                counter += operators.getOperator(symbol).getCountOperands() - 1;
            else
                counter -= 1;

        if (counter != -1)
            throw std::invalid_argument("invalid expression");
    }

    /// Описание: Вычисляет и возвращает результат выражения,
    /// состоящего из переменных и операторов.
    ///
    /// Требования к методу:
    /// метод должен проверить равенство
    /// набора переменных в аргументе функции
    /// и наборе переменных, определенных для данного экземпляра класса.
    /// Если переменные не равны, метод должен
    /// выбросить исключение std::invalid_argument.
    /// метод должен проверить размер вектора buffer
    /// после обработки всех элементов выражения.
    /// Если размер вектора не равен 1,
    /// метод должен выбросить исключение std::invalid_argument,
    /// указывающее на некорректное выражение.
    /// метод должен вернуть значение из первого
    /// и единственного элемента вектора buffer
    /// как результат вычисления выражения.
    /// \param variables объект типа SetVariable<T>,
    /// который содержит набор переменных для вычисления.
    /// \return  Результат вычисления выражения
    T call(SetVariable<T> variables) {
        if (!variables.isEqualVariables(functionVariables))
            throw std::invalid_argument(
                    "functionVariables != nameVariables");

        // вектор для вычисления результата
        std::vector<T> buffer;

        // алгоритм вычисления выражения в обратной Польской нотации
        ALGORITHM_OF_CALL(
                buffer.push_back(variables.getVariable(symbol)),
                buffer.push_back(currentOperator.
                        callOperand(currentOperands)),
                T)

        // проверка на корректность вычислений
        if (buffer.size() != 1)
            throw std::invalid_argument("invalid expression");

        return buffer[0];
    }

    /// Описание: Возвращает количество
    /// переменных, используемых функцией.
    /// \return Количество переменных,
    /// используемых функцией (целое число без знака).
    size_t getCountVariables() {
        return functionVariables.size();
    }

    /// Описание: Возвращает выражение функции
    /// \return  Выражение функции в виде строки
    /// в обратной Польской нотации
    /// (объект класса std::string)
    std::string getFunctionExpression() {
        return expression;
    }

    /// Описание: Возвращает переменные, используемые функцией.
    /// \return Список переменных, используемых функцией, в виде строки.
    std::string getFunctionVariables() {
        return functionVariables;
    }

    /// Метод класса Function преобразует собственное выражение,
    /// записанное в обратной Польской нотации в инфиксную форму.
    /// \return инфиксную форма выражения
    std::string getReversExpression() {
        // пустой буфер (вектор строк)
        std::vector<std::pair<std::string, std::string>> buffer;

        // алгоритм преобразования выражения,
        // записанного в обратной Польской нотации в инфиксную форму.
        ALGORITHM_OF_CALL(
                buffer.emplace_back(std::string{symbol}, std::string{symbol}),
                buffer.push_back(operators.getRevers(currentOperands,
                                                     symbol)),
                std::pair<std::string comma std::string>);

        // Если размер буфера не равен 1, это означает,
        // что выражение было некорректным
        if (buffer.size() != 1)
            throw std::invalid_argument("invalid expression");

        return buffer[0].first;
    }
};


#endif //REVERSEPOLISHNOTATION_FUNCTION_H
