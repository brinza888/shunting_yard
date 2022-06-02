#include <iostream>
#include "eval.h"

std::ostream& operator<<(std::ostream& stream, Token::Type type) {
    switch (type) {
        case Token::Type::Number: {
            stream << "Number";
            break;
        }
        case Token::Type::Operator: {
            stream << "Operator";
            break;
        }
        case Token::Type::Name: {
            stream << "Name";
            break;
        }
        case Token::Type::LeftBrace: {
            stream << "LeftBrace";
            break;
        }
        case Token::Type::RightBrace: {
            stream << "RightBrace";
            break;
        }
    }
    return stream;
}

std::ostream& operator<<(std::ostream& stream, Token& token) {
    stream << token.type << ":" << token.value;
    return stream;
}

int main() {

    auto tokens = tokenize("(5 + 5) * 2");
    while (!tokens.empty()) {
        std::cout << tokens.front() << ", ";
        tokens.pop();
    }
    std::cout << std::endl;

    return 0;
}
