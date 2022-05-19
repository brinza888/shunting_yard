//
// Created by brinza on 19.05.22.
//

#ifndef SHUNTING_YARD_EVAL_H
#define SHUNTING_YARD_EVAL_H

#include <vector>
#include <string>

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
};


std::vector<Token> tokenize(const std::string& expression);
std::vector<std::string> shuntingYard(const std::vector<std::string>& tokens);
double eval(const std::vector<std::string>& rpnExpression);


#endif //SHUNTING_YARD_EVAL_H
