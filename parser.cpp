#include "parser.h"

std::unique_ptr<raw_AST_nodes::Expression> Parser::parse() {
    auto result = parseExpression();

    if (current().type != TokenType::END) {
        throw std::runtime_error("Unexpected token after expression");
    }
    return result;
}

std::unique_ptr<raw_AST_nodes::Expression> Parser::parseExpression() {
    auto left = parseTerm();
    while (current().type == TokenType::PLUS || current().type == TokenType::MINUS) {
        TokenType op = current().type;
        consume(op);
        auto right = parseTerm();
        if (op == TokenType::PLUS) {
            left = std::make_unique<raw_AST_nodes::AddExpression>(std::move(left), std::move(right));
        } else {
            left = std::make_unique<raw_AST_nodes::SubtractExpression>(std::move(left), std::move(right));
        }
    }
    return left;
}

std::unique_ptr<raw_AST_nodes::Expression> Parser::parseTerm() {
    auto left = parseFactor();

    while( current().type == TokenType::STAR ) {
        consume( TokenType::STAR );
        auto right = parseFactor();
        left = std::make_unique<raw_AST_nodes::MultiplyExpression>(
            std::move( left ), std::move( right ) );
    }
    return left;
}

std::unique_ptr<raw_AST_nodes::Expression> Parser::parseFactor() {
    auto left = parsePrimary();

    if( current().type == TokenType::CARET ) {
        consume( TokenType::CARET );
        Token token = current();
        if( token.type != TokenType::NUMBER ) {
            throw std::runtime_error("expected integef after caret( ^ )");
        }
        if( token.double_number != 0 || token.int_value < 1 ) {
            throw std::runtime_error("Expected positive integer value( >=1 ) after caret( ^ )");
        }
        consume( TokenType::NUMBER );
        left = std::make_unique<raw_AST_nodes::PowerExpression>(
            std::move( left ), std::make_unique<raw_AST_nodes::Exponent>(token.int_value));
    }
    return left;
}

std::unique_ptr<raw_AST_nodes::Expression> Parser::parsePrimary() {
    Token token = current();
    consume( token.type );
    switch( token.type ) {
    case TokenType::MATRIX: return std::make_unique<raw_AST_nodes::MatrixVariable>( token.lexeme );
    case TokenType::NUMBER: {
        if( token.double_number != 0) {
            return std::make_unique<raw_AST_nodes::NumberLiteral>(token.double_number);
        }
        else {
            return std::make_unique<raw_AST_nodes::NumberLiteral>(token.int_value);
        }
    }

    case TokenType::LPAREN:
    {
        auto middle = parseExpression();
        consume(TokenType::RPAREN);
        return std::move( middle );
    }
    default:
        throw std::runtime_error("Invaled sequence of characters");
    }
}


