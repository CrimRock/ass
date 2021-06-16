#ifndef __ASS_LEX_HPP__
#define __ASS_LEX_HPP__

#include <string>

namespace ass {
    enum token_type {
        THEEOF = -1,
        NEWLINE = 0,
        NUMBER = 1,
        IDENT = 2,
        STRING = 3,
        SEMICOLON = 4,
        DOUBLECOLON = 5,
        LPAREN = 6,
        RPAREN = 7,
        LBRACKET = 8,
        RBRACKET = 9,
        COMA = 10,
        // Keywords
        FN = 101,
        IMPORT = 102,
        // Operators
        EQ = 201,
        PLUS = 202,
        MINUS = 203,
        ASTERISK = 204,
        SLASH = 205,
        EQEQ = 206,
        NOTEQ = 207,
        LT = 208,
        LTEQ = 209,
        GT = 210,
        GTEQ = 211,
    };

    class token {
    public:
        enum token_type type;
        std::string text;

        token();
        token(std::string text, int type);

        static int check_if_keyword(std::string text);
    };

    class lexer {
    public:
        lexer(std::string input);
        void next_char();
        char peek();
        void abort(std::string message);
        token get_token();
        std::string get_line(long l);
        void skip_whitespace();
        void skip_comment();
    private:
        std::string src;
        long pos;
        int line;
        char chr;
    };
}

#endif // __ASS_LEX_HPP__