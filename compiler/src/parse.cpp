#include <parse.hpp>
#include <cstdlib>
#include <iostream>

static std::string type_to_str(int type) {
    if (type == ass::token_type::NEWLINE)
        return "newline";
    else if (type == ass::token_type::NUMBER)
        return "number";
    else if (type == ass::token_type::IDENT)
        return "identifier";
    else if (type == ass::token_type::STRING)
        return "string";
    else if (type == ass::token_type::SEMICOLON)
        return ";";
    else if (type == ass::token_type::DOUBLECOLON)
        return "::";
    else if (type == ass::token_type::LPAREN)
        return "(";
    else if (type == ass::token_type::RPAREN)
        return ")";
    else if (type == ass::token_type::LBRACKET)
        return "{";
    else if (type == ass::token_type::RBRACKET)
        return "}";
    else if (type == ass::token_type::COMA)
        return ",";
    else if (type == ass::token_type::FN)
        return "fn";
    else if (type == ass::token_type::IMPORT)
        return "import";
    else if (type == ass::token_type::EQ)
        return "=";
    else if (type == ass::token_type::PLUS)
        return "+";
    else if (type == ass::token_type::MINUS)
        return "-";
    else if (type == ass::token_type::ASTERISK)
        return "*";
    else if (type == ass::token_type::SLASH)
        return "/";
    else if (type == ass::token_type::EQEQ)
        return "==";
    else if (type == ass::token_type::NOTEQ)
        return "!=";
    else if (type == ass::token_type::LT)
        return "<";
    else if (type == ass::token_type::LTEQ)
        return "<=";
    else if (type == ass::token_type::GT)
        return ">";
    else if (type == ass::token_type::GTEQ)
        return ">=";
    
    return "";
}

ass::parser::parser(ass::lexer *lexer, ass::emitter *emitter) {
    this->lexer = lexer;
    this->emitter = emitter;

    next_token();
    next_token();
    this->line = 1;
}

bool ass::parser::check_token(int type) {
    return type == curToken.type;
}

bool ass::parser::check_peek(int type) {
    return type == peekToken.type;
}

void ass::parser::match(int type) {
    if (!check_token(type))
        abort("Expected `" + type_to_str(type) + "`, got `" + type_to_str(type) + "`, at line " + std::to_string(line));
    next_token();
}

void ass::parser::next_token() {
    curToken = peekToken;
    peekToken = lexer->get_token();
}

bool ass::parser::iscmp() {
    return check_token(GT) || check_token(GTEQ) || check_token(LT) || check_token(LTEQ) || check_token(EQEQ) || check_token(NOTEQ);
}

void ass::parser::abort(std::string message) {
    std::cout << "Error! " + message + "\n";
    std::exit(1);
}

void ass::parser::program() {
    write = [&](std::string message){
        emitter->emit_line(message);
    };

    while (check_token(NEWLINE))
        next_token();
    
    while (!check_token(THEEOF))
        statement(write);
}

void ass::parser::statement(std::function<void(std::string)>& write) {
    if (check_token(IMPORT)) {
        next_token();

        if (check_token(STRING)) {
            if (curToken.text == "io") {
                emitter->header_line("#include <iostream>");
            } else {
                abort("Cannot find package `" + curToken.text + "`, at line " + std::to_string(line));
            }
            next_token();
            match(SEMICOLON);
        } else {
            abort("Expected `string`, but got `" + type_to_str(curToken.type) + "` after `import`, at line " + std::to_string(line));
        }
    } else if (check_token(FN)) {
        next_token();
        function func;
        func.name = curToken.text;
        next_token();

        match(LPAREN);
        // Parse function arguments
        while (!check_token(RPAREN)) {
            std::string at, an;
            at = curToken.text;
            match(IDENT);
            an = curToken.text;
            func.args.push_back(std::make_pair(at, an));
            match(IDENT);
            if (check_token(COMA)) {
                match(COMA);
            }
        }
        match(RPAREN);

        // Get return type of function
        if (check_token(IDENT)) {
            func.type = curToken.text;
            match(IDENT);
        } else {
            func.type = "void";
        }

        std::string f = func.type + " " + func.name + "(";
        bool was = false;
        std::for_each(func.args.begin(), func.args.end(), [&f, &was](std::pair<std::string, std::string> arg){
            if (was) f += ", ";
            else was = true;

            f += arg.first + " " + arg.second;
        });
        f += ") {";
        write(f);

        match(LBRACKET);

        while (!check_token(RBRACKET)) {
            statement(write);
        }

        match(RBRACKET);
        if (func.name == "main")
            write("return 0;");
        write("}");
    } else if (check_token(IDENT)) {
        // Check if it's call of library function
        if (curToken.text == "io") {
            match(IDENT);
            match(DOUBLECOLON);

            if (curToken.text == "write") {
                match(IDENT);
                match(LPAREN);
                write("std::cout");
                while (!check_token(RPAREN)) {
                    if (check_token(STRING)) {
                        write(" << \"" + curToken.text + "\"");
                    } else if (check_token(NUMBER)) {
                        write(" << " + curToken.text);
                    }
                    next_token();
                }
                write(";\n");
                match(RPAREN);
                match(SEMICOLON);
            } else {
                abort("'io' library doesn't contains element `" + curToken.text + "`, at line " + std::to_string(line));
            }
        }
    } else if (check_token(NEWLINE)) {
        line++;
        match(NEWLINE);
    } else {
        std::cout << "watafuk\n";
    }
}
