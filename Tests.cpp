#include <cassert>
#include <iostream>

#include "Tests.h"
#include "Tokens.h"
#include "parser.h"
#include "printer_to_dot.h"

void run_tests_for_tokenize() {
    std::string input = "44A1 + (13)( 52 * 34 - 2)A2^34 - A5";
    std::vector<Token> tokens = tokenize(input);

    // === Проверка базовой токенизации (до вставки STAR) ===
    assert(tokens.size() == 19);

    // Token 0
    assert(tokens[0].type == TokenType::NUMBER);
    assert(tokens[0].lexeme == "44");
    assert(tokens[0].double_number == 0.0);
    assert(tokens[0].int_value == 44);

    // Token 1
    assert(tokens[1].type == TokenType::MATRIX);
    assert(tokens[1].lexeme == "A1");
    assert(tokens[1].double_number == 0.0);
    assert(tokens[1].int_value == 0);

    // Token 2
    assert(tokens[2].type == TokenType::PLUS);
    assert(tokens[2].lexeme == "+");
    assert(tokens[2].double_number == 0.0);
    assert(tokens[2].int_value == 0);

    // Token 3
    assert(tokens[3].type == TokenType::LPAREN);
    assert(tokens[3].lexeme == "(");
    assert(tokens[3].double_number == 0.0);
    assert(tokens[3].int_value == 0);

    // Token 4
    assert(tokens[4].type == TokenType::NUMBER);
    assert(tokens[4].lexeme == "13");
    assert(tokens[4].double_number == 0.0);
    assert(tokens[4].int_value == 13);

    // Token 5
    assert(tokens[5].type == TokenType::RPAREN);
    assert(tokens[5].lexeme == ")");
    assert(tokens[5].double_number == 0.0);
    assert(tokens[5].int_value == 0);

    // Token 6
    assert(tokens[6].type == TokenType::LPAREN);
    assert(tokens[6].lexeme == "(");
    assert(tokens[6].double_number == 0.0);
    assert(tokens[6].int_value == 0);

    // Token 7
    assert(tokens[7].type == TokenType::NUMBER);
    assert(tokens[7].lexeme == "52");
    assert(tokens[7].double_number == 0.0);
    assert(tokens[7].int_value == 52);

    // Token 8
    assert(tokens[8].type == TokenType::STAR);
    assert(tokens[8].lexeme == "*");
    assert(tokens[8].double_number == 0.0);
    assert(tokens[8].int_value == 0);

    // Token 9
    assert(tokens[9].type == TokenType::NUMBER);
    assert(tokens[9].lexeme == "34");
    assert(tokens[9].double_number == 0.0);
    assert(tokens[9].int_value == 34);

    // Token 10
    assert(tokens[10].type == TokenType::MINUS);
    assert(tokens[10].lexeme == "-");
    assert(tokens[10].double_number == 0.0);
    assert(tokens[10].int_value == 0);

    // Token 11
    assert(tokens[11].type == TokenType::NUMBER);
    assert(tokens[11].lexeme == "2");
    assert(tokens[11].double_number == 0.0);
    assert(tokens[11].int_value == 2);

    // Token 12
    assert(tokens[12].type == TokenType::RPAREN);
    assert(tokens[12].lexeme == ")");
    assert(tokens[12].double_number == 0.0);
    assert(tokens[12].int_value == 0);

    // Token 13
    assert(tokens[13].type == TokenType::MATRIX);
    assert(tokens[13].lexeme == "A2");
    assert(tokens[13].double_number == 0.0);
    assert(tokens[13].int_value == 0);

    // Token 14
    assert(tokens[14].type == TokenType::CARET);
    assert(tokens[14].lexeme == "^");
    assert(tokens[14].double_number == 0.0);
    assert(tokens[14].int_value == 0);

    // Token 15
    assert(tokens[15].type == TokenType::NUMBER);
    assert(tokens[15].lexeme == "34");
    assert(tokens[15].double_number == 0.0);
    assert(tokens[15].int_value == 34);

    // Token 16
    assert(tokens[16].type == TokenType::MINUS);
    assert(tokens[16].lexeme == "-");
    assert(tokens[16].double_number == 0.0);
    assert(tokens[16].int_value == 0);

    // Token 17
    assert(tokens[17].type == TokenType::MATRIX);
    assert(tokens[17].lexeme == "A5");
    assert(tokens[17].double_number == 0.0);
    assert(tokens[17].int_value == 0);

    // Token 18
    assert(tokens[18].type == TokenType::END);
    assert(tokens[18].lexeme == "");
    assert(tokens[18].double_number == 0.0);
    assert(tokens[18].int_value == 0);

    std::cout << " Basic tokenization tests passed!\n";
}

