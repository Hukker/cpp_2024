#include <map>
#include <stdexcept>
#include <format>
#include "Token.h"


Token::Token(std::string token, Type type, OperatorAssociativity asc) : type{ type }, str{ token }
{
    if (type == OPERATOR && asc == NONE) {
        throw std::logic_error("Associativity required!");
    }
    else if (type != OPERATOR && asc != NONE) {
        throw std::logic_error("Non-operator token can't have an associativity!");
    }
    opAsc = asc;
}

int Token::getPrecendance() const
{
    static std::map<std::string, int> op_leftassociative =
    {
        {"+", 2},
        {"-", 2},
        {"/", 3},
        {"*", 3},
        {"^", 5}
    };


    static std::map<std::string, int> op_rightassociative =
    {
        {"-", 4} 
    };

    switch (opAsc)
    {
    case LEFT:
        if (op_leftassociative.contains(str)) return op_leftassociative[str];
        else throw std::invalid_argument("Unknown operator");
        break;
    case RIGHT:
        if (op_rightassociative.contains(str)) return op_rightassociative[str];
        else throw std::invalid_argument("Unknown operator");
        break;
    case NONE:
        throw std::logic_error(
            std::format("\"{}\"not an operatator, impossible.", str)
        );
        break;
    }
}