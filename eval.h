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
#include <cmath>
#include <cctype>


class InvalidInput: public std::invalid_argument {
    using std::invalid_argument::invalid_argument;
};


struct Token {
    enum class Type {
        Number = 0,
        Operator,
        Name,
        LeftBrace,
        RightBrace
    };

    Type type = Type::Number;
    std::string value = "0";

    Token() = default;
    Token(std::string value, Type type);
    Token(const char chr, Type type);
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


const std::unordered_set<char> whitespaces ({
    ' ', '\n', '\t'
});

namespace {
    std::unordered_map<char, Operator> operators({
         {'+', Operator(100, [](double a, double b){ return a + b; })},
         {'-', Operator(100, [](double a, double b){ return a - b; })},
         {'*', Operator(200, [](double a, double b){ return a * b; })},
         {'/', Operator(200, [](double a, double b){ return a / b; })}
    });

    std::unordered_map<std::string, double(*)(double)> functions({
            {"sin", [](double x){ return sin(x); }},
            {"cos", [](double x){ return cos(x); }},
            {"tan", [](double x){ return tan(x); }},
            {"cot", [](double x){ return (1 / tan(x)); }},
//            {"abs", [](double x){ return abs(x); }},
            {"-", [](double x){ return -x; }}
    });
}


std::queue<Token> tokenize(const std::string& expression);
std::queue<std::string> shuntingYard(std::queue<Token> tokens);
double eval(const std::vector<std::string>& rpnExpression);


#endif //SHUNTING_YARD_EVAL_H
