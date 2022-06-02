//
// Created by brinza on 19.05.22.
//

#include "eval.h"
#include <cctype>
#include <cmath>
#include <stack>

using namespace std;


//namespace {
//    std::unordered_map<char, Operator> operators({
//       {'+', Operator(100, [](double a, double b){ return a + b; })},
//       {'-', Operator(100, [](double a, double b){ return a - b; })},
//       {'*', Operator(200, [](double a, double b){ return a * b; })},
//       {'/', Operator(200, [](double a, double b){ return a / b; })}
//    });
//
//    std::unordered_map<string, double(*)(double)> functions({
//        {"sin", [](double x){ return sin(x); }},
//        {"cos", [](double x){ return cos(x); }},
//        {"tan", [](double x){ return tan(x); }},
//        {"cot", [](double x){ return (1 / tan(x)); }},
//        {"abs", [](double x){ return abs(x); }},
//        {"-", [](double x){ return -x; }}
//    });
//}


Operator::Operator(unsigned int priority, double(*func)(double, double), Associativity assoc) {
    this->priority = priority;
    this->func = func;
    this->assoc = assoc;
}

Token::Token(std::string value, Type type) {
    this->value = value;
    this->type = type;
}

Token::Token(const char chr, Type type) {
    this->value = chr;
    this->type = type;
}

bool isOperator(char value) {
    return (operators.find(value) != operators.end());
}

bool isFunction(const string& name) {
    return (functions.find(name) != functions.end());
}

string constructNumber(string::const_iterator& it, string::const_iterator end) {
    string value;
    bool dotExists = false;
    while (it != end && (isdigit(*it) || *it == '.')) {
        value += *it;
        if (*it == '.') {
            if (dotExists) {
                throw InvalidInput("Invalid number in input");
            }
            else {
                dotExists = true;
            }
        }
        it++;
    }
    if (value.empty()) {
        throw InvalidInput("Excepted a number, but nothing found");
    }
    return value;
}

string constructName(string::const_iterator& it, string::const_iterator end) {
    string value;
    while (it != end && isalpha(*it)) {
        value += *it;
        it++;
    }
    if (value.empty()) {
        throw InvalidInput("Excepted a name, nothing found");
    }
    return value;
}

queue<Token> tokenize(const string& expression) {
    queue<Token> tokens;
    auto it = expression.cbegin();
    while (it != expression.cend()) {
        if (whitespaces.find(*it) != whitespaces.cend()) {

        }
        else if (isdigit(*it) || *it == '.') {  // construct number
            string number = constructNumber(it, expression.cend());
            tokens.emplace(number, Token::Type::Number);
            continue;
        }
        else if (isOperator(*it)) {
            tokens.emplace(*it, Token::Type::Operator);
        }
        else if (*it == '(') {
            tokens.emplace(*it, Token::Type::LeftBrace);
        }
        else if (*it == ')') {
            tokens.emplace(*it, Token::Type::RightBrace);
        }
        else if (isalpha(*it)) {  // construct symbol
            string name = constructName(it, expression.cend());
            if (isFunction(name)) {
                tokens.emplace(name, Token::Type::Name);
            }
            else {
                throw InvalidInput("Name not exists: " + name);
            }
            continue;
        }
        it++;
    }

    return tokens;
}

queue<string> shuntingYard(queue<Token> tokens) {
    stack<Token> stack;
    Token token;

    while (!tokens.empty()) {
        token = tokens.front();
        tokens.pop();
        switch ()
    }

    return queue<string>();
}

double eval(const vector<string>& rpmExpression) {
    return 0.0;
}
