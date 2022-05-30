//
// Created by brinza on 19.05.22.
//

#ifndef SHUNTING_YARD_EVAL_H
#define SHUNTING_YARD_EVAL_H

#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>


class SyntaxError: public std::invalid_argument {
    using std::invalid_argument::invalid_argument;
};

class InputError: public std::invalid_argument {
    using std::invalid_argument::invalid_argument;
};

class InvalidSymbol: public std::invalid_argument {
    using std::invalid_argument::invalid_argument;
};


const std::unordered_set<char> whitespaces ({
    ' ', '\n', '\t'
});

struct Token {
    enum class Type {
        Number,
        Operator,
        Symbol,
        LeftBrace,
        RightBrace
    };

    Type type = Type::Number;
    std::string value = "0";

    Token(const std::string& value, Type type);
};

struct Operator {
    enum class Associativity {
        Left = 0,
        Right = 1
    };

    unsigned int priority = 0;
    Associativity assoc = Associativity::Left;
    double(*func)(double, double);

    Operator(unsigned int priority, double(*func)(double, double), Associativity assoc = Associativity::Left);
};


std::queue<Token> tokenize(const std::string& expression);
std::queue<std::string> shuntingYard(const std::vector<std::string>& tokens);
double eval(const std::vector<std::string>& rpnExpression);


#endif //SHUNTING_YARD_EVAL_H
