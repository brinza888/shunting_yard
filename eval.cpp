//
// Created by brinza on 19.05.22.
//

#include "eval.h"
#include <cctype>

using namespace std;


vector<Token> tokenize(const string& expression) {
    vector<Token> tokens;
    string token;
    TokenType state = TokenType::None, prevState = TokenType::None;
    for (auto&& chr: expression) {
        if (chr == ' ') {  // skip whitespaces
            state = TokenType::None;
        }
        else if (isdigit(chr)) {
            state = TokenType::Number;
        }
        else if (isalpha(chr)) {
            state = TokenType::None;
        }

        if (prevState != state) {
            tokens.push_back({prevState});
        }
    }

    return tokens;
}

vector<string> shuntingYard(const vector<string>& tokens) {
    return vector<string>();
}

double eval(const vector<string>& rpmExpression) {
    return 0.0;
}
