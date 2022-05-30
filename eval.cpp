//
// Created by brinza on 19.05.22.
//

#include "eval.h"
#include <cctype>
#include <cmath>

using namespace std;


namespace {
    std::unordered_map<char, Operator> operators({
       {'+', Operator(100, [](double a, double b){ return a + b; })},
       {'-', Operator(100, [](double a, double b){ return a - b; })},
       {'*', Operator(200, [](double a, double b){ return a * b; })},
       {'/', Operator(200, [](double a, double b){ return a / b; })}
    });

    std::unordered_map<string, double(*)(double)> functions({
        {"sin", [](double x){ return sin(x); }},
        {"cos", [](double x){ return cos(x); }},
        {"tan", [](double x){ return tan(x); }},
        {"cot", [](double x){ return (1 / tan(x)); }},
        {"abs", [](double x){ return abs(x); }},
        {"-", [](double x){ return -x; }}
    });
}


Operator::Operator(unsigned int priority, double(*func)(double, double), Associativity assoc) {
    this->priority = priority;
    this->func = func;
    this->assoc = assoc;
}

Token::Token(const std::string& value, Type type) {
    this->value = value;
    this->type = type;
}

bool isOperator(char value) {
    return (operators.find(value) != operators.end());
}

bool isFunction(const string& symbol) {
    return (functions.find(symbol) != functions.end());
}

string constructNumber(string::const_iterator& it) {
    string value;
    bool dotExists = false;
    while (isdigit(*it) || *it == '.') {
        value += *it;
        if (*it == '.') {
            if (dotExists) {
                throw SyntaxError("Invalid value in input");
            }
            else {
                dotExists = true;
            }
        }
        it++;
    }
    if (value.empty()) {
        throw InputError("Excepted a number, but nothing found");
    }
    return value;
}

string constructSymbol(string::const_iterator& it) {
    string value;
    while (isalpha(*it)) {
        value += *it;
        it++;
    }
    if (value.empty()) {
        throw InputError("Excepted a symbol, nothing found");
    }
    return value;
}

queue<Token> tokenize(const string& expression) {
    queue<Token> tokens;
    string::const_iterator it = expression.cbegin();
    while (it != expression.cend()) {
        if (whitespaces.find(*it) != whitespaces.cend()) {

        }
        else if (isdigit(*it) || *it == '.') {  // construct number
            string value = constructNumber(it);
            tokens.emplace(value, Token::Type::Number);
        }
        else if (isOperator(*it)) {
            if (tokens.front().type != Token::Type::Number) {
                tokens.
            }
            tokens.emplace(*it, Token::Type::Operator);
        }
        else if (*it == '(') {
            tokens.emplace(*it, Token::Type::LeftBrace);
        }
        else if (*it == ')') {
            tokens.emplace(*it, Token::Type::RightBrace);
        }
        else if (isalpha(*it)) {  // construct symbol
            string symbol = constructSymbol(it);
            if (isFunction(symbol)) {
                tokens.emplace(symbol, Token::Type::Symbol);
            }
            else {
                throw InvalidSymbol("Symbol not exists (not a function)");
            }
        }
        it++;
    }

    return tokens;
}

queue<string> shuntingYard(const vector<string>& tokens) {
    return queue<string>();
}

double eval(const vector<string>& rpmExpression) {
    return 0.0;
}
