//
// Created by brinza on 19.05.22.
//

#include "eval.h"
#include <cctype>

using namespace std;


Operator::Operator(unsigned int priority, double(*func)(double, double), Associativity assoc) {
    this->priority = priority;
    this->func = func;
    this->assoc = assoc;
}

Token::Token(const std::string& value, TokenType type) {
    this->value = value;
    this->type = type;
}

queue<Token> tokenize(const string& expression) {
    queue<Token> tokens;
    string value;
    TokenType state = TokenType::None, prevState = TokenType::None;
    for (auto&& chr: expression) {
        if (chr == ' ') {  // skip whitespaces
            state = TokenType::None;
        }
        else if (isdigit(chr)) {
            state = TokenType::Number;
            value += chr;
        }
        else if (isalpha(chr)) {
            state = TokenType::None;
            value += chr;
        }
        if (prevState != state && !value.empty()) {
            tokens.push(Token(value, prevState));
            value.clear();
        }
    }

    return tokens;
}

queue<string> shuntingYard(const vector<string>& tokens) {
    return queue<string>();
}

double eval(const vector<string>& rpmExpression) {
    return 0.0;
}
