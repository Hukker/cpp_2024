#pragma once
#include <string>

class Token
{
public:
    // type
    enum Type
    {
        OPERATOR,      // un/bin operation
        L_PARANTHESIS, // opened bracked
        R_PARANTHESIS, // closed bracked
        INT_LITERAL,   // integer number
        FLOAT_LITERAL, // float literal 
        FUNCTION,      // function
        SEPARATOR      // separate args
    };

    // Ассоциативность
    enum OperatorAssociativity
    {
        NONE,  // not an operator
        RIGHT, // rightassociated
        LEFT   // leftassociated
    };

    Token(std::string token, Type type, OperatorAssociativity asc = NONE);

    // priority
    int getPrecendance() const;

    const Type& getType() const { return type; }
    const OperatorAssociativity& getAsc() const { return opAsc; }
    const std::string& getStr() const { return str; }

private:
    Type type;
    OperatorAssociativity opAsc;
    std::string str;
};
