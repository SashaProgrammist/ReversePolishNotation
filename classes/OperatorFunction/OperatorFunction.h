#ifndef REVERSEPOLISHNOTATION_OPERATORFUNCTION_H
#define REVERSEPOLISHNOTATION_OPERATORFUNCTION_H

#include <vector>
#include <stdexcept>
#include <map>
#include <algorithm>

enum NotationType {
    uncertain,   // Неопределённый тип нотации оператора
    prefix,      // Префиксная нотация (например, -a)
    infix,       // Инфиксная нотация (например, a + b)
    postfix      // Постфиксная нотация (например, a b +)
};

/// Этот класс представляет операторную функцию
/// в математическом выражении. Он определяет
/// свойства и поведение операторов, включая их
/// арность, приоритет, символ и функцию вычисления.
/// \tparam T
template<class T>
class OperatorFunction {
private:
    size_t countOperands{};   // Количество операндов для этого оператора
    size_t priority{};        // Приоритет оператора
    std::string symbol;       // Символ оператора

    T (*init)(std::vector<T>); // Функция оператора

    NotationType notationType = uncertain; // Тип нотации оператора

public:
    OperatorFunction() = default;

    // Конструктор класса OperatorFunction
    OperatorFunction(size_t countOperands,
                     size_t priority,
                     std::string symbol,
                     T (*init)(std::vector<T>), NotationType type = uncertain) :
            countOperands(countOperands),
            priority(priority),
            symbol(std::move(symbol)),
            init(init), notationType(type) {
        // Если тип нотации не задан, определить его на основе количества операндов
        if (notationType == uncertain)
            switch (countOperands) {
                case 0:
                    notationType = postfix;
                    break;
                case 1:
                    notationType = prefix;
                    break;
                case 2:
                    notationType = infix;
                    break;
                default:
                    notationType = prefix;
                    break;
            }
    }

    // Вызов оператора с заданными операндами и возврат результата
    [[nodiscard]] T callOperand(std::vector<T> operands) const {
        if (countOperands != operands.size())
            throw std::invalid_argument(
                    "number of operands does not match");

        T result = init(operands);

        return result;
    }


    // Геттер для количества операндов
    [[nodiscard]] size_t getCountOperands() const {
        return countOperands;
    }

    // Геттер для приоритета оператора
    [[nodiscard]] size_t getPriority() const {
        return priority;
    }

    // Геттер для символа оператора
    [[nodiscard]] std::string getSymbol() const {
        return symbol;
    }

    // Проверка, является ли оператор постфиксным
    [[nodiscard]] bool isPostfix() const {
        return notationType == postfix;
    }

    // Проверка, является ли оператор инфиксным
    [[nodiscard]] bool isInfix() const {
        return notationType == infix;
    }

    // Проверка, является ли оператор префиксным
    [[nodiscard]] bool isPrefix() const {
        return notationType == prefix;
    }

    // Проверка, является ли оператор нульарным (без операндов)
    [[nodiscard]] bool isNolar() const {
        return countOperands == 0;
    }

    // Геттер для типа нотации оператора
    [[nodiscard]] NotationType getNotationType() const {
        return notationType;
    }
};

template<class T>
class SetOperator;

template<class T>
std::string getExpression(std::string &expression,
                          SetOperator<T> &operators);

template<class T>
class SetOperator {
private:
    // Карта,
    // содержащая символы операторов в качестве ключей
    // и соответствующие им операторы.
    std::map<std::string, OperatorFunction<T>> set;

    // Строка, содержащая символы всех операторов,
    // присутствующих в контексте.
    std::string symbolsOperators{};

    // Максимальный приоритет оператора в контексте.
    size_t maxPriority{};

    //  Количество ноларных операторов в контексте.
    size_t countNolars{};

