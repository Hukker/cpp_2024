#include <iostream>
#include <string>
#include "Calculator.h"
#include "Loader.h"

int main() {
    Loader loader("plugins");

    Calculator calculator(loader);
    std::string expression;

    do {
        std::cout << "Enter an expression ('_' to exit): ";

        if (!std::getline(std::cin, expression)) {
            break;
        }

        if (expression == "_") {
            break;
        }

        try {
            calculator.evaluate_expression(expression);
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }

    } while (true);

    return 0;
}