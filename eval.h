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
    size_t argc = 1;
    double(*func)(std::vector<double>&);

    Function(const std::string& name, size_t argc, double(*func)(std::vector<double>&));
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
         {'^', new Operator("^", 300, [](double a, double b){ return std::pow(a, b); }, Operator::Associativity::Right)},
         {'u', new Operator("u", 400, [](double a, double b){ return -a; })}
    });

    const FunctionMap functions({
            {"sin", new Function("sin", 1, [](std::vector<double>& args){ return std::sin(args[0]); })},
            {"cos", new Function("cos", 1, [](std::vector<double>& args){ return std::cos(args[0]); })},
            {"tan", new Function("tan", 1, [](std::vector<double>& args){ return std::tan(args[0]); })},
            {"cot", new Function("cot", 1, [](std::vector<double>& args){ return (1 / std::tan(args[0])); })},
            {"abs", new Function("abs", 1, [](std::vector<double>& args){ return std::abs(args[0]); })},
            {"sgn", new Function("sgn", 1, [](std::vector<double>& args){
                double x = args[0];
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