void run_tests_for_supplement_with_implied_tokens(){
    std::string input = "44A1 + (13)( 52 * 34 - 2)A2^34 - A5";
    std::vector<Token> tokens = tokenize(input);

    supplement_with_implied_tokens(tokens);

    assert(tokens.size() == 22);

    // Token 0
    assert(tokens[0].type == TokenType::NUMBER);
    assert(tokens[0].lexeme == "44");
    assert(tokens[0].double_number == 0.0);
    assert(tokens[0].int_value == 44);

    // Token 1
    assert(tokens[1].type == TokenType::STAR);
    assert(tokens[1].lexeme == "");
    assert(tokens[1].double_number == 0.0);
    assert(tokens[1].int_value == 0);

    // Token 2
    assert(tokens[2].type == TokenType::MATRIX);
    assert(tokens[2].lexeme == "A1");
    assert(tokens[2].double_number == 0.0);
    assert(tokens[2].int_value == 0);

    // Token 3
    assert(tokens[3].type == TokenType::PLUS);
    assert(tokens[3].lexeme == "+");
    assert(tokens[3].double_number == 0.0);
    assert(tokens[3].int_value == 0);

    // Token 4
    assert(tokens[4].type == TokenType::LPAREN);
    assert(tokens[4].lexeme == "(");
    assert(tokens[4].double_number == 0.0);
    assert(tokens[4].int_value == 0);

    // Token 5
    assert(tokens[5].type == TokenType::NUMBER);
    assert(tokens[5].lexeme == "13");
    assert(tokens[5].double_number == 0.0);
    assert(tokens[5].int_value == 13);

    // Token 6
    assert(tokens[6].type == TokenType::RPAREN);
    assert(tokens[6].lexeme == ")");
    assert(tokens[6].double_number == 0.0);
    assert(tokens[6].int_value == 0);

    // Token 7
    assert(tokens[7].type == TokenType::STAR);
    assert(tokens[7].lexeme == "");
    assert(tokens[7].double_number == 0.0);
    assert(tokens[7].int_value == 0);

    // Token 8
    assert(tokens[8].type == TokenType::LPAREN);
    assert(tokens[8].lexeme == "(");
    assert(tokens[8].double_number == 0.0);
    assert(tokens[8].int_value == 0);

    // Token 9
    assert(tokens[9].type == TokenType::NUMBER);
    assert(tokens[9].lexeme == "52");
    assert(tokens[9].double_number == 0.0);
    assert(tokens[9].int_value == 52);

    // Token 10
    assert(tokens[10].type == TokenType::STAR);
    assert(tokens[10].lexeme == "*");
    assert(tokens[10].double_number == 0.0);
    assert(tokens[10].int_value == 0);

    // Token 11
    assert(tokens[11].type == TokenType::NUMBER);
    assert(tokens[11].lexeme == "34");
    assert(tokens[11].double_number == 0.0);
    assert(tokens[11].int_value == 34);

    // Token 12
    assert(tokens[12].type == TokenType::MINUS);
    assert(tokens[12].lexeme == "-");
    assert(tokens[12].double_number == 0.0);
    assert(tokens[12].int_value == 0);

    // Token 13
    assert(tokens[13].type == TokenType::NUMBER);
    assert(tokens[13].lexeme == "2");
    assert(tokens[13].double_number == 0.0);
    assert(tokens[13].int_value == 2);

    // Token 14
    assert(tokens[14].type == TokenType::RPAREN);
    assert(tokens[14].lexeme == ")");
    assert(tokens[14].double_number == 0.0);
    assert(tokens[14].int_value == 0);

    // Token 15
    assert(tokens[15].type == TokenType::STAR);
    assert(tokens[15].lexeme == "");
    assert(tokens[15].double_number == 0.0);
    assert(tokens[15].int_value == 0);

    // Token 16
    assert(tokens[16].type == TokenType::MATRIX);
    assert(tokens[16].lexeme == "A2");
    assert(tokens[16].double_number == 0.0);
    assert(tokens[16].int_value == 0);

    // Token 17
    assert(tokens[17].type == TokenType::CARET);
    assert(tokens[17].lexeme == "^");
    assert(tokens[17].double_number == 0.0);
    assert(tokens[17].int_value == 0);

    // Token 18
    assert(tokens[18].type == TokenType::NUMBER);
    assert(tokens[18].lexeme == "34");
    assert(tokens[18].double_number == 0.0);
    assert(tokens[18].int_value == 34);

    // Token 19
    assert(tokens[19].type == TokenType::MINUS);
    assert(tokens[19].lexeme == "-");
    assert(tokens[19].double_number == 0.0);
    assert(tokens[19].int_value == 0);

    // Token 20
    assert(tokens[20].type == TokenType::MATRIX);
    assert(tokens[20].lexeme == "A5");
    assert(tokens[20].double_number == 0.0);
    assert(tokens[20].int_value == 0);

    // Token 21
    assert(tokens[21].type == TokenType::END);
    assert(tokens[21].lexeme == "");
    assert(tokens[21].double_number == 0.0);
    assert(tokens[21].int_value == 0);

    std::cout << " All tokenizer tests (including implied STAR) passed!\n";
}

