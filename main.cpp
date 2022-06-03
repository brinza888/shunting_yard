#include <iostream>
#include "eval.h"


int main() {

    std::string line;
    std::getline(std::cin, line);
    auto tokens = Eval::tokenize(line);

    std::cout << "Tokenization:" << std::endl;
    for (auto token: tokens) {
        std::cout << token << " ";
    }
    std::cout << std::endl;

    auto rpn = Eval::shuntingYard(tokens);

    std::cout << "RPN:" << std::endl;
    for (auto token: rpn) {
        std::cout << token << " ";
    }
    std::cout << std::endl;

    double result = Eval::eval(rpn);

    std::cout << "Result: " << result << std::endl;

    return 0;
}
