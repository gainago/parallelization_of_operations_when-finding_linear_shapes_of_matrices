#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <vector>
#include <stdexcept>

#include "Tokens.h"
#include "raw_AST_nodes.h"

/* Source Gramma
 * <Expression>         ::= <Term> ( ( "+" | "-" ) <Term> )^*
 * <Term>               ::= <Factor> ( "*" <Factor> )^*
 * <Factor>             ::= <Primary> ( "^" <Unsigned_Integer> )?
 * <Primary>            ::= <Matrix_Variable> | <Number_Literal> | "(" <Expression> ")"
 * <Matrix_Variable>    ::= "A" <Digit>+
 * <Number_Literal>     ::= [ "+" | "-" ] <Digit>+ [ "." <Digit>+ ]
 * <Unsigned_Integer>   ::= <Digit>+
 * <Digit>              ::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
 */
//->->-> Normalize ->->->
/* Tokens Grammar (normalized token sequence for parser)
 * <Expression>    ::= <Term> ( ( PLUS | MINUS ) <Term> )^*
 * <Term>          ::= <Factor> ( STAR <Factor> )^*
 * <Factor>        ::= <Primary> ( CARET <Number> )?
 * <Primary>       ::= MATRIX | NUMBER | LPAREN <Expression> RPAREN
 */


class Parser {
    std::vector<Token> tokens;
    size_t position = 0;

    Token& current() {
        return tokens[position];
    }

    void consume( TokenType expected ) {
        if( current().type != expected ) {
            throw std::runtime_error( "Unexpected token");
        }
        ++position;
    }

    std::unique_ptr<raw_AST_nodes::Expression> parseExpression();
    std::unique_ptr<raw_AST_nodes::Expression> parseTerm();
    std::unique_ptr<raw_AST_nodes::Expression> parseFactor();
    std::unique_ptr<raw_AST_nodes::Expression> parsePrimary();

public:
    Parser( std::vector<Token> tokens_ ) : tokens( std::move( tokens_ ) ), position( 0 ) {}

    std::unique_ptr<raw_AST_nodes::Expression> parse();

};

#endif // PARSER_H
