#include "GeneratorFormula.h"

size_t getRandom() {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    return gen();
}
