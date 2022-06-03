#include <iostream>
#include "eval.h"


int main() {

    auto tokens = tokenize("2 * sgn(12) * 2 + sin(3) * 0");
    auto rpn = shuntingYard(tokens);
    double result = eval(rpn);

    std::cout << result << std::endl;

    return 0;
}
