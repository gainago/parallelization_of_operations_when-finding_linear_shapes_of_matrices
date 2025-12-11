#include <regex>
#include <string>
#include <vector>

#include "Tokens.h"

std::string token_type_to_string( TokenType type ) {
    switch( type ) {
    case TokenType::MATRIX: return "MATRIX";
    case TokenType::NUMBER: return "NUMBER";
    case TokenType::PLUS:   return "PLUS";
    case TokenType::MINUS:  return "MINUS";
    case TokenType::STAR:   return "STAR";
    case TokenType::CARET:  return "CARET";
    case TokenType::LPAREN: return "LPAREN";
    case TokenType::RPAREN: return "RPAREN";
    case TokenType::END:    return "END";
    default:                return "UNKNOWN";
    }
}

std::vector<Token> tokenize( const std::string& input ) {
    std::vector<Token> tokens;
    //remove spaces
    std::string normalized_input = std::regex_replace( input, std::regex( "\\s+" ), "" );
    size_t position = 0;
    while( position < normalized_input.size() ) {
        char symbol = normalized_input[position];
        if( symbol == '+' ) {
            tokens.push_back( {TokenType::PLUS, "+"} );
            ++position;
        }
        else if( symbol == '-' ) {
            tokens.push_back( {TokenType::MINUS, "-"} );
            ++position;
        }
        else if( symbol == '*' ) {
            tokens.push_back( {TokenType::STAR, "*"} );
            ++position;
        }
        else if( symbol == '^' ) {
            tokens.push_back( {TokenType::CARET, "^"} );
            ++position;
        }
        else if( symbol == '(' ) {
            tokens.push_back( {TokenType::LPAREN, "("} );
            ++position;
        }
        else if( symbol == ')' ) {
            tokens.push_back( {TokenType::RPAREN, ")"} );
            ++position;
        }
        else if( std::isdigit( symbol ) ) {
            //match number a.b
            size_t start_number_position = position;
            bool is_int = true;
            while( position < normalized_input.size() &&
                   ( std::isdigit( normalized_input[position] ) ||
                    normalized_input[position] == '.')
                   ) {
                if( normalized_input[position] == '.' ) {
                    is_int = false;
                }

                ++position;
            }
            std::string num_str = normalized_input.substr( start_number_position,
                                                          position - start_number_position);
            try {
                if( is_int ) {
                    int int_val = std::stoi( num_str );
                    tokens.push_back( {TokenType::NUMBER, num_str, 0, int_val} );
                }
                else {
                    double val = std::stod( num_str );
                    tokens.push_back( {TokenType::NUMBER, num_str, val} );
                }
            } catch(...) {
                throw std::runtime_error( "Invalid number: " + num_str );
            }
        }
        else if( symbol == 'A' ) {
            size_t start_position = position;
            ++position;
            while( position < normalized_input.size() && std::isdigit( normalized_input[position] )) {
                ++position;
            }
            if( position == start_position + 1 ) {
                throw std::runtime_error( "Matrix name must have digits after 'A'");
            }
            std::string name_str = normalized_input.substr( start_position, position - start_position);
            tokens.push_back( {TokenType::MATRIX, name_str});
        }
        else {
            throw std::runtime_error( "Unexpected character: " + std::string( 1, symbol ) );
        }
    }
    tokens.push_back({TokenType::END, ""});
    return tokens;
}

void supplement_with_implied_tokens( std::vector<Token>& tokenize_input ){

    for( size_t position = 0; (position + 1) != tokenize_input.size(); ++position ) {

        TokenType current_type = tokenize_input[position].type;
        TokenType next_type = tokenize_input[position + 1].type;
        if( current_type == TokenType::NUMBER  &&
            ( next_type == TokenType::MATRIX || next_type == TokenType::LPAREN ) ) {
            tokenize_input.insert( tokenize_input.begin() + position + 1, Token{TokenType::STAR});
        }
        if( current_type == TokenType::RPAREN  &&
            ( next_type == TokenType::MATRIX || next_type == TokenType::LPAREN||
                                                  next_type == TokenType::NUMBER ) ) {
            tokenize_input.insert( tokenize_input.begin() + position + 1, Token{TokenType::STAR});
        }
    }
}