void run_tests_for_check_arithmetic_signs() {
    // === Тест 1: корректное выражение — должно пройти ===
    {
        std::vector<Token> tokens = {
            {TokenType::NUMBER, "5", 5.0, 5},
            {TokenType::PLUS, "+", 0.0, 0},
            {TokenType::NUMBER, "3", 3.0, 3},
            {TokenType::END, "", 0.0, 0}
        };
        check_arithmetic_signs(tokens);
    }

    // === Тест 2: допустимая пара "* -" ===
    {
        std::vector<Token> tokens = {
            {TokenType::NUMBER, "4", 4.0, 4},
            {TokenType::STAR, "*", 0.0, 0},
            {TokenType::MINUS, "-", 0.0, 0},
            {TokenType::NUMBER, "2", 2.0, 2},
            {TokenType::END, "", 0.0, 0}
        };
        check_arithmetic_signs(tokens);
    }

    // === Тест 3: допустимая пара "+ -" ===
    {
        std::vector<Token> tokens = {
            {TokenType::NUMBER, "1", 1.0, 1},
            {TokenType::PLUS, "+", 0.0, 0},
            {TokenType::MINUS, "-", 0.0, 0},
            {TokenType::NUMBER, "7", 7.0, 7},
            {TokenType::END, "", 0.0, 0}
        };
        check_arithmetic_signs(tokens);
    }

    // === Тест 4: недопустимая пара "+ +" ===
    {
        std::vector<Token> tokens = {
            {TokenType::NUMBER, "2", 2.0, 2},
            {TokenType::PLUS, "+", 0.0, 0},
            {TokenType::PLUS, "+", 0.0, 0},
            {TokenType::NUMBER, "3", 3.0, 3},
            {TokenType::END, "", 0.0, 0}
        };
        bool threw = false;
        try {
            check_arithmetic_signs(tokens);
        } catch (const std::runtime_error& e) {
            threw = true;
            assert(std::string(e.what()) == "Invalid pair of arithmetic symbols");
        }
        assert(threw);
    }

    // === Тест 5: три знака подряд — запрещено ===
    {
        std::vector<Token> tokens = {
            {TokenType::NUMBER, "1", 1.0, 1},
            {TokenType::PLUS, "+", 0.0, 0},
            {TokenType::MINUS, "-", 0.0, 0},
            {TokenType::STAR, "*", 0.0, 0},
            {TokenType::NUMBER, "4", 4.0, 4},
            {TokenType::END, "", 0.0, 0}
        };
        bool threw = false;
        try {
            check_arithmetic_signs(tokens);
        } catch (const std::runtime_error& e) {
            threw = true;
            assert(std::string(e.what()) == "Sequence of three arithmetic symbols is forbidden");
        }
        assert(threw);
    }

    // === Тест 6: CARET без NUMBER после — ошибка ===
    {
        std::vector<Token> tokens = {
            {TokenType::NUMBER, "2", 2.0, 2},
            {TokenType::CARET, "^", 0.0, 0},
            {TokenType::PLUS, "+", 0.0, 0},
            {TokenType::END, "", 0.0, 0}
        };
        bool threw = false;
        try {
            check_arithmetic_signs(tokens);
        } catch (const std::runtime_error& e) {
            threw = true;
            assert(std::string(e.what()) == "After caret( ^ ) must follow Number");
        }
        assert(threw);
    }

    // === Тест 7: CARET перед END — ошибка ===
    {
        std::vector<Token> tokens = {
            {TokenType::NUMBER, "3", 3.0, 3},
            {TokenType::CARET, "^", 0.0, 0},
            {TokenType::END, "", 0.0, 0}
        };
        bool threw = false;
        try {
            check_arithmetic_signs(tokens);
        } catch (const std::runtime_error& e) {
            threw = true;
            assert(std::string(e.what()) == "After caret( ^ ) must follow Number");
        }
        assert(threw);
    }

    // === Тест 8: выражение заканчивается на арифметический знак — ошибка ===
    {
        std::vector<Token> tokens = {
            {TokenType::NUMBER, "5", 5.0, 5},
            {TokenType::PLUS, "+", 0.0, 0},
            {TokenType::END, "", 0.0, 0}
        };
        bool threw = false;
        try {
            check_arithmetic_signs(tokens);
        } catch (const std::runtime_error& e) {
            threw = true;
            assert(std::string(e.what()) == "Last symbol could not be arithmetic");
        }
        assert(threw);
    }

    // === Тест 9: первый символ — PLUS — запрещено ===
    {
        std::vector<Token> tokens = {
            {TokenType::PLUS, "+", 0.0, 0},
            {TokenType::NUMBER, "4", 4.0, 4},
            {TokenType::END, "", 0.0, 0}
        };
        bool threw = false;
        try {
            check_arithmetic_signs(tokens);
        } catch (const std::runtime_error& e) {
            threw = true;
            assert(std::string(e.what()) == "Forbidden first arithmetic symbol");
        }
        assert(threw);
    }

    // === Тест 10: первый символ — MINUS — разрешено (унарный минус) ===
    {
        std::vector<Token> tokens = {
            {TokenType::MINUS, "-", 0.0, 0},
            {TokenType::NUMBER, "9", 9.0, 9},
            {TokenType::END, "", 0.0, 0}
        };
        check_arithmetic_signs(tokens); // должно пройти
    }

    // === Тест 11: первый символ — STAR — запрещено ===
    {
        std::vector<Token> tokens = {
            {TokenType::STAR, "*", 0.0, 0},
            {TokenType::NUMBER, "2", 2.0, 2},
            {TokenType::END, "", 0.0, 0}
        };
        bool threw = false;
        try {
            check_arithmetic_signs(tokens);
        } catch (const std::runtime_error& e) {
            threw = true;
            assert(std::string(e.what()) == "Forbidden first arithmetic symbol");
        }
        assert(threw);
    }

    // === Тест 12: корректное использование CARET с числом ===
    {
        std::vector<Token> tokens = {
            {TokenType::NUMBER, "2", 2.0, 2},
            {TokenType::CARET, "^", 0.0, 0},
            {TokenType::NUMBER, "10", 10.0, 10},
            {TokenType::END, "", 0.0, 0}
        };
        check_arithmetic_signs(tokens); // должно пройти
    }

    std::cout << "All arithmetic signs validation tests passed!\n";
}

