#include <iostream>
#include <format>
#include <map>
#include <string>
#include <vector>
#include <cctype>
#include <stdexcept>

enum State { S0, S1, S2, S3, S4, S5 };

class Token {
public:
    enum Type {
        INT_LITERAL,
        FLOAT_LITERAL,
        FUNCTION,
        OPERATOR,
        LEFT_PARANTHESIS,
        RIGHT_PARANTHESIS,
        SEPARATOR
    };

    Token(const std::string& value, Type type, Type direction = LEFT_PARANTHESIS)
        : value(value), type(type), direction(direction) {}

    Type getType() const { return type; }
    const std::string& getValue() const { return value; }

private:
    std::string value;
    Type type;
    Type direction;
};

void tokenizeOpParanthSep(bool isOp, bool isParanth, bool isRParanth, bool isSep, std::vector<Token>& tokens) {
    if (isOp) {
        if (tokens.empty() || tokens.back().getType() == Token::LEFT_PARANTHESIS) {
            tokens.push_back({ std::string(1, (isOp ? '+' : '-')), Token::OPERATOR, Token::RIGHT_PARANTHESIS });
        }
        else {
            tokens.push_back({ std::string(1, (isOp ? '+' : '-')), Token::OPERATOR, Token::LEFT_PARANTHESIS });
        }
    }
    else if (isParanth) {
        tokens.push_back({ std::string(1, (isRParanth ? ')' : '(')), isRParanth ? Token::RIGHT_PARANTHESIS : Token::LEFT_PARANTHESIS });
    }
    else if (isSep) {
        tokens.push_back({ std::string(1, ','), Token::SEPARATOR });
    }
}

void tokenize(const std::string& expr, std::vector<Token>& tokens) {
    State state = S0;

    std::string validOperators = "+-*^/";

    bool isDigit, isLetter, isOp, isParanth, isPoint, isSep, isLParanth, isRParanth;

    std::string buffer;
    Token::Type bufferTokenType = Token::INT_LITERAL;

    for (auto& s : expr) {
        isDigit = std::isdigit(s);
        isLetter = std::isalpha(s);
        isLParanth = s == '(';
        isRParanth = s == ')';
        isParanth = isLParanth || isRParanth;
        isPoint = s == '.';
        isSep = s == ',';
        isOp = validOperators.find(s) != validOperators.npos;

        if (!(isDigit || isLetter || isParanth || isPoint || isSep || isOp))
            throw std::invalid_argument(std::format("Unknown symbol: {}", s));

        switch (state) {
        case S0:
            if (isOp || isParanth)
                state = S1;
            else if (isDigit)
                state = S2;
            else if (isLetter)
                state = S4;
            else if (isPoint || isSep)
                throw std::invalid_argument(std::format("Unexpected symbol: \"{}\"", s));
            break;
        case S1:
            if (isDigit)
                state = S2;
            else if (isLetter)
                state = S4;
            else if (isPoint || isSep)
                throw std::invalid_argument(std::format("Unexpected symbol: \"{}\"", s));
            break;
        case S2:
            bufferTokenType = Token::INT_LITERAL;
            if (isPoint)
                state = S3;
            else if (isParanth || isOp || isSep)
                state = S5;
            else if (isLetter)
                throw std::invalid_argument(std::format("Unexpected symbol: \"{}\"", s));
            break;
        case S3:
            bufferTokenType = Token::FLOAT_LITERAL;
            if (isParanth || isOp || isSep)
                state = S5;
            else if (isPoint)
                throw std::invalid_argument(std::format("Unexpected symbol: \"{}\"", s));
            break;
        case S4:
            bufferTokenType = Token::FUNCTION;
            if (isLParanth)
                state = S5;
            else if (isOp || isRParanth || isSep)
                throw std::invalid_argument(std::format("Unexpected symbol \"{}\"", s));
            break;
        case S5:
            if (isParanth || isOp)
                state = S1;
            else if (isDigit)
                state = S2;
            else if (isLetter)
                state = S4;
            else if (isPoint || isSep)
                throw std::invalid_argument(std::format("Unexpected symbol: \"{}\"", s));
            break;
        default:
            break;
        }

        switch (state) {
        case S1:
            tokenizeOpParanthSep(isOp, isParanth, isRParanth, isSep, tokens);
            break;
        case S2: case S3: case S4:
            buffer.push_back(s);
            break;
        case S5:
            tokens.push_back({ buffer, bufferTokenType });
            buffer.clear();
            tokenizeOpParanthSep(isOp, isParanth, isRParanth, isSep, tokens);
            break;
        }
    }
    if (!buffer.empty())
        tokens.push_back({ buffer, bufferTokenType });
}
