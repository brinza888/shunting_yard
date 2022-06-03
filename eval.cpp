//
// Created by brinza on 19.05.22.
//

#include "eval.h"
#include "iostream"
#include <cctype>
#include <stack>

using namespace std;
using namespace Eval;


Operator::Operator(const string& name, unsigned int priority, double(*func)(double, double), Associativity assoc):
    name(name),
    priority(priority),
    func(func),
    assoc(assoc)
{

}

Function::Function(const string& name, size_t argc, double (*func)(vector<double>& args)):
    name(name),
    func(func),
    argc(argc)
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

Token::Token(Token::Type type) {
    this->type = type;
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

std::ostream& Eval::operator<<(std::ostream& stream, Token token) {
    if (token.type == Token::Type::Number) {
        stream << token.value.number;
    }
    else if (token.type == Token::Type::LeftBrace) {
        stream << "(";
    }
    else if (token.type == Token::Type::RightBrace) {
        stream << ")";
    }
    else if (token.type == Token::Type::Operator) {
        stream << token.value.oper->name;
    }
    else if (token.type == Token::Type::Function) {
        stream << token.value.func->name;
    }
    else if (token.type == Token::Type::ArgsSep) {
        stream << ',';
    }
    return stream;
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

vector<Token> Eval::tokenize(const string& expression) {
    vector<Token> tokens;

    OperatorMap::const_iterator operatorIt;
    FunctionMap::const_iterator functionIt;
    auto it = expression.cbegin();

    MinusState minusState = MinusState::UNARY;

    while (it != expression.cend()) {
        if (whitespaces.find(*it) != whitespaces.cend()) {
            // handle whitespace characters
        }
        else if (isdigit(*it) || *it == '.') {  // handle number
            if (!tokens.empty() && (tokens.back().type != Token::Type::Operator &&
                                    tokens.back().type != Token::Type::LeftBrace &&
                                    tokens.back().type != Token::Type::ArgsSep)) {
                throw SyntaxError("Missing '(' or ',' or operator  before number");
            }
            string numberStr = constructNumber(it, expression.cend());
            double number = stod(numberStr);
            tokens.emplace(tokens.end(), number);
            minusState = MinusState::BINARY;
            continue;
        }
        else if ((operatorIt = operators.find(*it)) != operators.cend()) {  // handle operator
            Operator* oper = operatorIt->second;
            if (minusState == MinusState::UNARY && oper->name == "-") {  // handle unary minus
                tokens.emplace(tokens.end(), 0.0);
            }
            else {  // check binary operators
                if (!tokens.empty() && (tokens.back().type != Token::Type::Number &&
                                        tokens.back().type != Token::Type::RightBrace)) {
                    throw SyntaxError("Missing ')' or number before binary operator");
                }
            }
            tokens.emplace(tokens.end(), oper);
        }
        else if (*it == '(') {
            if (!tokens.empty() && (tokens.back().type != Token::Type::Operator &&
                                    tokens.back().type != Token::Type::Function &&
                                    tokens.back().type != Token::Type::LeftBrace &&
                                    tokens.back().type != Token::Type::ArgsSep)) {
                throw SyntaxError("Missing '(' or ',' or operator or function before '('");
            }
            tokens.emplace(tokens.end(), *it);
            minusState = MinusState::UNARY;
        }
        else if (*it == ')') {
            if (!tokens.empty() && (tokens.back().type != Token::Type::Number &&
                                    tokens.back().type != Token::Type::RightBrace)) {
                throw SyntaxError("Missing ')' or number before ')'");
            }
            tokens.emplace(tokens.end(), *it);
            minusState = MinusState::BINARY;
        }
        else if (*it == ',') {
            if (tokens.empty() ||
                !tokens.empty() && (tokens.back().type != Token::Type::Number &&
                                    tokens.back().type != Token::Type::RightBrace)) {
                throw SyntaxError("Missing ')' or number before ','");
            }
            tokens.emplace(tokens.end(), Token::Type::ArgsSep);
            minusState = MinusState::UNARY;
        }
        else if (isalpha(*it)) {  // handle function name
            if (!tokens.empty() && (tokens.back().type != Token::Type::Operator &&
                                    tokens.back().type != Token::Type::LeftBrace &&
                                    tokens.back().type != Token::Type::ArgsSep)) {
                throw SyntaxError("Missing operator before function");
            }
            string name = constructName(it, expression.cend());
            functionIt = functions.find(name);
            if (functionIt != functions.cend()) {
                tokens.emplace(tokens.end(), functionIt->second);
            }
            else {
                throw InvalidInput("Function not exists: " + name);
            }
            minusState = MinusState::BINARY;
            continue;
        }
        else {  // if no one case success
            throw InvalidInput("Invalid char: " + string(1, *it));
        }
        ++it;
    }

    return tokens;
}

vector<Token> Eval::shuntingYard(const vector<Token>& input) {
    stack<Token> stack;
    vector<Token> output;

    for (auto&& token: input) {
        if (token.type == Token::Type::Number) {
            output.push_back(token);
        }
        else if (token.type == Token::Type::Function) {
            stack.push(token);
        }
        else if (token.type == Token::Type::ArgsSep) {
            Token top;
            bool argsCheck = false;
            while (!stack.empty()) {
                top = stack.top();
                if (top.type == Token::Type::LeftBrace) {
                    argsCheck = true;
                    break;
                }
                output.push_back(top);
                stack.pop();
            }
            if (!argsCheck) {
                throw SyntaxError("Missing argument separator ',' or left brace '(' in expression");
            }
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

double Eval::eval(const vector<Token>& rpnExpression) {
    stack<double> stack;
    for (auto&& token: rpnExpression) {
        if (token.type == Token::Type::Number) {
            stack.push(token.value.number);
        }
        else if (token.type == Token::Type::Operator) {
            if (stack.size() < 2) {
                throw SyntaxError("Not enough operands for operator: " + token.value.oper->name);
            }
            double b = stack.top();
            stack.pop();
            double a = stack.top();
            stack.pop();
            stack.push(token.value.oper->func(a, b));
        }
        else if (token.type == Token::Type::Function) {
            size_t argc = token.value.func->argc;
            if (stack.size() < argc) {
                throw SyntaxError("Not enough arguments for function: " + token.value.func->name);
            }
            vector<double> args;
            for (size_t i = 0; i < argc; i++) {
                args.insert(args.begin(), stack.top());
                stack.pop();
            }
            stack.push(token.value.func->func(args));
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

double Eval::eval(const std::string& expression) {
    auto tokens = tokenize(expression);
    auto rpn = shuntingYard(tokens);
    double result = eval(rpn);
    return result;
}
