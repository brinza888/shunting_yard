//
// Created by brinza on 19.05.22.
//

#ifndef SHUNTING_YARD_EVAL_H
#define SHUNTING_YARD_EVAL_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
#include <cmath>
#include <cctype>


class InvalidInput: public std::invalid_argument {
    using std::invalid_argument::invalid_argument;
};

class SyntaxError: public std::invalid_argument {
    using std::invalid_argument::invalid_argument;
};


struct Operator {
    enum class Associativity {
        Left = 0,
        Right = 1
    };

    const std::string name;
    unsigned int priority = 0;
    Associativity assoc = Associativity::Left;
    double(*func)(double, double);

    Operator(const std::string& name,
             unsigned int priority,
             double(*func)(double, double),
             Associativity assoc = Associativity::Left);
};

struct Function {
    const std::string name;
    double(*func)(double);

    Function(const std::string& name, double(*func)(double));
};

struct Token {
    enum class Type {
        Number = 0,
        Operator,
        Function,
        LeftBrace,
        RightBrace
    };

    union UValue {
        double number = 0;
        Operator* oper;
        Function* func;
    } value;

    Type type = Type::Number;

    Token() = default;
    Token(double number);
    Token(Operator* oper);
    Token(Function* func);
    Token(char brace);
};


const std::unordered_set<char> whitespaces ({
    ' ', '\n', '\t'
});

namespace {
    using OperatorMap = typename std::unordered_map<char, Operator*>;
    using FunctionMap = typename std::unordered_map<std::string, Function*>;

    const OperatorMap operators({
         {'+', new Operator("+", 100, [](double a, double b){ return a + b; })},
         {'-', new Operator("-", 100, [](double a, double b){ return a - b; })},
         {'*', new Operator("*", 200, [](double a, double b){ return a * b; })},
         {'/', new Operator("/", 200, [](double a, double b){ return a / b; })},
         {'^', new Operator("/", 300, [](double a, double b){ return std::pow(a, b); }, Operator::Associativity::Right)}
    });

    const FunctionMap functions({
            {"sin", new Function("sin", [](double x){ return std::sin(x); })},
            {"cos", new Function("cos", [](double x){ return std::cos(x); })},
            {"tan", new Function("tan", [](double x){ return std::tan(x); })},
            {"cot", new Function("cot", [](double x){ return (1 / std::tan(x)); })},
            {"abs", new Function("abs", [](double x){ return std::abs(x); })},
            {"-u", new Function("unary_minus", [](double x){ return -x; })},
            {"sgn", new Function("sgn", [](double x){
                if (x > 0) return 1.0;
                else if (x < 0) return -1.0;
                else return 0.0;
            })}
    });
}


std::vector<Token> tokenize(const std::string& expression);
std::vector<Token> shuntingYard(const std::vector<Token>& input);
double eval(const std::vector<Token>& rpnExpression);


#endif //SHUNTING_YARD_EVAL_H
