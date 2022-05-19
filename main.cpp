#include <iostream>
#include "unordered_map"


double add(double l, double r) {
    return l + r;
}

double mul(double l, double r) {
    return l * r;
}

std::unordered_map<std::string, double(*)(double, double)> operators ({
    {"+", &add},
    {"*", &mul}
});


int main() {

    // ... code ...

    std::cout << operators.at("+")(10, 4) << std::endl;
    std::cout << operators.at("*")(10, 4) << std::endl;

    // ... code ...

    return 0;
}