    // Общее количество операторов
    // в контексте (ноларных и неноларных).
    size_t countOperators{};

public:
    /// Конструктор класса SetOperator
    /// \param operators принимает вектор операторов
    explicit SetOperator(std::vector<OperatorFunction<T>> operators) :
            countOperators(operators.size()) {
        // Перебираем операторы в переданном векторе.
        for (auto _operator: operators) {
            // Добавляем оператор в множество set,
            // используя символ оператора в качестве ключа.
            set[_operator.getSymbol()] = _operator;

            // Добавляем символ оператора в строку symbolsOperators.
            symbolsOperators += _operator.getSymbol();

            // Если приоритет текущего оператора больше
            // максимального приоритета, то обновляем его.
            if (maxPriority < _operator.getPriority())
                maxPriority = _operator.getPriority();
            // Если текущий оператор является ноларным,
            // то увеличиваем счетчик ноларных операторов
            if (_operator.isNolar())
                countNolars++;
        }
    }

    /// Возвращает строку с символами операторов
    [[nodiscard]] std::string getSymbolsOperators() const {
        return symbolsOperators;
    }

    /// Проверяет, содержится ли символ оператора в множестве операторов
    bool isIn(std::string symbolOperator) {
        return set.find(symbolOperator) != set.end();
    }

    /// Проверяет, содержится ли символ оператора в множестве
    /// (перегрузка для работы с символами)
    bool isIn(char symbolOperator) {
        return isIn(std::string{symbolOperator});
    }

    /// Возвращает ссылку на объект оператора,
    /// соответствующего переданному символу оператора
    OperatorFunction<T> &getOperator(
            std::string symbolOperator) {
        if (!isIn(symbolOperator))
            throw std::invalid_argument(symbolOperator +
                                        " not find");

        return set[symbolOperator];
    }

    /// Возвращает ссылку на объект оператора,
    /// соответствующего переданному символу оператора
    /// (перегрузка для работы с символами)
    OperatorFunction<T> &getOperator(char symbolOperator) {
        return getOperator(std::string{symbolOperator});
    }

