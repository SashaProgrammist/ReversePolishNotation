#ifndef REVERSEPOLISHNOTATION_FUNCTION_H
#define REVERSEPOLISHNOTATION_FUNCTION_H

#include <algorithm>
#include <iostream>

#include "../OperatorFunction/OperatorFunction.h"
#include "../Variable/Variable.h"

// Макрос "comma" определяет
// запятую для удобства использования
// в макросах.
#define comma ,

// Макрос ADD_FROM_BUFFER_TO_RESULT(currentOperator)
// используется для добавления операторов
// из буфера в результат. Он выполняет следующие действия:
// 1. Проверяет, не пуст ли буфер
//    ternaryBuffer и соответствует ли текущий оператор
//    последнему символу в ternaryBuffer.
// 2. Если соответствие найдено и количество операндов
//    в ternaryBuffer не равно 1,
//    выбрасывается исключение std::invalid_argument
//    с сообщением "ternary dont hef operands".
// 3. Удаляет последний элемент из ternaryBuffer.
// 4. Добавляет последний элемент из buffer
//    в результат и удаляет его из buffer.
#define ADD_FROM_BUFFER_TO_RESULT        \
if (!ternaryBuffer.empty() &&            \
std::string{buffer.end()[-1]} ==         \
ternaryBuffer.end()[-1].first) {         \
if (ternaryBuffer.end()[-1].second != 1) \
throw std::invalid_argument(             \
"ternary dont hef operands");            \
ternaryBuffer.pop_back();                \
}                                        \
result += buffer.end()[-1];              \
buffer.pop_back();


// Освободить из буфера префиксные
// операторы до открывающей скобки.
#define FREE_FROM_BUFFER_PREFIX_OPERATORS             \
while (!buffer.empty() && buffer.end()[-1] != '(' &&  \
operators.getOperator(buffer.end()[-1]).isPrefix()) { \
ADD_FROM_BUFFER_TO_RESULT                             \
}

// Освободить буфер до открывающей скобки.
#define FREE_BUFFER_BEFORE_BRACKET                   \
while (!buffer.empty() && buffer.end()[-1] != '(') { \
    ADD_FROM_BUFFER_TO_RESULT                        \
}                                                    \
if (buffer.empty())                                  \
    throw std::invalid_argument("missing opening bracket");

// Алгоритм вызова операторов.
// pushVariable - действие при переменной,
// pushOperator - действие при операции,
// T - тип данных.
#define ALGORITHM_OF_CALL(pushVariable, pushOperator, T)             \
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
        pushOperator;                                                \
    } else                                                         \
        pushVariable;                                              \
}

