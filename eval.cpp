//
// Created by brinza on 19.05.22.
//

#include "eval.h"
#include <cctype>
#include <stack>

using namespace std;


Operator::Operator(const string& name, unsigned int priority, double(*func)(double, double), Associativity assoc):
    name(name),
    priority(priority),
    func(func),
    assoc(assoc)
{

}

Function::Function(const string& name, double (*func)(double)):
    name(name),
    func(func)
{

}

Token::Token(double number) {
    value.number = number;
    type = Type::Number;
}

Token::Token(Operator* oper) {
    value.oper = oper;
    type = Type::Operator;
}

Token::Token(Function* func) {
    value.func = func;
    type = Type::Function;
}

Token::Token(char brace) {
    if (brace == '(') {
        type = Type::LeftBrace;
    }
    else if (brace == ')') {
        type = Type::RightBrace;
    }
    else {
        throw InvalidInput("Excepted brace");
    }
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

vector<Token> tokenize(const string& expression) {
    vector<Token> tokens;

    OperatorMap::const_iterator operatorIt;
    FunctionMap::const_iterator functionIt;
    auto it = expression.cbegin();

    while (it != expression.cend()) {
        if (whitespaces.find(*it) != whitespaces.cend()) {

        }
        else if (isdigit(*it) || *it == '.') {  // construct number
            string numberStr = constructNumber(it, expression.cend());
            double number = stod(numberStr);
            tokens.emplace(tokens.end(), number);
            continue;
        }
        else if ((operatorIt = operators.find(*it)) != operators.cend()) {
            tokens.emplace(tokens.end(), operatorIt->second);
        }
        else if (*it == '(' || *it == ')') {
            tokens.emplace(tokens.end(), *it);
        }
        else if (isalpha(*it)) {  // construct symbol
            string name = constructName(it, expression.cend());
            functionIt = functions.find(name);
            if (functionIt != functions.cend()) {
                tokens.emplace(tokens.end(), functionIt->second);
            }
            else {
                throw InvalidInput("Function not exists: " + name);
            }
            continue;
        }
        else {
            throw InvalidInput("Invalid char in input");
        }
        it++;
    }

    return tokens;
}

vector<Token> shuntingYard(const vector<Token>& input) {
    stack<Token> stack;
    vector<Token> output;

    for (auto&& token: input) {
        if (token.type == Token::Type::Number) {
            output.push_back(token);
        }
        else if (token.type == Token::Type::Function) {
            stack.push(token);
        }
        else if (token.type == Token::Type::Operator) {
            Operator* cur = token.value.oper;
            while (!stack.empty()) {
                Token top = stack.top();
                if (top.type == Token::Type::Operator) {
                    Operator* op = top.value.oper;
                    if (op->priority > cur->priority ||
                        (op->priority == cur->priority && cur->assoc == Operator::Associativity::Left)) {
                        output.push_back(top);
                        stack.pop();
                    }
                    else {
                        break;
                    }
                }
                else if (top.type == Token::Type::Function) {
                    output.push_back(top);
                    stack.pop();
                }
                else {
                    break;
                }
            }
            stack.push(token);
        }
        else if (token.type == Token::Type::LeftBrace) {
            stack.push(token);
        }
        else if (token.type == Token::Type::RightBrace) {
            bool bracesCheck = false;
            while (!stack.empty()) {
                Token top = stack.top();
                stack.pop();
                if (top.type == Token::Type::LeftBrace) {
                    bracesCheck = true;
                    break;
                }
                output.push_back(top);
            }
            if (!bracesCheck) {
                throw SyntaxError("Missing left brace '(' in expression");
            }
        }
    }

    while (!stack.empty()) {
        Token top = stack.top();
        stack.pop();
        if (top.type == Token::Type::LeftBrace) {
            throw SyntaxError("Missing right brace ')' in expression");
        }
        output.push_back(top);
    }

    return output;
}

double eval(const vector<Token>& rpnExpression) {
    stack<double> stack;
    for (auto&& token: rpnExpression) {
        if (token.type == Token::Type::Number) {
            stack.push(token.value.number);
        }
        else if (token.type == Token::Type::Operator) {
            if (stack.size() < 2) {
                throw SyntaxError("Not enough operands for Operator " + token.value.oper->name);
            }
            double b = stack.top();
            stack.pop();
            double a = stack.top();
            stack.pop();
            stack.push(token.value.oper->func(a, b));
        }
        else if (token.type == Token::Type::Function) {
            if (stack.size() < 1) {
                throw SyntaxError("Not enough arguments for Function " + token.value.func->name);
            }
            double x = stack.top();
            stack.pop();
            stack.push(token.value.func->func(x));
        }
        else {
            throw SyntaxError("In RPN met not allowed token: " + std::to_string(static_cast<int>(token.type)));
        }
    }
    if (stack.size() > 1) {
        throw SyntaxError("Calculation finished with stack size greater than 1");
    }
    return stack.top();
}
