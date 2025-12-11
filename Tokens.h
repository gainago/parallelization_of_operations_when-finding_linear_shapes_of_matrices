#ifndef TOKENS_H
#define TOKENS_H

#include <string>
#include <vector>

enum class TokenType {
    MATRIX,
    NUMBER,
    PLUS,
    MINUS,
    STAR,
    CARET,
    LPAREN,
    RPAREN,
    END
};

struct Token {
    TokenType type;
    std::string lexeme;
    double double_number = 0.0;
    int int_value = 0;
};

std::string token_type_to_string( TokenType type );

std::vector<Token> tokenize( const std::string& input );
void check_arithmetic_signs( std::vector<Token> const& tokenize_input );
void supplement_with_implied_tokens( std::vector<Token>& tokenize_input );
void replace_minus_in_sign_pairs( std::vector<Token> & tokenize_checked_input );
void replace_first_unary_minus( std::vector<Token>& tokenize_input );
void replace_unary_pluses( std::vector<Token>& tokenize_input );
void normalize_tokenize_sequence( std::vector<Token>& tokenize_input );
#endif // TOKENS_H
