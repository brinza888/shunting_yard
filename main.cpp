#include <iostream>
#include "eval.h"


std::ostream& operator<<(std::ostream& stream, Token token) {
    if (token.type == Token::Type::Number) {
        stream << token.value.number;
    }
    else if (token.type == Token::Type::LeftBrace) {
        stream << "(";
    }
    else if (token.type == Token::Type::RightBrace) {
        stream << ")";
    }
    else if (token.type == Token::Type::Operator) {
        stream << token.value.oper->name;
    }
    else if (token.type == Token::Type::Function) {
        stream << token.value.func->name;
    }
    else if (token.type == Token::Type::ArgsSep) {
        stream << ',';
    }
    return stream;
}


int main() {

    std::string line;
    std::getline(std::cin, line);
    auto tokens = tokenize(line);

    std::cout << "Tokenization:" << std::endl;
    for (auto token: tokens) {
        std::cout << token << " ";
    }
    std::cout << std::endl;

    auto rpn = shuntingYard(tokens);

    std::cout << "RPN:" << std::endl;
    for (auto token: rpn) {
        std::cout << token << " ";
    }
    std::cout << std::endl;

    double result = eval(rpn);

    std::cout << "Result: " << result << std::endl;

    return 0;
}
