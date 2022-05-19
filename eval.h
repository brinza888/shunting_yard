//
// Created by brinza on 19.05.22.
//

#ifndef SHUNTING_YARD_EVAL_H
#define SHUNTING_YARD_EVAL_H

#include <vector>
#include <string>
#include <queue>
#include <unordered_map>


enum class TokenType {
    None,
    Number,
    Operator,
    Function,
    Brace
};

struct Token {
    TokenType type = TokenType::None;
    std::string value;

    Token(const std::string& value, TokenType type);
};

enum class Associativity {
    Left = 0,
    Right = 1
};

struct Operator {
    unsigned int priority = 0;
    Associativity assoc = Associativity::Left;
    double(*func)(double, double);

    Operator(unsigned int priority, double(*func)(double, double), Associativity assoc = Associativity::Left);
};


std::queue<Token> tokenize(const std::string& expression);
std::queue<std::string> shuntingYard(const std::vector<std::string>& tokens);
double eval(const std::vector<std::string>& rpnExpression);


#endif //SHUNTING_YARD_EVAL_H