void run_tests_for_replace_minus_in_sign_pairs() {
    // === Тест 1: замена "+ -" на "+ ( -1 ) *" ===
    {
        std::vector<Token> tokens = {
            {TokenType::NUMBER, "5", 5.0, 5},
            {TokenType::PLUS, "+", 0.0, 0},
            {TokenType::MINUS, "-", 0.0, 0},
            {TokenType::NUMBER, "3", 3.0, 3},
            {TokenType::END, "", 0.0, 0}
        };

        replace_minus_in_sign_pairs(tokens);

        assert(tokens.size() == 8);

        // Token 0
        assert(tokens[0].type == TokenType::NUMBER);
        assert(tokens[0].lexeme == "5");
        assert(tokens[0].double_number == 5.0);
        assert(tokens[0].int_value == 5);

        // Token 1
        assert(tokens[1].type == TokenType::PLUS);
        assert(tokens[1].lexeme == "+");
        assert(tokens[1].double_number == 0.0);
        assert(tokens[1].int_value == 0);

        // Token 2
        assert(tokens[2].type == TokenType::LPAREN);
        assert(tokens[2].lexeme == "(");
        assert(tokens[2].double_number == 0.0);
        assert(tokens[2].int_value == 0);

        // Token 3
        assert(tokens[3].type == TokenType::NUMBER);
        assert(tokens[3].lexeme == "-1");
        assert(tokens[3].double_number == -1.0);
        assert(tokens[3].int_value == -1);  // ← важно!

        // Token 4
        assert(tokens[4].type == TokenType::RPAREN);
        assert(tokens[4].lexeme == ")");
        assert(tokens[4].double_number == 0.0);
        assert(tokens[4].int_value == 0);

        // Token 5
        assert(tokens[5].type == TokenType::STAR);
        assert(tokens[5].lexeme == "*");
        assert(tokens[5].double_number == 0.0);
        assert(tokens[5].int_value == 0);

        // Token 6
        assert(tokens[6].type == TokenType::NUMBER);
        assert(tokens[6].lexeme == "3");
        assert(tokens[6].double_number == 3.0);
        assert(tokens[6].int_value == 3);

        // Token 7
        assert(tokens[7].type == TokenType::END);
        assert(tokens[7].lexeme == "");
        assert(tokens[7].double_number == 0.0);
        assert(tokens[7].int_value == 0);
    }

    // === Тест 2: замена "* -" на "* ( -1 ) *" ===
    {
        std::vector<Token> tokens = {
            {TokenType::NUMBER, "4", 4.0, 4},
            {TokenType::STAR, "*", 0.0, 0},
            {TokenType::MINUS, "-", 0.0, 0},
            {TokenType::MATRIX, "A1", 0.0, 0},
            {TokenType::END, "", 0.0, 0}
        };

        replace_minus_in_sign_pairs(tokens);

        assert(tokens.size() == 8);

        assert(tokens[0].type == TokenType::NUMBER);
        assert(tokens[0].lexeme == "4");
        assert(tokens[0].double_number == 4.0);
        assert(tokens[0].int_value == 4);

        assert(tokens[1].type == TokenType::STAR);
        assert(tokens[1].lexeme == "*");
        assert(tokens[1].double_number == 0.0);
        assert(tokens[1].int_value == 0);

        assert(tokens[2].type == TokenType::LPAREN);
        assert(tokens[2].lexeme == "(");
        assert(tokens[2].double_number == 0.0);
        assert(tokens[2].int_value == 0);

        assert(tokens[3].type == TokenType::NUMBER);
        assert(tokens[3].lexeme == "-1");
        assert(tokens[3].double_number == -1.0);
        assert(tokens[3].int_value == -1);

        assert(tokens[4].type == TokenType::RPAREN);
        assert(tokens[4].lexeme == ")");
        assert(tokens[4].double_number == 0.0);
        assert(tokens[4].int_value == 0);

        assert(tokens[5].type == TokenType::STAR);
        assert(tokens[5].lexeme == "*");
        assert(tokens[5].double_number == 0.0);
        assert(tokens[5].int_value == 0);

        assert(tokens[6].type == TokenType::MATRIX);
        assert(tokens[6].lexeme == "A1");
        assert(tokens[6].double_number == 0.0);
        assert(tokens[6].int_value == 0);

        assert(tokens[7].type == TokenType::END);
        assert(tokens[7].lexeme == "");
        assert(tokens[7].double_number == 0.0);
        assert(tokens[7].int_value == 0);
    }

    // === Тест 3: несколько замен в одном выражении ===
    {
        std::vector<Token> tokens = {
            {TokenType::NUMBER, "2", 2.0, 2},
            {TokenType::PLUS, "+", 0.0, 0},
            {TokenType::MINUS, "-", 0.0, 0},
            {TokenType::NUMBER, "5", 5.0, 5},
            {TokenType::STAR, "*", 0.0, 0},
            {TokenType::MINUS, "-", 0.0, 0},
            {TokenType::NUMBER, "7", 7.0, 7},
            {TokenType::END, "", 0.0, 0}
        };

        replace_minus_in_sign_pairs(tokens);

        assert(tokens.size() == 14);

        // После: 2 + ( -1 ) * 5 * ( -1 ) * 7

        // Проверка первой замены: + -5 → + ( -1 ) * 5
        assert(tokens[2].type == TokenType::LPAREN);
        assert(tokens[3].type == TokenType::NUMBER);
        assert(tokens[3].lexeme == "-1");
        assert(tokens[3].double_number == -1.0);
        assert(tokens[3].int_value == -1);
        assert(tokens[4].type == TokenType::RPAREN);
        assert(tokens[5].type == TokenType::STAR);
        assert(tokens[6].type == TokenType::NUMBER);
        assert(tokens[6].lexeme == "5");

        // Проверка второй замены: * -7 → * ( -1 ) * 7
        assert(tokens[7].type == TokenType::STAR);        // звёздочка между 5 и ( -1 )
        assert(tokens[8].type == TokenType::LPAREN);
        assert(tokens[9].type == TokenType::NUMBER);
        assert(tokens[9].lexeme == "-1");
        assert(tokens[9].double_number == -1.0);
        assert(tokens[9].int_value == -1);
        assert(tokens[10].type == TokenType::RPAREN);
        assert(tokens[11].type == TokenType::STAR);
        assert(tokens[12].type == TokenType::NUMBER);
        assert(tokens[12].lexeme == "7");

        assert(tokens[13].type == TokenType::END);
    }

    // === Тест 4: унарный минус в начале — НЕ обрабатывается (остаётся для replace_first_unary_minus) ===
    {
        std::vector<Token> tokens = {
            {TokenType::MINUS, "-", 0.0, 0},
            {TokenType::NUMBER, "9", 9.0, 9},
            {TokenType::END, "", 0.0, 0}
        };

        std::vector<Token> original = tokens;
        replace_minus_in_sign_pairs(tokens);

        assert(tokens.size() == original.size());
        for (size_t i = 0; i < tokens.size(); ++i) {
            assert(tokens[i].type == original[i].type);
            assert(tokens[i].lexeme == original[i].lexeme);
            assert(tokens[i].double_number == original[i].double_number);
            assert(tokens[i].int_value == original[i].int_value);
        }
    }

    // === Тест 5: бинарный минус (5 - 3) — не заменяется ===
    {
        std::vector<Token> tokens = {
            {TokenType::NUMBER, "5", 5.0, 5},
            {TokenType::MINUS, "-", 0.0, 0},
            {TokenType::NUMBER, "3", 3.0, 3},
            {TokenType::END, "", 0.0, 0}
        };

        std::vector<Token> original = tokens;
        replace_minus_in_sign_pairs(tokens);

        assert(tokens.size() == original.size());
        for (size_t i = 0; i < tokens.size(); ++i) {
            assert(tokens[i].type == original[i].type);
            assert(tokens[i].lexeme == original[i].lexeme);
            assert(tokens[i].double_number == original[i].double_number);
            assert(tokens[i].int_value == original[i].int_value);
        }
    }

    // === Тест 6: выражение без унарного минуса — без изменений ===
    {
        std::vector<Token> tokens = {
            {TokenType::NUMBER, "2", 2.0, 2},
            {TokenType::PLUS, "+", 0.0, 0},
            {TokenType::NUMBER, "3", 3.0, 3},
            {TokenType::STAR, "*", 0.0, 0},
            {TokenType::NUMBER, "4", 4.0, 4},
            {TokenType::END, "", 0.0, 0}
        };

        std::vector<Token> original = tokens;
        replace_minus_in_sign_pairs(tokens);

        assert(tokens.size() == original.size());
        for (size_t i = 0; i < tokens.size(); ++i) {
            assert(tokens[i].type == original[i].type);
            assert(tokens[i].lexeme == original[i].lexeme);
            assert(tokens[i].double_number == original[i].double_number);
            assert(tokens[i].int_value == original[i].int_value);
        }
    }

    std::cout << " replace_minus_in_sign_pairs tests passed!\n";
}

