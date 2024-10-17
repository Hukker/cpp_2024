#include <iostream>
#include <stdexcept>
#include <algorithm>
#include "calculator.h"

Calculator::Calculator(Loader& loader) : loader(loader) {}

Calculator::~Calculator() {}

std::string Calculator::skim_expression(const std::string& expression) {
    std::string res = expression;
    res.erase(std::remove(res.begin(), res.end(), ' '), res.end());
    return res;
}

double Calculator::process_function(const std::string& funcName, double arg) {
    return loader.method_function(funcName, arg);
}
double Calculator::process_function(const std::string& funcName, double arg1, double arg2) {
    return loader.method_function(funcName, arg1, arg2);
}

void Calculator::shunting_yard(const std::vector<Token>& expr, std::vector<Token>& outQueue) {
    std::stack<Token> stack;
    auto fromStackToQueue = [&]() { outQueue.push_back(stack.top()); stack.pop(); };
    for (const auto& token : expr)
    {
        switch (token.getType())
        {
        case Token::INT_LITERAL:
        case Token::FLOAT_LITERAL:
            outQueue.push_back(token);
            break;
        case Token::L_PARANTHESIS:
        case Token::FUNCTION:
            stack.push(token);
            break;
        case Token::OPERATOR:
            if (!stack.empty())
            {
                while (stack.top().getType() == Token::OPERATOR && ((stack.top().getPrecendance() > token.getPrecendance())
                    || (stack.top().getPrecendance() == token.getPrecendance() && token.getAsc() == Token::LEFT)))
                {
                    fromStackToQueue();
                    if (stack.empty())
                        break;
                }
            }
            stack.push(token);
            break;

        case Token::R_PARANTHESIS:
            if (stack.empty())
                throw std::invalid_argument("Non-balanced on paranthesis expression!");
            while (stack.top().getType() != Token::L_PARANTHESIS)
            {
                fromStackToQueue();
                if (stack.empty())
                    throw std::invalid_argument("Non-balanced on paranthesis expression!");
            }
            stack.pop();
            if (!stack.empty() && stack.top().getType() == Token::FUNCTION)
                fromStackToQueue();
            break;

        case Token::SEPARATOR:
            if (stack.empty())
                throw std::invalid_argument("Paranthesis or separator missed!");
            while (stack.top().getType() != Token::L_PARANTHESIS)
            {
                fromStackToQueue();
                if (stack.empty())
                    throw std::invalid_argument("Paranthesis-unbalanced expression!");
            }
            break;
        }
    }
    while (!stack.empty())
    {
        if (stack.top().getType() == Token::L_PARANTHESIS)
            throw std::invalid_argument("Paranthesis-unbalanced expression!");
        else
            fromStackToQueue();
    }
}

double Calculator::process_expression(std::vector<Token>& rpn_expr) {
    std::stack<double> stack;
    auto getOneToken = [&]()
        {
            if (stack.empty()) throw std::invalid_argument("Not enough arguments in function!");
            double x = stack.top();
            stack.pop();
            return x;
        };
    auto getTwoTokens = [&]()
        {
            double x = getOneToken(), y = getOneToken();
            return std::tuple{ y,x };
        };
    auto checkedDivision = [&](double a, double b)
        {
            if (b == 0.f) throw std::invalid_argument("Division by zero");
            return a / b;
        };

    double res;
    for (auto& token : rpn_expr)
    {
        const std::string& str = token.getStr();

        switch (token.getType())
        {

        case Token::INT_LITERAL:
            stack.push(std::stof(str));
            break;

        case Token::FLOAT_LITERAL:
            stack.push(std::stof(str));
            break;

        case Token::OPERATOR:

            switch (token.getAsc())
            {

            case Token::LEFT:
            {
                auto [a, b] = getTwoTokens();
                if (str == "+") res = a + b;
                else if (str == "-") res = a - b;
                else if (str == "*") res = a * b;
                else if (str == "/") res = checkedDivision(a, b);
                else if (str == "^") res = process_function("pow", a, b);
                else throw std::invalid_argument("Unknown operator!");
                break;
            }

            case Token::RIGHT:
            {
                auto a = getOneToken();
                if (str == "-") {
                    res = -a;
                }
                else if (str == "+") {
                    res = a;
                }
                else {
                    throw std::invalid_argument("Unknown operator!");
                }
                break;
            }

            case Token::NONE: {
                throw std::invalid_argument("Operator must have associativity!");
                break;
            }
            }
            stack.push(res);
            break;
        case Token::FUNCTION:
            auto a = getOneToken();
            res = process_function(token.getStr(), a);
            stack.push(res);

            break;
        }
    }
    return stack.top();
}


void Calculator::evaluate_expression(const std::string& expression) {
    std::string skimmed_expr = skim_expression(expression);
    std::vector<Token> token_expr;
    std::vector<Token> rpn_token_expr;
    tokenize(skimmed_expr, token_expr);
    shunting_yard(token_expr, rpn_token_expr);
    double ans = process_expression(rpn_token_expr);
    std::cout << ans << std::endl;
}