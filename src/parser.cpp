#include "parser.hpp"
#include <iostream>
#include <algorithm>

Symbol sym;
std::vector<std::string> dtypes = {"INTEGER", "REAL", "CHAR", "STRING", "BOOLEAN"};

Parser::Parser(std::vector<Token> arr): tokens(arr), index(0) {
    this->nextSym();
}

int is_dtype(std::string type) {
    std::cout << "Is_dtype called\n";
    for (int i = 0; i < dtypes.size(); ++i) {
        std::cout << dtypes[i] << std::endl;
        if (type == dtypes[i])
            return 1;
    }
    return 0;
}



void Parser::nextSym() {
    // std::cout << "tokens size: " << this->tokens.size() << " index: " << index << std::endl;
    if (index < this->tokens.size()) {
        sym = lexerToParser(this->tokens[index]);
        this->index++;
        // std::cout << "parser sym: " << sym << std::endl;
    }
}

Symbol lexerToParser(Token token) {
    switch (token.tokenName) {
    case IDENTIFIER:
        return Symbol::ident;
        break;
    case OPERATOR:
        if (token.tokenValue == "+") 
            return Symbol::plus;
        else if (token.tokenValue == "-")
            return Symbol::minus;
        else if (token.tokenValue == "*")
            return Symbol::star;
        else if (token.tokenValue == "/")
            return Symbol::slash;
        else if (token.tokenValue == "=")   
            return Symbol::eq;
        else if (token.tokenValue == "<>")
            return Symbol::neq;
        else if (token.tokenValue == ">")
            return Symbol::great;
        else if (token.tokenValue == "<")
            return Symbol::less;
        else if (token.tokenValue == ">=")
            return Symbol::greateq;
        else if (token.tokenValue == "<=")
            return Symbol::lesseq;
        else if (token.tokenValue == "<--")
            return Symbol::assignsym;
        else
            return Symbol::not_token;
        break;
    case SEPARATOR:
        if (token.tokenValue == "(")
            return Symbol::lparen;
        else if (token.tokenValue == ")")
            return Symbol::rparen;
        else if (token.tokenValue == ",")
            return Symbol::comma;
        else if (token.tokenValue == ":")
            return Symbol::colon;
        
        
        else
            return Symbol::not_token;
        break;
    case LITERAL:
        if (token.tokenValue.find('\'') == token.tokenValue.npos && token.tokenValue.find('"') == token.tokenValue.npos)
            return Symbol::num_lit;
        break;
    case KEYWORD:
        if (token.tokenValue == "NOT")
            return Symbol::notsym;
        else if (token.tokenValue == "AND")
            return Symbol::andsym;
        else if (token.tokenValue == "OR")
            return Symbol::orsym;
        else if (token.tokenValue == "DECLARE")
            return Symbol::declsym;
        else if (token.tokenValue == "INPUT")
            return Symbol::inputsym;
        else if (token.tokenValue == "IF")
            return Symbol::ifsym;
        else if (token.tokenValue == "THEN")
            return Symbol::thensym;
        else if (token.tokenValue == "ENDIF")
            return Symbol::eifsym;
        else if (token.tokenValue == "ELSE")
            return Symbol::elsesym;
        else if (token.tokenValue == "WHILE")
            return Symbol::whilesym;
        else if (token.tokenValue == "ENDWHILE")
            return Symbol::ewhilesym;
        else if (token.tokenValue == "TRUE")
            return Symbol::truesym;
        else if (token.tokenValue == "FALSE")
            return Symbol::falsesym;
        else if (is_dtype(token.tokenValue))
            return Symbol::dtype;
        else
            return Symbol::not_token;
    case WHITESPACE:
        if (token.tokenValue == "\n")
            return Symbol::eol;
        else if (token.tokenValue == "<tab>")
            return Symbol::indent;
        else
            return Symbol::not_token;
    default:
        return Symbol::not_token;
        break;
    }
}

int Parser::accept(Symbol s) {
    if (s == sym) {
        nextSym();
        return 1;
    }
    return 0;
}
int Parser::expect(Symbol s) {
    if (accept(s))
        return 1;
    std::cout << "Parser: unexpected token: " << tokens[index - 1].tokenValue << std::endl;
    return 0;
}

void Parser::factor() {
    std::cout << "factor called\n";
    if (accept(ident))
        ;
    else if (accept(num_lit)) {
        ;
    }
    else if (accept(lparen)) {
        expr();
        expect(rparen);
    } else {
        std::cout << "Factor: syntax error\n";
        std::cout << sym << std::endl;
        nextSym();
    }
}

void Parser::term() {
    std::cout << "term called\n";
    factor();
    while (sym == star || sym == slash) {
        nextSym();
        factor();
    }
}

void Parser::expr() {
    std::cout << "expr called\n";
    if (sym == plus || sym == minus)
        nextSym();
    term();
    while (sym == plus || sym == minus) {
        nextSym();
        term();
    }
}

/* TODO: Fix this
    Will not accept condition, if it is fully surrounded by parentheses
*/
void Parser::condition() {
    std::cout << "Condition called\n";
    if (accept(notsym))
        ;
    if (sym == truesym || sym == falsesym)
        nextSym();
    else
        expr();
    if (sym == eq || sym == neq || sym == great || sym == less || sym == greateq || sym == lesseq) {
        nextSym();
        if (sym == truesym || sym == falsesym)
            nextSym();
        else
            expr();
    } else
        std::cout << "Condition: error unexpected token " << sym << std::endl;
}

void Parser::bool_expr() {
    std::cout << "Bool_expr called\n";
    condition();
    while (sym == andsym || sym == orsym) {
        nextSym();
        condition();
    }
}

int Parser::statement() {
    std::cout << "Statement called\n";
    /* Consuming the indentation before statements, if they are inside of block */
    while (accept(indent))
        ;
    if (accept(declsym)) { // Declaration of vaeriable
        expect(ident);
        while (accept(comma))
            expect(ident);
        expect(colon);
        expect(dtype);
        expect(eol);
    } else if (accept(ident)) { // Assignment of variable
        expect(assignsym);
        expr();
        expect(eol);
    } else if (accept(inputsym)) { // Input statement
        expect(ident);
        expect(eol);
    } else if (accept(ifsym)) { // If statement
        bool_expr();
        if (accept(eol))
            ;
        expect(thensym); // Body of if statement
        expect(eol);
        block();

        if (accept(elsesym)) { // Optional else statement
            expect(eol);            
            block();
        } 
        expect(eifsym); // End of if statements
        expect(eol);
    } else if (accept(whilesym)) { // While statement
        bool_expr();
        expect(eol);
        block();
        expect(ewhilesym);
        expect(eol);
    } else {
        std::cout << "Statement: error unexpected token: " << sym << std::endl;
        return 0;
    }
    return 1;
}

void Parser::block() {
    std::cout << "Block called\n";
    while (sym != eifsym && sym != ewhilesym && sym != elsesym) {
        if (!statement()) 
            break;
        while (accept(indent))
            ;
        std::cout << "Block: sym is "<< sym << std::endl;
    }
}