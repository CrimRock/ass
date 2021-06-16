#include <lex.hpp>
#include <cstdlib>
#include <ctype.h>
#include <iostream>

ass::lexer::lexer(std::string input) {
    src = input + '\n';
    chr = 0;
    pos = -1;
    line = 1;
    next_char();
}

void ass::lexer::next_char() {
    pos++;
    if (pos >= src.length())
        chr = '\0';
    else
        chr = src[pos];
}

char ass::lexer::peek() {
    if (pos + 1 >= src.length())
        return '\0';
    return src[pos + 1];
}

void ass::lexer::abort(std::string message) {
    std::cout << pos << std::endl;
    std::cout << "Lexing error. " << message << std::endl;
    std::exit(1);
}

ass::token ass::lexer::get_token() {
    skip_whitespace();
    skip_comment();

    ass::token token;

    if (chr == '+') {
        token = ass::token("+", PLUS);
    } else if (chr == '-') {
        token = ass::token("-", MINUS);
    } else if (chr == '*') {
        token = ass::token("*", ASTERISK);
    } else if (chr == '/') {
        token = ass::token("/", SLASH);
    } else if (chr == ':') {
        if (peek() == ':') {
            next_char();
            token = ass::token("::", DOUBLECOLON);
        } else {
            abort("Expected ::, got :" + peek() + std::string(", at line ") + std::to_string(line));
        }
    } else if (chr == '=') {
        if (peek() == '=') {
            next_char();
            token = ass::token("==", EQEQ);
        } else {
            token = ass::token("=", EQ);
        }
    } else if (chr == '>') {
        if (peek() == '=') {
            next_char();
            token = ass::token(">=", GTEQ);
        } else {
            token = ass::token(">", GT);
        }
    } else if (chr == '<') {
        if (peek() == '=') {
            next_char();
            token = ass::token("<=", LTEQ);
        } else {
            token = ass::token("<", LT);
        }
    } else if (chr == '!') {
        if (peek() == '=') {
            next_char();
            token = ass::token("!=", NOTEQ);
        } else {
            abort("Expected !=, got !" + peek() + std::string(", at line ") + std::to_string(line));
        }
    } else if (chr == ',') {
        token = ass::token(",", COMA);
    } else if (chr == '\"') {
        next_char();
        long start = pos;

        while (chr != '\"'/* && src[pos - 1] != '\\'*/) {
            if (chr == '\n')
                abort("Illegal character in string, at line " + std::to_string(line));
            next_char();
        }

        token = ass::token(src.substr(start, pos - start), STRING);
    } else if (chr == '{') {
        token = ass::token("{", LBRACKET);
    } else if (chr == '}') {
        token = ass::token("}", RBRACKET);
    } else if (chr == '(') {
        token = ass::token("(", LPAREN);
    } else if (chr == ')') {
        token = ass::token(")", RPAREN);
    } else if (chr == ';') {
        token = ass::token(";", SEMICOLON);
    } else if (isdigit(chr)) {
        long start = pos;
        while (isdigit(chr))
            next_char();
            
        if (peek() == '.') {
            next_char();

            if (!isdigit(peek())) {
                abort("Illegal character in number, at line " + std::to_string(line));
            }
            while (isdigit(peek())) {
                next_char();
            }
        }

        token = ass::token(src.substr(start, pos - start), NUMBER);
    } else if (isalpha(chr)) {
        long start = pos;
        while (isalnum(peek()))
            next_char();
        
        std::string text = src.substr(start, pos - start + 1);
        int keyword = ass::token::check_if_keyword(text);
        if (keyword == -2)
            token = ass::token(text, IDENT);
        else
            token = ass::token(text, keyword);
    } else if (chr == '\n') {
        token = ass::token("\n", NEWLINE);
        line++;
    } else if (chr == '\0') {
        token = ass::token("", THEEOF);
    } else {
        abort("Unknown token: " + chr + std::string(", at line ") + std::to_string(line));
    }

    next_char();
    return token;
}

std::string ass::lexer::get_line(long l) {
    std::string res = "";
    long cur = 0;

    for (char c : src) {
        if (cur == l) {
            res += c;
            continue;
        }

        if (c == '\n') cur++;
    }

    return res;
}

void ass::lexer::skip_whitespace() {
    while (chr == ' ' || chr == '\t' || chr == '\r') {
        next_char();
    }
}

void ass::lexer::skip_comment() {
    if (chr == '/' && peek() == '/') {
        while (chr != '\n') {
            next_char();
        }
    }
}

ass::token::token() {}

ass::token::token(std::string text, int type) {
    this->text = text;
    this->type = ass::token_type(type);
}

int ass::token::check_if_keyword(std::string text) {
    if (text == "import") {
        return IMPORT;
    } else if (text == "fn") {
        return FN;
    }

    return -2;
}