void run_tests_for_replace_first_unary_minus() {
    // === Тест: унарный минус в начале заменяется на (-1)* ===
    {
        std::vector<Token> tokens = {
            {TokenType::MINUS, "-", 0.0, 0},
            {TokenType::NUMBER, "7", 7.0, 7},
            {TokenType::END, "", 0.0, 0}
        };

        replace_first_unary_minus(tokens);

        // Token 0
        assert(tokens[0].type == TokenType::LPAREN);
        assert(tokens[0].lexeme == "(");
        assert(tokens[0].double_number == 0.0);
        assert(tokens[0].int_value == 0);

        // Token 1
        assert(tokens[1].type == TokenType::MINUS);
        assert(tokens[1].lexeme == "-");
        assert(tokens[1].double_number == 0.0);
        assert(tokens[1].int_value == 0);

        // Token 2
        assert(tokens[2].type == TokenType::NUMBER);
        assert(tokens[2].lexeme == "1");
        assert(tokens[2].double_number == 1.0);
        assert(tokens[2].int_value == 1);

        // Token 3
        assert(tokens[3].type == TokenType::RPAREN);
        assert(tokens[3].lexeme == ")");
        assert(tokens[3].double_number == 0.0);
        assert(tokens[3].int_value == 0);

        // Token 4
        assert(tokens[4].type == TokenType::STAR);
        assert(tokens[4].lexeme == "*");
        assert(tokens[4].double_number == 0.0);
        assert(tokens[4].int_value == 0);

        // Token 5
        assert(tokens[5].type == TokenType::NUMBER);
        assert(tokens[5].lexeme == "7");
        assert(tokens[5].double_number == 7.0);
        assert(tokens[5].int_value == 7);

        // Token 6
        assert(tokens[6].type == TokenType::END);
        assert(tokens[6].lexeme == "");
        assert(tokens[6].double_number == 0.0);
        assert(tokens[6].int_value == 0);

        assert(tokens.size() == 7);
    }

    // === Тест: выражение без унарного минуса в начале — без изменений ===
    {
        std::vector<Token> tokens = {
            {TokenType::NUMBER, "3", 3.0, 3},
            {TokenType::PLUS, "+", 0.0, 0},
            {TokenType::NUMBER, "4", 4.0, 4},
            {TokenType::END, "", 0.0, 0}
        };

        std::vector<Token> original = tokens;
        replace_first_unary_minus(tokens);

        assert(tokens.size() == original.size());
        for (size_t i = 0; i < tokens.size(); ++i) {
            assert(tokens[i].type == original[i].type);
            assert(tokens[i].lexeme == original[i].lexeme);
            assert(tokens[i].double_number == original[i].double_number);
            assert(tokens[i].int_value == original[i].int_value);
        }
    }

    // === Тест: только MINUS и END (крайний, но допустимый после валидации? — на всякий случай) ===
    // Примечание: check_arithmetic_signs не пропустит такое, но проверим устойчивость
    {
        std::vector<Token> tokens = {
            {TokenType::MINUS, "-", 0.0, 0},
            {TokenType::END, "", 0.0, 0}
        };

        replace_first_unary_minus(tokens);

        assert(tokens[0].type == TokenType::LPAREN);
        assert(tokens[1].type == TokenType::MINUS);
        assert(tokens[2].type == TokenType::NUMBER);
        assert(tokens[2].double_number == 1.0);
        assert(tokens[3].type == TokenType::RPAREN);
        assert(tokens[4].type == TokenType::STAR);
        assert(tokens[5].type == TokenType::END);
        assert(tokens.size() == 6);
    }

    std::cout << " replace_first_unary_minus tests passed!\n";
}