void check_arithmetic_signs( std::vector<Token> const & tokenize_input ) {

    auto is_arithmetic_sign = [](TokenType type)->bool {
        if( type == TokenType::CARET ||
            type == TokenType::MINUS ||
            type == TokenType::PLUS ||
            type == TokenType::STAR ) {
            return true;
        }
        return false;
    };

    auto is_valid_pair = []( TokenType type_first, TokenType type_second)->bool {
        if( type_first == TokenType::STAR && type_second == TokenType::MINUS ) {
            return true;
        }
        if( type_first == TokenType::PLUS && type_second == TokenType::MINUS ) {
            return true;
        }
        return false;
    };

    int count_of_continues_signs = 0;
    for( size_t position = 0; (position + 1) < tokenize_input.size(); ++position ) {
        if( is_arithmetic_sign( tokenize_input[position].type ) ) {
            ++count_of_continues_signs;
        }
        else {
            count_of_continues_signs = 0;
        }

        if( count_of_continues_signs == 2 ) {
            if( !is_valid_pair( tokenize_input[position - 1].type,
                               tokenize_input[position].type ) ) {
                throw std::runtime_error("Invalid pair of arithmetic symbols");
            }
        }
        if( count_of_continues_signs == 3) {
            throw std::runtime_error("Sequence of three arithmetic symbols is forbidden");
        }
        if( tokenize_input[position].type == TokenType::CARET &&
            tokenize_input[position + 1].type != TokenType::NUMBER ) {
            throw std::runtime_error("After caret( ^ ) must follow Number");
        }
    }

    if( is_arithmetic_sign( tokenize_input[ tokenize_input.size() - 1 - 1].type ) ) {//last symbol
        throw std::runtime_error( "Last symbol could not be arithmetic");               //in source sequence
    }

    if( is_arithmetic_sign( tokenize_input[0].type ) &&
        tokenize_input[0].type != TokenType::MINUS) {
        throw std::runtime_error( "Forbidden first arithmetic symbol");
    }
}

void replace_minus_in_sign_pairs( std::vector<Token> & tokenize_input ) {

    for( size_t position = 0; (position + 1) < tokenize_input.size(); ++position) {
        if( tokenize_input[position + 1].type == TokenType::MINUS &&
            (tokenize_input[position].type == TokenType::STAR ||
            tokenize_input[position].type == TokenType::PLUS)) {

            std::vector<Token> replase_minus{
                Token{TokenType::LPAREN, "(", 0, 0},
                Token{TokenType::NUMBER, "-1", -1.0, -1},
                Token{TokenType::RPAREN, ")", 0, 0},
                Token{TokenType::STAR, "*", 0, 0},
                };
            tokenize_input.erase( tokenize_input.begin() + position + 1);
            tokenize_input.insert(tokenize_input.begin() + position + 1, replase_minus.begin(), replase_minus.end() );
        }
    }
}

void replace_first_unary_minus( std::vector<Token>& tokenize_input ) {
    if( tokenize_input[0].type == TokenType::MINUS ) {

        std::vector<Token> replase_minus{
                                         Token{TokenType::LPAREN, "(", 0.0, 0},
                                         Token{TokenType::NUMBER, "-1", -1.0, -1},
                                         Token{TokenType::RPAREN, ")", 0.0, 0},
                                         Token{TokenType::STAR, "*", 0.0, 0},
                                         };
        tokenize_input.erase( tokenize_input.begin() );
        tokenize_input.insert(tokenize_input.begin(), replase_minus.begin(), replase_minus.end() );
    }
}

void replace_unary_pluses( std::vector<Token>& tokenize_input ) {
    for( size_t position = 0; (position + 1) < tokenize_input.size();) {
        if(tokenize_input[position].type == TokenType::PLUS && (
                position == 0 || tokenize_input[position - 1].type == TokenType::LPAREN)
            ) {
            tokenize_input.erase( tokenize_input.begin() + position );
        } else {
            ++position;
        }
    }
}

void normalize_tokenize_sequence( std::vector<Token>& tokenize_input ) {
    supplement_with_implied_tokens( tokenize_input );
    replace_minus_in_sign_pairs( tokenize_input );
    replace_first_unary_minus( tokenize_input );
    replace_unary_pluses( tokenize_input );
}