    /// Метод получает обратное выражение для заданного оператора.
    /// \param expressions Вектор пар, представляющих выражения.
    /// Каждая пара состоит из двух строк:
    /// первая строка - значение операнда,
    /// вторая строка - символ оператора.
    /// \param symbolOperator Символ оператора,
    /// для которого требуется получить обратное выражение.
    /// \return Метод возвращает пару строк.
    /// Первая строка представляет собой
    /// обратное выражение, а вторая строка
    /// - символ оператора. Эта пара представляет
    /// собой результат преобразования выражения
    /// для заданного оператора.
    std::pair<std::string, std::string>
    getRevers(std::vector<std::pair<std::string, std::string>>
              expressions,
              std::string symbolOperator) {
        // Получаем оператор для символа.
        OperatorFunction<T> _operator = getOperator(symbolOperator);

        // Проверяем, что количество операндов
        // в выражении соответствует ожидаемому.
        if (expressions.size() != _operator.getCountOperands())
            throw std::invalid_argument(
                    "expressions.size() != "
                    "_operator.getCountOperands()");

        if (_operator.getCountOperands()) {
            // Если оператор является префиксным и имеет один операнд,
            if (_operator.isPrefix()) {
                if (_operator.getCountOperands() == 1) {
                    // Проверяем, находится ли операнд
                    // в множестве операторов и получаем его оператор.
                    bool isIn_ = isIn(expressions[0].second);
                    OperatorFunction<T> operator_;
                    if (isIn_)
                        operator_ = getOperator(expressions[0].second);

                    // Если операнд не находится в множестве операторов
                    // или является постфиксным и нолярным,
                    // возвращаем символ оператора.
                    if (!isIn_ ||
                        operator_.isPostfix() &&
                        operator_.isNolar())
                        return {symbolOperator +
                                expressions[0].first,
                                symbolOperator};
                }

                // Строим префиксное выражение.
                std::string resultFirst = symbolOperator + "(";
                for (const auto &expression: expressions)
                    resultFirst += expression.first + ",";
                resultFirst.pop_back();
                resultFirst += ")";

                return {resultFirst, symbolOperator};
            }

            // Если оператор является постфиксным,
            if (_operator.isPostfix()) {
                // Проверяем, что все операнды постфиксные операторы
                // или все операнды не являются операторами.
                if (std::all_of(
                        expressions.begin(),
                        expressions.end(),
                        [this](const std::pair<std::string,
                                std::string> &expression) {
                            return isIn(expression.second) &&
                                   getOperator(
                                           expression.second
                                   ).isPostfix();
                        }) ||
                    std::all_of(
                            expressions.begin(),
                            expressions.end(),
                            [this](const std::pair<std::string,
                                    std::string> &expression) {
                                return !isIn(expression.second);
                            })) {
                    // Строим постфиксное выражение.
                    std::string result;
                    for (const auto &expression: expressions)
                        result += expression.first;
                    result += symbolOperator;
                    return {result, symbolOperator};
                }

                // Строим постфиксное выражение с обрамлением скобками.
                std::string result = "(";
                for (const auto &expression: expressions)
                    result += expression.first + ",";
                result.pop_back();
                result += ")" + symbolOperator;
                return {result, symbolOperator};
            }

            // Если оператор инфиксный,
            std::string result;

            // Проверяем, находится ли первый операнд
            // в множестве операторов и получаем
            // его оператор и приоритет.
            bool isIn_ = isIn(expressions[0].second);
            OperatorFunction<T> operator_;
            size_t priorityOperator;
            if (isIn_) {
                operator_ = getOperator(expressions[0].second);
                priorityOperator = operator_.getPriority();
            }

            // Если первый операнд не находится в
            // множестве операторов
            // или не является инфиксным или
            // его приоритет выше или равен приоритету текущего оператора,
            // добавляем символ текущего
            // оператора к первому операнду.
            if (!isIn_ ||
                !operator_.isInfix() ||
                priorityOperator >=
                _operator.getPriority())
                result += expressions[0].first +
                          symbolOperator;
            else
                // иначе обрамляем скобками
                result += "(" + expressions[0].first +
                          ")" + symbolOperator;

            // Обработка остальных операндов.
            for (auto expression = ++expressions.begin();
                 expression != expressions.end();
                 ++expression) {
                // Проверяем, находится ли операнд
                // в множестве операторов и
                // получаем его оператор и приоритет.
                isIn_ = isIn((*expression).second);
                if (isIn_) {
                    operator_ = getOperator((*expression).second);
                    priorityOperator = operator_.getPriority();
                }

                // Если операнд не находится
                // в множестве операторов
                // или не является инфиксным или
                // его приоритет выше приоритета текущего оператора,
                // добавляем символы текущего выражения к результату.
                if (!isIn_ ||
                    !operator_.isInfix() ||
                    priorityOperator >
                    _operator.getPriority())
                    result += (*expression).first + ":";
                else
                    // иначе обрамляем скобками
                    result += "(" + (*expression).first + "):";
            }

            result.pop_back();

            return {result, symbolOperator};
        } else
            return {symbolOperator, symbolOperator};
    }

    /// Преобразует символ оператора в строку и вызывает
    /// основной метод getRevers для обработки выражения.
    /// \param expressions Вектор пар
    /// (выражение, последний оператор) для выражения.
    /// \param symbolOperator Символ оператора.
    /// \return Пара строк,
    /// первая строка - результирующее выражение,
    /// вторая строка - символ оператора.
    std::pair<std::string, std::string>
    getRevers(std::vector<std::pair<std::string, std::string>> expressions,
              char symbolOperator) {
        return getRevers(expressions, std::string{symbolOperator});
    }

    /// Возвращает количество ноларных операторов.
    /// \return Количество ноларных
    /// операторов в данном контексте.
    size_t getCountNolares() {
        return countNolars;
    }

    /// Возвращает количество неноларных операторов.
    /// \return Количество неноларных
    /// операторов в данном контексте.
    size_t getCountNonNolares() {
        return countOperators - countNolars;
    }
};

#endif //REVERSEPOLISHNOTATION_OPERATORFUNCTION_H