/// функция, которая преобразует выражение,
/// содержащее операторы и операнды,
/// в постфиксную форму записи.
/// \tparam T класс  которым оперирует множество операторов
/// \param expression строка выражения в инфиксной форме
/// \param operators множество операторов
/// \return строка выражения в обратной Польской нотации
template<class T>
std::string getExpression(std::string &expression,
                          SetOperator<T> &operators) {
    // объявление переменных result и buffer,
    // которые будут использоваться
    // для хранения результата и
    // временного хранения символов.
    std::string result, buffer;
    std::vector<std::pair<std::string, size_t>> ternaryBuffer;

    // итерируемся по символам входного выражения.
    for (auto symbol: expression) {
        // проверяем, является ли символ оператором.
        if (operators.isIn(symbol)) {
            // получаем оператор, соответствующий текущему символу.
            auto &currentOperator =
                    operators.getOperator(symbol);

            // проверяем, является ли оператор инфиксным.
            if (!(currentOperator.isPostfix() ||
                  currentOperator.isPrefix()))
                // Выполняем цикл, пока буфер не пустой,
                // символ на вершине буфера не является открывающей скобкой
                // и приоритет оператора на вершине буфера
                // больше или равен приоритету текущего оператора.
                // В цикле мы добавляем символ с вершины буфера
                // к результату и удаляем его из буфера.
                while (!buffer.empty() && (buffer.end()[-1]) != '(' &&
                       operators.getOperator(buffer.end()[-1]).
                               getPriority() >=
                       currentOperator.getPriority()) {
                    ADD_FROM_BUFFER_TO_RESULT
                }


            // проверяем, не является ли оператор постфиксным.
            if (!currentOperator.isPostfix()) {
                if (currentOperator.isInfix() &&
                    currentOperator.getCountOperands() >= 3)
                    ternaryBuffer.push_back(
                            {currentOperator.getSymbol(),
                             currentOperator.getCountOperands() - 1});
                buffer += symbol; // добавляем текущий символ к буферу.
            } else { //  ветка, которая выполняется,
                //       если оператор является постфиксным.
                result += symbol; // Добавляем текущий символ к результату.
                // Если это нулевой арности оператор,
                if (currentOperator.isNolar())
                    // освобождаем из буфера все префиксные операторы.
                    FREE_FROM_BUFFER_PREFIX_OPERATORS
            }
        } else
            // проверяем, какой символ встретился.
            switch (symbol) {
                case ',':
                    // Ветка, которая выполняется, если символ - запятая.
                    // Используется для разделения операндов в тернарных
                    // и других многозначных операторах.
                FREE_BUFFER_BEFORE_BRACKET
                    break;
                case ':': {
                    // Ветка, которая выполняется, если символ - двоеточие.
                    // Используется для разделения операндов в инфиксных тернарных
                    // и других многозначных операторах.

                    // Инициализация переменных
                    // Счетчик операторов, которые нужно добавить к результату.
                    long long index = 0;
                    // Флаг для продолжения обработки
                    bool isContinue = true;

                    // Проверяем операторы в буфере,
                    // начиная с последнего добавленного.
                    while (isContinue) {
                        // Проверяем, находится ли индекс
                        // в пределах размера буфера,
                        // и текущий символ в буфере
                        // не является открывающей скобкой '('.
                        if (index < buffer.size() &&
                            buffer.end()[-index - 1] != '(') {
                            // Получаем текущий оператор из буфера.
                            auto current =
                                    operators.getOperator(buffer.end()
                                                          [-index - 1]);

                            // Проверка, является ли текущий
                            // оператор текущим тернарным оператором
                            if (current.getSymbol() ==
                                ternaryBuffer.end()[-1].first) {
                                if (ternaryBuffer.
                                        end()[-1].second > 1) {
                                    // Если у тернарного оператора
                                    // ещё есть ожидаемые операнды,
                                    // уменьшаем счётчик ожидаемых операндов.
                                    isContinue = false;
                                    ternaryBuffer.end()[-1].second--;
                                } else {
                                    // Если все операнды для данного
                                    // тернарного оператора обработаны,
                                    // убираем его из буфера тернарных
                                    // операторов и инкрементируем индекс.
                                    ternaryBuffer.pop_back();
                                    index++;
                                }
                            } else
                                index++;
                        } else
                            // Генерируем исключение
                            // с сообщением "missing ternary operator",
                            // если не удается найти
                            // соответствующий тернарный
                            // оператор в буфере операторов.
                            // Это обработка случая,
                            // когда тернарный оператор
                            // не был правильно представлен в выражении.l
                            throw std::invalid_argument(
                                    "missing ternary operator");
                    }

                    // Добавляем операторы в выходную строку.
                    for (int i = 0; i < index; ++i) {
                        result += buffer.end()[-1];
                        buffer.pop_back();
                    }
                }
                    break;
                case '(':
                    // Ветка, которая выполняется,
                    // если символ - открывающая скобка.
                    // Добавляем открывающую скобку к буферу.
                    buffer += symbol;
                    break;
                case ')': {
                    // Ветка, которая выполняется,
                    // если символ - закрывающая скобка.
                    // Освобождаем буфер в результат
                    // до встречи открывающей скобки и удаляем ее.
                    FREE_BUFFER_BEFORE_BRACKET

                    buffer.pop_back();

                    // Освобождаем из буфера все префиксные операторы.
                    FREE_FROM_BUFFER_PREFIX_OPERATORS
                }
                    break;
                default:
                    // Ветка, которая выполняется,
                    // если символ является символом переменной.
                    // Добавляем символ к результату и
                    // освобождаем из буфера все префиксные операторы.
                    result += symbol;
                    FREE_FROM_BUFFER_PREFIX_OPERATORS
                    break;
            }
    }

    // Выполняем цикл, пока буфер не пустой.
    // Получаем символ на вершине буфера и добавляем его к результату.
    while (!buffer.empty()) {
        auto symbol = buffer.end()[-1];
        buffer.pop_back();

        //  выбрасываем исключение,
        //  если на вершине буфера находится открывающая скобка.
        if (symbol == '(')
            throw std::invalid_argument("missing closing bracket");

        // добавляем символ к результату
        result += symbol;
    }

    // Возвращаем результат преобразования выражения в постфиксную форму.
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
        this->expression = getExpression<T>(expression, operators);
        // Получение имени переменных из выражения
        this->functionVariables = getName(this->expression,
                                          operators);

        // проверка на корректность выражения
        int counter = 0;
        // Перебор символов в выражении
        for (auto symbol: this->expression)
            // Если символ является оператором
            if (operators.isIn(symbol))
                // Увеличиваем счетчик на количество операндов
                // (так как из стека будут убраны столько операндов),
                // обрабатываемых оператором минус один.
                // (так как после выполнения операции её результат попадёт в стек)
                counter += operators.
                        getOperator(symbol).getCountOperands() - 1;
            else
                // Если символ не является оператором,
                // уменьшаем счетчик на один
                // так как он добавится в стек
                counter -= 1;

        // Если счетчик не равен -1, выбрасываем исключение с сообщением об ошибке
        // так как Если счетчик равен -1, то в стеке остался один операнд иначе
        // либо возникнет ситуация, в которой для операции не будет хватать
        // операндов, либо в результате в стеке будет больше одного операнда.
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
        // Проверка, что переданные переменные равны переменным функции
        if (!variables.isEqualVariables(functionVariables))
            throw std::invalid_argument(
                    "functionVariables != nameVariables");

        // Создание вектора для вычисления результата
        std::vector<T> buffer;

        // Алгоритм вычисления выражения в обратной польской нотации
        for (const auto &symbol: expression) {
            // Если символ является оператором
            if (operators.isIn(symbol)) {
                // Получение ссылки на текущий оператор
                auto &currentOperator = operators.getOperator(symbol);

                // Получение количества операндов у текущего оператора
                size_t countOperands =
                        currentOperator.getCountOperands();

                // Проверка, что в буфере достаточно
                // операндов для выполнения операции
                if (countOperands > buffer.size())
                    throw std::invalid_argument("invalid expression");

                // Вектор для хранения текущих операндов
                std::vector<T> currentOperands;

                // Извлечение операндов из буфера (с конца)
                for (size_t i = countOperands; i >= 1; --i) {
                    auto current = buffer.end()[-i];
                    currentOperands.push_back(current);
                }

                // Удаление из буфера извлеченных операндов
                for (int i = 0; i < countOperands; ++i)
                    buffer.pop_back();

                // Выполнение операции с текущими
                // операндами и добавление результата в буфер
                buffer.push_back(currentOperator.
                        callOperand(currentOperands));
            }
                // Если символ является переменной
            else {
                // Получение значения переменной из
                // переданных переменных и добавление в буфер
                buffer.push_back(variables.getVariable(symbol));
            }
        }

        // Проверка на корректность вычислений
        // должен остаться только один элемент в буфере
        if (buffer.size() != 1)
            throw std::invalid_argument("invalid expression");

        // Возвращение результата вычислений
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
    /// записанное в обратной Польской нотации
    /// в нотацию соответствующую operators.
    /// \return инфиксную форма выражения
    std::string getReversExpression() {
        // пустой буфер (вектор строк)
        std::vector<std::pair<std::string, std::string>> buffer;

        // алгоритм преобразования выражения,
        // записанного в обратной Польской нотации в инфиксную форму.
        ALGORITHM_OF_CALL(
                buffer.emplace_back(std::string{symbol},
                                    std::string{symbol}),
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
