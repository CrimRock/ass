#ifndef __ASS_PARSE_HPP__
#define __ASS_PARSE_HPP__

#include <emit.hpp>
#include <lex.hpp>
#include <string>
#include <vector>
#include <functional>

namespace ass {
    struct variable {
        std::string type, value, name;
    };

    struct function {
        std::string name, type;
        // Argument is pair of strings - type and name.
        std::vector<std::pair<std::string, std::string>> args;
    };

    class parser {
    public:
        parser(ass::lexer *lexer, ass::emitter *emitter);
        bool check_token(int type);
        bool check_peek(int type);
        void match(int type);
        void next_token();
        bool iscmp();
        void abort(std::string message);
        void program();
        void statement(std::function<void(std::string)>& write);
        /*void comparison(std::function<void(std::string)>& write);
        void expression(std::function<void(std::string)>& write);
        void term(std::function<void(std::string)>& write);
        void unary(std::function<void(std::string)>& write);
        void primary(std::function<void(std::string)>& write);
        void newline();*/
    private:
        std::function<void(std::string)> write;
        std::vector<function> funcs;
        std::vector<variable> vars;
        ass::token curToken, peekToken;
        ass::lexer *lexer;
        ass::emitter *emitter;
        long line;
    };
}

#endif // __ASS_PARSE_HPP__