void run_tests_for_replace_unary_pluses() {
    // === Тест 1: унарный плюс в начале выражения ===
    {
        std::vector<Token> tokens = {
            {TokenType::PLUS, "+", 0.0, 0},
            {TokenType::NUMBER, "123", 123.0, 123},
            {TokenType::END, "", 0.0, 0}
        };

        replace_unary_pluses(tokens);

        assert(tokens.size() == 2);

        // Token 0
        assert(tokens[0].type == TokenType::NUMBER);
        assert(tokens[0].lexeme == "123");
        assert(tokens[0].double_number == 123.0);
        assert(tokens[0].int_value == 123);

        // Token 1
        assert(tokens[1].type == TokenType::END);
        assert(tokens[1].lexeme == "");
        assert(tokens[1].double_number == 0.0);
        assert(tokens[1].int_value == 0);
    }

    // === Тест 2: унарный плюс после открывающей скобки ===
    {
        std::vector<Token> tokens = {
            {TokenType::LPAREN, "(", 0.0, 0},
            {TokenType::PLUS, "+", 0.0, 0},
            {TokenType::MATRIX, "A5", 0.0, 0},
            {TokenType::RPAREN, ")", 0.0, 0},
            {TokenType::END, "", 0.0, 0}
        };

        replace_unary_pluses(tokens);

        assert(tokens.size() == 4);

        // Token 0
        assert(tokens[0].type == TokenType::LPAREN);
        assert(tokens[0].lexeme == "(");
        assert(tokens[0].double_number == 0.0);
        assert(tokens[0].int_value == 0);

        // Token 1
        assert(tokens[1].type == TokenType::MATRIX);
        assert(tokens[1].lexeme == "A5");
        assert(tokens[1].double_number == 0.0);
        assert(tokens[1].int_value == 0);

        // Token 2
        assert(tokens[2].type == TokenType::RPAREN);
        assert(tokens[2].lexeme == ")");
        assert(tokens[2].double_number == 0.0);
        assert(tokens[2].int_value == 0);

        // Token 3
        assert(tokens[3].type == TokenType::END);
        assert(tokens[3].lexeme == "");
        assert(tokens[3].double_number == 0.0);
        assert(tokens[3].int_value == 0);
    }

    // === Тест 3: несколько унарных плюсов подряд в начале ===
    {
        std::vector<Token> tokens = {
            {TokenType::PLUS, "+", 0.0, 0},
            {TokenType::PLUS, "+", 0.0, 0},
            {TokenType::PLUS, "+", 0.0, 0},
            {TokenType::NUMBER, "7", 7.0, 7},
            {TokenType::END, "", 0.0, 0}
        };

        replace_unary_pluses(tokens);

        assert(tokens.size() == 2);

        // Token 0
        assert(tokens[0].type == TokenType::NUMBER);
        assert(tokens[0].lexeme == "7");
        assert(tokens[0].double_number == 7.0);
        assert(tokens[0].int_value == 7);

        // Token 1
        assert(tokens[1].type == TokenType::END);
        assert(tokens[1].lexeme == "");
        assert(tokens[1].double_number == 0.0);
        assert(tokens[1].int_value == 0);
    }

    // === Тест 4: бинарный плюс — не должен удаляться ===
    {
        std::vector<Token> tokens = {
            {TokenType::NUMBER, "4", 4.0, 4},
            {TokenType::PLUS, "+", 0.0, 0},
            {TokenType::NUMBER, "9", 9.0, 9},
            {TokenType::END, "", 0.0, 0}
        };

        std::vector<Token> original = tokens;
        replace_unary_pluses(tokens);

        assert(tokens.size() == original.size());
        for (size_t i = 0; i < tokens.size(); ++i) {
            assert(tokens[i].type == original[i].type);
            assert(tokens[i].lexeme == original[i].lexeme);
            assert(tokens[i].double_number == original[i].double_number);
            assert(tokens[i].int_value == original[i].int_value);
        }
    }

    // === Тест 5: плюс после оператора (например, после STAR) — бинарный, не удаляется ===
    {
        std::vector<Token> tokens = {
            {TokenType::NUMBER, "2", 2.0, 2},
            {TokenType::STAR, "*", 0.0, 0},
            {TokenType::PLUS, "+", 0.0, 0},
            {TokenType::NUMBER, "3", 3.0, 3},
            {TokenType::END, "", 0.0, 0}
        };

        std::vector<Token> original = tokens;
        replace_unary_pluses(tokens);

        assert(tokens.size() == original.size());
        for (size_t i = 0; i < tokens.size(); ++i) {
            assert(tokens[i].type == original[i].type);
            assert(tokens[i].lexeme == original[i].lexeme);
            assert(tokens[i].double_number == original[i].double_number);
            assert(tokens[i].int_value == original[i].int_value);
        }
    }

    // === Тест 6: унарный плюс внутри вложенных скобок ===
    {
        std::vector<Token> tokens = {
            {TokenType::LPAREN, "(", 0.0, 0},
            {TokenType::LPAREN, "(", 0.0, 0},
            {TokenType::PLUS, "+", 0.0, 0},
            {TokenType::NUMBER, "1", 1.0, 1},
            {TokenType::RPAREN, ")", 0.0, 0},
            {TokenType::RPAREN, ")", 0.0, 0},
            {TokenType::END, "", 0.0, 0}
        };

        replace_unary_pluses(tokens);

        assert(tokens.size() == 6);

        assert(tokens[0].type == TokenType::LPAREN);
        assert(tokens[1].type == TokenType::LPAREN);
        assert(tokens[2].type == TokenType::NUMBER);
        assert(tokens[2].lexeme == "1");
        assert(tokens[2].double_number == 1.0);
        assert(tokens[2].int_value == 1);
        assert(tokens[3].type == TokenType::RPAREN);
        assert(tokens[4].type == TokenType::RPAREN);
        assert(tokens[5].type == TokenType::END);
    }

    std::cout << " replace_unary_pluses tests passed!\n";
}

void run_tests_for_normalize_tokenize_sequence() {
    {
        std::string input = "A3 -4A4 - 5A5 + 2*-A5";
        std::vector<Token> tokens = tokenize(input);
        check_arithmetic_signs( tokens );
        normalize_tokenize_sequence( tokens );
        // for( int i = 0; i < tokens.size(); i++) {
        //     std::cout << token_type_to_string( tokens[i].type ) << "\"" << tokens[i].lexeme << "\"";
        // }"
        Parser parser(tokens);
        std::unique_ptr<raw_AST_nodes::Expression> AST = parser.parse();
        //print_raw_AST_to_file("Dot_example", *AST);
        TypedResult typed_result = typecheck(*AST);

        print_raw_AST_to_file("Dot_example", *(typed_result.node));
    }
}
